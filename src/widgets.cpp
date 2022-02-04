#include "widgets.hpp"

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
#include <SDL_opengl.h>

namespace ImGui
{

static void* load_tex(stbi_uc* image_data,
                           int out_width,
                           int out_height)
{
    if (image_data == NULL)
        return nullptr;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 out_width,
                 out_height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 image_data);
    stbi_image_free(image_data);

    return reinterpret_cast<void*>(static_cast<uintptr_t>(image_texture));
}

bool LoadTextureFromFile(const char* filename,
                         void** out_texture,
                         int* out_width,
                         int* out_height)
{
    auto* data = stbi_load(filename, out_width, out_height, NULL, 4);
    *out_texture = 
        load_tex(data, *out_width, *out_height);
    return out_texture != nullptr;
}

bool LoadTextureFromMemory(const void* buffer,
                           int size,
                           void** out_texture,
                           int* out_width,
                           int* out_height)
{
    auto data = stbi_load_from_memory(static_cast<const stbi_uc*>(buffer), size, out_width, out_height, NULL, 4);
    *out_texture = 
        load_tex(data, *out_width, *out_height);
    return out_texture != nullptr;
}

void UnLoadTexture(void* out_texture)
{
    if (out_texture == nullptr)
        return;
    glBindTexture(GL_TEXTURE_2D, 0);
    auto image_texture = static_cast<GLuint>(reinterpret_cast<uintptr_t>(out_texture));
    glDeleteTextures(1, &image_texture);
}

} // namespace ImGui
