#include "guinea.hpp"

#include <iostream>

#ifndef BUILD_GUINEA_BACKEND_STATIC
#include "load_lib.hpp"
#endif

namespace ImGui
{
void (*_assert_handler)(void*, const char*) = nullptr;
void* _assert_handler_obj                   = nullptr;
} // namespace ImGui

namespace ui
{
guinea::~guinea() noexcept = default;

const char* guinea::setup(int argc, char** argv) noexcept
{
    if (argc > 1)
        return argv[1];
    return DEFAULT_GUINEA_BACKEND;
}

void guinea::failure(const char* msg) noexcept
{
    if (msg)
        std::cerr << msg << "\n";
}

int guinea::shutdown() noexcept
{
    return EXIT_SUCCESS;
}

static void create_context() noexcept
{
    ui::CreateContext();
    ui::plot::CreateContext();
    ui::ne::SetCurrentEditor(ui::ne::CreateEditor());

    ImGuiStyle& style                 = ImGui::GetStyle();
    style.WindowRounding              = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
}

static void destroy_context() noexcept
{
    ui::ne::DestroyEditor(ui::ne::GetCurrentEditor());
    ui::plot::DestroyContext();
    ui::DestroyContext();
}

struct context
{
    context() noexcept
    {
        create_context();
    }
    ~context() noexcept
    {
        destroy_context();
    }
};

using loop_func = decltype(&loop);

int guinea::launch(int argc, char** argv) noexcept
{
    if (auto backend = setup(argc, argv))
    {
#ifdef BUILD_GUINEA_BACKEND_STATIC
        context ctx;
        loop(*this, ImGui::GetCurrentContext());
#else
        if (auto l = lib::open(backend))
        {
            if (auto loop_ptr = reinterpret_cast<loop_func>(lib::get_symbol(l, "loop")))
            {
                context ctx;
                loop_ptr(*this, ImGui::GetCurrentContext());
            }
            else
                failure(lib::get_error_message());
            lib::close(l);
        }
        else
            failure(lib::get_error_message());
#endif
    }

    return shutdown();
}

} // namespace ui
