#include "guinea.hpp"

#include <iostream>

#ifndef BUILD_GUINEA_BACKEND_STATIC
#include "load_lib.hpp"
#endif

extern "C" void loop(ui::guinea& self, ImGuiContext* ctx) noexcept;

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

static void create_context(guinea* self) noexcept
{
    ctx::set_current(self);
    ui::CreateContext();
    ctx::set_current(self); // its a bit cheesy but ok, guinea context is not youes in hot loop

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

    ui::plot::CreateContext();
    ui::ne::SetCurrentEditor(ui::ne::CreateEditor());
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
}

static void destroy_context() noexcept
{
    ui::ne::DestroyEditor(ui::ne::GetCurrentEditor());
    ui::plot::DestroyContext();
    ui::DestroyContext();
    ctx::set_current(nullptr);
}

struct context
{
    context(guinea* self) noexcept
    {
        create_context(self);
    }
    ~context() noexcept
    {
        destroy_context();
    }
};

int guinea::launch(int argc, char** argv) noexcept
{
    if (auto backend = setup(argc, argv))
    {
#ifdef BUILD_GUINEA_BACKEND_STATIC
        context ctx{this};
        loop(*this, ImGui::GetCurrentContext());
#else
        if (auto l = lib::open(backend))
        {
            if (auto loop_ptr = reinterpret_cast<decltype(&loop)>(lib::get_symbol(l, "loop")))
            {
                *reinterpret_cast<void**>(&load_texture_ptr)   = lib::get_symbol(l, "load_texture");
                *reinterpret_cast<void**>(&unload_texture_ptr) = lib::get_symbol(l, "unload_texture");

                context ctx{this};
                loop_ptr(*this, ImGui::GetCurrentContext());
            }
            else
                failure(lib::get_error_message());

            load_texture_ptr   = nullptr;
            unload_texture_ptr = nullptr;
            lib::close(l);
        }
        else
            failure(lib::get_error_message());
#endif
    }

    return shutdown();
}

} // namespace ui
