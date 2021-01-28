#pragma once

#ifndef _WIDGET_HPP_
#define _WIDGET_HPP_

#include "imgui.h"
#include <optional>
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

bool LoadTextureFromFile(const char* filename,
                         void** out_texture,
                         int* out_width,
                         int* out_height);

void UnLoadTexture(void* out_texture);

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
bool DragNum(const char* label, T& p_data, float v_speed = 1.0f, const T* p_min = nullptr, const T* p_max = nullptr, const char* format = nullptr, ImGuiSliderFlags flags = 0) noexcept
{
    return ImGui::DragScalar(label, map_imgui_datatype<T>(), &p_data, v_speed, p_min, p_max, format, flags);
}

template<typename T, typename U = T, typename V = T>
bool DragNum(const char* label, T& p_data, float v_speed, const U& p_min, const V& p_max = std::numeric_limits<T>::max(), const char* format = nullptr, ImGuiSliderFlags flags = 0) noexcept
{
    T min, max;
    return DragNum(label, p_data, v_speed, to_ptr(min, p_min), to_ptr(max, p_max), format, flags);
}

template<typename T, typename U = T, typename V = T>
bool SliderNum(const char* label, T& p_data, const U& p_min, const V& p_max, const char* format = nullptr, ImGuiSliderFlags flags = 0) noexcept
{
    T min, max;
    return ImGui::SliderScalar(label, map_imgui_datatype<T>(), &p_data, to_ptr(min, p_min), to_ptr(max, p_max), format, flags);
}

template<typename T, typename U = T, typename V = T>
bool VSliderNum(const char* label, const ImVec2& size, T& p_data, const U& p_min, const V& p_max, const char* format = nullptr, ImGuiSliderFlags flags = 0) noexcept
{
    T min, max;
    return ImGui::VSliderScalar(label, size, map_imgui_datatype<T>(), &p_data, to_ptr(min, p_min), to_ptr(max, p_max), format, flags);
}

template<typename T>
bool InputNum(const char* label, T& p_data, const T* p_step = nullptr, const T* p_step_fast = nullptr, const char* format = nullptr, ImGuiSliderFlags flags = 0) noexcept
{
    return ImGui::InputScalar(label, map_imgui_datatype<T>(), &p_data, p_step, p_step_fast, format, flags);
}

template<typename T, typename U = T, typename V = T>
bool InputNum(const char* label, T& p_data, const U& p_step, const V& p_step_fast = std::numeric_limits<T>::max(), const char* format = nullptr, ImGuiSliderFlags flags = 0) noexcept
{
    T step, fast_step;
    return InputNum(label, p_data, to_ptr(step, p_step_fast), to_ptr(fast_step, p_step_fast), format, flags);
}

} // namespace ImGui

#endif
