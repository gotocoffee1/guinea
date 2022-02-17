#pragma once

#ifndef _WIDGET_HPP_
#define _WIDGET_HPP_

#include "imgui.h"
#include <string_view>

namespace ImGui
{
inline void TextUnformatted(std::string_view text)
{
    ImGui::TextUnformatted(text.data(), text.data() + text.size());
}

bool BufferingBar(const char* label,
                  float value,
                  const ImVec2& size_arg);

bool Spinner(const char* label,
             float radius    = 6,
             float thickness = 3);
} // namespace ImGui

namespace ImGui
{
using img_data = unsigned char*;

img_data LoadImageFromFile(const char* filename,
                           int* out_width,
                           int* out_height) noexcept;
img_data LoadImageFromMemory(const void* buffer,
                             int size,
                             int* out_width,
                             int* out_height) noexcept;

void UnLoadImage(img_data img) noexcept;

ImTextureID LoadTexture(const img_data image_data, int width, int height) noexcept;
void UnLoadTexture(ImTextureID texture) noexcept;

struct Img
{
    Img(img_data data, int width, int height) noexcept
        : _data{data}, _width{width}, _height{height}
    {
    }

    explicit Img(const char* filename) noexcept
    {
        _data = LoadImageFromFile(filename, &_width, &_height);
    }

    Img(const void* buffer, int size) noexcept
    {
        _data = LoadImageFromMemory(buffer, size, &_width, &_height);
    }

    // TODO
    Img(const Img&) noexcept = delete;
    Img& operator=(Img const&) noexcept = delete;

    Img(Img&& other) noexcept
        : _data{other._data}, _width{other._width}, _height{other._height}
    {
        other._data = nullptr;
    }

    Img& operator=(Img&& other) noexcept
    {
        if (&other != this)
        {
            _data   = other._data;
            _width  = other._width;
            _height = other._height;

            other._data = nullptr;
        }
        return *this;
    }

    ~Img() noexcept
    {
        UnLoadImage(_data);
    }

    ImVec2 size() const noexcept
    {
        return ImVec2{static_cast<float>(_width), static_cast<float>(_height)};
    }

    explicit operator bool() const noexcept
    {
        return _data != nullptr;
    }

    int width() const noexcept
    {
        return _width;
    }

    int height() const noexcept
    {
        return _height;
    }

    const img_data data() const noexcept
    {
        return _data;
    }

  private:
    img_data _data;
    int _width;
    int _height;
};

struct Texture
{
    Texture(ImTextureID id, int width, int height) noexcept
        : _id{id}, _width{width}, _height{height}
    {
    }

    explicit Texture(const Img& img) noexcept
        : _id{LoadTexture(img.data(), img.width(), img.height())}, _width{img.width()}, _height{img.height()}
    {
    }

    Texture(const Texture&) noexcept = delete;
    Texture& operator=(Texture const&) noexcept = delete;

    Texture(Texture&& other) noexcept
        : _id{other._id}, _width{other._width}, _height{other._height}
    {
        other._id = nullptr;
    }

    Texture& operator=(Texture&& other) noexcept
    {
        if (&other != this)
        {
            _id     = other._id;
            _width  = other._width;
            _height = other._height;

            other._id = nullptr;
        }
        return *this;
    }

    ~Texture() noexcept
    {
        UnLoadTexture(_id);
    }

    ImVec2 size() const noexcept
    {
        return ImVec2{static_cast<float>(_width), static_cast<float>(_height)};
    }

    explicit operator ImTextureID() const noexcept
    {
        return _id;
    }

    explicit operator bool() const noexcept
    {
        return _id != nullptr;
    }

    ImTextureID id() const noexcept
    {
        return _id;
    }

    int width() const noexcept
    {
        return _width;
    }

    int height() const noexcept
    {
        return _height;
    }

  private:
    ImTextureID _id;
    int _width;
    int _height;
};
} // namespace ImGui

