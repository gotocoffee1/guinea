#include "widgets.hpp"
#include "guinea_base.hpp"

#include "imgui_internal.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

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
    ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

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

struct ui::guinea::texture
{
    static ImTextureID load(const unsigned char* image_data, int width, int height) noexcept
    {
#ifdef BUILD_GUINEA_BACKEND_STATIC
        auto* ctx = ui::ctx::get_current();
        return ui::guinea::impl::load_texture(*ctx, image_data, width, height);
#else
        if (auto* ctx = ui::ctx::get_current(); ctx->funcs.load_texture_ptr)
            return ctx->funcs.load_texture_ptr(*ctx, image_data, width, height);
        return ImTextureID_Invalid;
#endif
    }

    static void unload(ImTextureID texture) noexcept
    {
#ifdef BUILD_GUINEA_BACKEND_STATIC
        auto* ctx = ui::ctx::get_current();
        return ui::guinea::impl::unload_texture(*ctx, texture);
#else
        if (auto* ctx = ui::ctx::get_current(); ctx->funcs.unload_texture_ptr)
            return ctx->funcs.unload_texture_ptr(*ctx, texture);
#endif
    }
};

namespace ImGui
{
img_data LoadImageFromFile(const char* filename,
                           int* out_width,
                           int* out_height) noexcept
{
    return stbi_load(filename, out_width, out_height, NULL, Img::comp);
}

img_data LoadImageFromMemory(const void* buffer,
                             int size,
                             int* out_width,
                             int* out_height) noexcept
{
    return stbi_load_from_memory(static_cast<const stbi_uc*>(buffer), size, out_width, out_height, NULL, Img::comp);
}

img_data CopyImage(const_img_data img,
                   int out_width,
                   int out_height) noexcept
{
    if (!img)
        return nullptr;
    auto size = sizeof(*img) * out_width * out_height * Img::comp;
    auto* p   = static_cast<stbi_uc*>(STBI_MALLOC(size));
    if (p)
        memcpy(p, img, size);
    return p;
}

img_data ResizeImage(const_img_data img,
                     int input_w,
                     int input_h,
                     int output_w,
                     int output_h) noexcept
{
    auto size = sizeof(*img) * output_w * output_h * Img::comp;
    auto* p   = static_cast<stbi_uc*>(STBI_MALLOC(size));
    if (p)
        if (stbir_resize_uint8(img, input_w, input_h, 0, p, output_w, output_h, 0, Img::comp))
            return p;
        else
            STBI_FREE(p);

    return nullptr;
}

void UnLoadImage(img_data img) noexcept
{
    if (img == nullptr)
        return;
    stbi_image_free(img);
}

ImTextureID LoadTexture(const_img_data image_data, int width, int height) noexcept
{
    if (image_data == NULL)
        return ImTextureID_Invalid;
    return ui::guinea::texture::load(image_data, width, height);
}

void UnLoadTexture(ImTextureID texture) noexcept
{
    if (texture == ImTextureID_Invalid)
        return;
    return ui::guinea::texture::unload(texture);
}

void Image(const Texture& tex, const ImVec2& uv0, const ImVec2& uv1)
{
    return Image(tex.ref(), tex.size(), uv0, uv1);
}

void ImageWithBg(const Texture& tex, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& bg_col, const ImVec4& tint_col)
{
    return ImageWithBg(tex.ref(), tex.size(), uv0, uv1, bg_col, tint_col);
}

bool ImageButton(const char* str_id, const Texture& tex, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& bg_col, const ImVec4& tint_col)
{
    return ImageButton(str_id, tex.ref(), tex.size(), uv0, uv1, bg_col, tint_col);
}

} // namespace ImGui
