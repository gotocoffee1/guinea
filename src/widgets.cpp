#include "widgets.hpp"
#include "guinea_base.hpp"

#include "imgui_internal.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace ImGui
{
bool BufferingBar(const char* label,
                  float value,
                  const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g         = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id        = window->GetID(label);

    ImVec2 pos  = window->DC.CursorPos;
    ImVec2 size = size_arg;
    size.x -= style.FramePadding.x * 2;

    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    // Render
    const float circleStart = size.x * 0.7f;
    const float circleEnd   = size.x;
    const float circleWidth = circleEnd - circleStart;

    const ImU32 fg_col = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
    const ImU32 bg_col = ImGui::GetColorU32(ImGuiCol_Button);

    window->DrawList->AddRectFilled(
        bb.Min, ImVec2(pos.x + circleStart, bb.Max.y), bg_col);
    window->DrawList->AddRectFilled(
        bb.Min, ImVec2(pos.x + circleStart * value, bb.Max.y), fg_col);

    const float t     = (float)g.Time;
    const float r     = size.y / 2;
    const float speed = 1.5f;

    const float a = speed * 0;
    const float b = speed * 0.333f;
    const float c = speed * 0.666f;

    const float o1 =
        (circleWidth + r) * (t + a - speed * (int)((t + a) / speed)) / speed;
    const float o2 =
        (circleWidth + r) * (t + b - speed * (int)((t + b) / speed)) / speed;
    const float o3 =
        (circleWidth + r) * (t + c - speed * (int)((t + c) / speed)) / speed;

    window->DrawList->AddCircleFilled(
        ImVec2(pos.x + circleEnd - o1, bb.Min.y + r), r, bg_col);
    window->DrawList->AddCircleFilled(
        ImVec2(pos.x + circleEnd - o2, bb.Min.y + r), r, bg_col);
    window->DrawList->AddCircleFilled(
        ImVec2(pos.x + circleEnd - o3, bb.Min.y + r), r, bg_col);

    return true;
}

bool Spinner(const char* label, float radius, float thickness)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g         = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id        = window->GetID(label);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size((radius)*2, (radius + style.FramePadding.y) * 2);

    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    // Render
    window->DrawList->PathClear();
    const float t = (float)g.Time;

    int num_segments = 30;
    int start        = (int)ImFabs(ImSin(t * 1.8f) * (num_segments - 5));

    const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
    const float a_max =
        IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

    const ImVec2 centre =
        ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

    for (int i = 0; i < num_segments; i++)
    {
        const float a =
            a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        window->DrawList->PathLineTo(
            ImVec2(centre.x + ImCos(a + t * 8) * radius,
                   centre.y + ImSin(a + t * 8) * radius));
    }
    const ImU32& color = GetColorU32(ImGuiCol_ButtonHovered);
    window->DrawList->PathStroke(color, false, thickness);
    return true;
}

} // namespace ImGui

extern "C" ImTextureID load_texture(const unsigned char* image_data, int out_width, int out_height) noexcept;
extern "C" void unload_texture(ImTextureID out_texture) noexcept;

#ifndef BUILD_GUINEA_BACKEND_STATIC

struct ui::guinea::texture
{
    static ImTextureID load(const unsigned char* image_data, int width, int height) noexcept
    {
        if (auto* ctx = ui::ctx::get_current(); ctx->load_texture_ptr)
            return static_cast<decltype(&load_texture)>(ctx->load_texture_ptr)(image_data, width, height);
        return nullptr;
    }

    static void unload(ImTextureID texture) noexcept
    {
        if (auto* ctx = ui::ctx::get_current(); ctx->unload_texture_ptr)
            return static_cast<decltype(&unload_texture)>(ctx->unload_texture_ptr)(texture);
    }
};

#endif

namespace ImGui
{
img_data LoadImageFromFile(const char* filename,
                           int* out_width,
                           int* out_height) noexcept
{
    return stbi_load(filename, out_width, out_height, NULL, 4);
}

img_data LoadImageFromMemory(const void* buffer,
                             int size,
                             int* out_width,
                             int* out_height) noexcept
{
    return stbi_load_from_memory(static_cast<const stbi_uc*>(buffer), size, out_width, out_height, NULL, 4);
}

void UnLoadImage(img_data img) noexcept
{
    if (img == nullptr)
        return;
    stbi_image_free(img);
}

ImTextureID LoadTexture(const img_data image_data, int width, int height) noexcept
{
    if (image_data == NULL)
        return nullptr;
#ifndef BUILD_GUINEA_BACKEND_STATIC
    return ui::guinea::texture::load(image_data, width, height);
#else
    return load_texture(image_data, width, height);
#endif
}

void UnLoadTexture(ImTextureID texture) noexcept
{
    if (texture == nullptr)
        return;
#ifndef BUILD_GUINEA_BACKEND_STATIC
    return ui::guinea::texture::unload(texture);
#else
    unload_texture(texture);
#endif
}

} // namespace ImGui