namespace ImGui
{
template<typename T>
constexpr ImGuiDataType map_imgui_datatype() noexcept
{
    if constexpr (false)
        ;
    else if constexpr (std::is_same_v<float, T>)
        return ImGuiDataType_Float;
    else if constexpr (std::is_same_v<double, T>)
        return ImGuiDataType_Double;
    else if constexpr (std::is_same_v<int8_t, T>)
        return ImGuiDataType_S8;
    else if constexpr (std::is_same_v<int16_t, T>)
        return ImGuiDataType_S16;
    else if constexpr (std::is_same_v<int32_t, T>)
        return ImGuiDataType_S32;
    else if constexpr (std::is_same_v<int64_t, T>)
        return ImGuiDataType_S64;
    else if constexpr (std::is_same_v<uint8_t, T>)
        return ImGuiDataType_U8;
    else if constexpr (std::is_same_v<uint16_t, T>)
        return ImGuiDataType_U16;
    else if constexpr (std::is_same_v<uint32_t, T>)
        return ImGuiDataType_U32;
    else if constexpr (std::is_same_v<uint64_t, T>)
        return ImGuiDataType_U64;
}

template<typename T, typename U>
constexpr const T* to_ptr(T& mem, const U& val) noexcept
{
    if constexpr (std::is_same_v<U, T>)
        return &val;
    else
    {
        mem = static_cast<T>(val);
        return &mem;
    }
}

template<typename T>
bool DragNum(const char* label, T& p_data, float v_speed = 1.0f, const T* p_min = nullptr, const T* p_max = nullptr, const char* format = nullptr, ImGuiSliderFlags flags = ImGuiSliderFlags_None) noexcept
{
    return ImGui::DragScalar(label, map_imgui_datatype<T>(), &p_data, v_speed, p_min, p_max, format, flags);
}

template<typename T>
bool DragNum(const char* label, T& p_data, float v_speed, std::nullptr_t p_min, std::nullptr_t p_max, const char* format = nullptr, ImGuiSliderFlags flags = ImGuiSliderFlags_None) noexcept
{
    return DragNum(label, p_data, v_speed, static_cast<T*>(p_min), static_cast<T*>(p_max), format, flags);
}

template<typename T, typename U = T, typename V = T>
bool DragNum(const char* label, T& p_data, float v_speed, const U& p_min, const V& p_max = std::numeric_limits<T>::max(), const char* format = nullptr, ImGuiSliderFlags flags = ImGuiSliderFlags_None) noexcept
{
    T min, max;
    return DragNum(label, p_data, v_speed, to_ptr(min, p_min), to_ptr(max, p_max), format, flags);
}

template<typename T, typename U = T, typename V = T>
bool SliderNum(const char* label, T& p_data, const U& p_min, const V& p_max, const char* format = nullptr, ImGuiSliderFlags flags = ImGuiSliderFlags_None) noexcept
{
    T min, max;
    return ImGui::SliderScalar(label, map_imgui_datatype<T>(), &p_data, to_ptr(min, p_min), to_ptr(max, p_max), format, flags);
}

template<typename T, typename U = T, typename V = T>
bool VSliderNum(const char* label, const ImVec2& size, T& p_data, const U& p_min, const V& p_max, const char* format = nullptr, ImGuiSliderFlags flags = ImGuiSliderFlags_None) noexcept
{
    T min, max;
    return ImGui::VSliderScalar(label, size, map_imgui_datatype<T>(), &p_data, to_ptr(min, p_min), to_ptr(max, p_max), format, flags);
}

template<typename T>
bool InputNum(const char* label, T& p_data, const T* p_step = nullptr, const T* p_step_fast = nullptr, const char* format = nullptr, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None) noexcept
{
    return ImGui::InputScalar(label, map_imgui_datatype<T>(), &p_data, p_step, p_step_fast, format, flags);
}

template<typename T>
bool InputNum(const char* label, T& p_data, std::nullptr_t p_step, std::nullptr_t p_step_fast, const char* format = nullptr, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None) noexcept
{
    return InputNum(label, p_data, static_cast<T*>(p_step), static_cast<T*>(p_step_fast), format, flags);
}

template<typename T, typename U = T, typename V = T>
bool InputNum(const char* label, T& p_data, const U& p_step, const V& p_step_fast = std::numeric_limits<T>::max(), const char* format = nullptr, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None) noexcept
{
    T step, fast_step;
    return InputNum(label, p_data, to_ptr(step, p_step), to_ptr(fast_step, p_step_fast), format, flags);
}

} // namespace ImGui

#endif
