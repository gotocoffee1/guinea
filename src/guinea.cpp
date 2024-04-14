#include "guinea.hpp"

#include <iostream>

#ifdef BUILD_GUINEA_BACKEND_STATIC
extern "C" void guinea_loop(ui::guinea& self, ImGuiContext* ctx) noexcept;
#endif

namespace ui
{

static void create_context(guinea* self) noexcept
{
    ctx::set_current(self);
    ui::SetCurrentContext(ui::CreateContext());
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

    ui::plot::SetCurrentContext(ui::plot::CreateContext());
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

guinea::guinea(init_data d) noexcept
    : init{std::move(d)}
{
    create_context(this);

    imgui_ctx = ui::GetCurrentContext();
    plot_ctx  = ui::plot::GetCurrentContext();
    ne_ctx    = ui::ne::GetCurrentEditor();

#ifdef BUILD_GUINEA_BACKEND_STATIC
    impl::init(*this, ImGui::GetCurrentContext());
#else
    lib.reset(lib::open(init.backend.empty() ? DEFAULT_GUINEA_BACKEND : init.backend));
    failure(lib::get_error_message());
    if (auto ptr = reinterpret_cast<const guinea_func*>(lib::get_symbol(lib.get(), "guinea")); ptr)
        funcs = *ptr;
    else
        failure(lib::get_error_message());
    funcs.init_ptr(*this, ImGui::GetCurrentContext());
#endif
}

guinea::~guinea() noexcept
{
    ui::SetCurrentContext(static_cast<ImGuiContext*>(imgui_ctx));
    ui::plot::SetCurrentContext(static_cast<ImPlotContext*>(plot_ctx));
    ui::ne::SetCurrentEditor(static_cast<ui::ne::EditorContext*>(ne_ctx));

#ifdef BUILD_GUINEA_BACKEND_STATIC
    impl::shutdown(*this, ImGui::GetCurrentContext());
#else
    funcs.shutdown_ptr(*this, ImGui::GetCurrentContext());
#endif

    destroy_context();
}

void guinea::failure(const char* msg) noexcept
{
    if (msg)
        std::cerr << msg << "\n";
}

bool guinea::loop() noexcept
{
    ui::SetCurrentContext(static_cast<ImGuiContext*>(imgui_ctx));
    ui::plot::SetCurrentContext(static_cast<ImPlotContext*>(plot_ctx));
    ui::ne::SetCurrentEditor(static_cast<ui::ne::EditorContext*>(ne_ctx));

#ifdef BUILD_GUINEA_BACKEND_STATIC
    return impl::loop(*this, ImGui::GetCurrentContext());
#else
    return funcs.loop_ptr(*this, ImGui::GetCurrentContext());
#endif
}

} // namespace ui
