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
             float radius = 6,
             float thickness = 3);

bool LoadTextureFromFile(const char* filename,
                         void** out_texture,
                         int* out_width,
                         int* out_height);

void UnLoadTexture(void* out_texture);
} // namespace ui

#endif
