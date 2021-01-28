#include "guinea.hpp"

namespace ImGui
{
void (*_assert_handler)(void*, const char*) = nullptr;
void* _assert_handler_obj                   = nullptr;
} // namespace ImGui

namespace ui
{
guinea::~guinea() noexcept
{
}

void guinea::create_context() noexcept
{
    ui::CreateContext();
    ui::plot::CreateContext();
    ui::ne::SetCurrentEditor(ui::ne::CreateEditor());

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
}

void guinea::destroy_context() noexcept
{
    ui::ne::DestroyEditor(ui::ne::GetCurrentEditor());
    ui::plot::DestroyContext();
    ui::DestroyContext();
}

} // namespace ui
