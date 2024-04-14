#pragma once

#include "imgui.h"

#ifndef UI_EXTERN_FILE
#define UI_EXTERN_FILE "EXTERN_FILE"
#endif

#ifndef UI_EXTERN_TEXT
#define UI_EXTERN_TEXT "EXTERN_TEXT"
#endif

#include <memory>
#include <string>

#ifndef BUILD_GUINEA_BACKEND_STATIC
#include "load_lib.hpp"
#endif

namespace ui
{
#ifndef BUILD_GUINEA_BACKEND_STATIC

class guinea;

struct guinea_func
{
    using init_t           = void (*)(guinea& self, ImGuiContext* ctx) noexcept;
    using loop_t           = bool (*)(guinea& self, ImGuiContext* ctx) noexcept;
    using shutdown_t       = void (*)(guinea& self, ImGuiContext* ctx) noexcept;
    using load_texture_t   = ImTextureID (*)(guinea& self, const unsigned char* image_data, int out_width, int out_height) noexcept;
    using unload_texture_t = void (*)(guinea& self, ImTextureID out_texture) noexcept;

    init_t init_ptr                     = nullptr;
    loop_t loop_ptr                     = nullptr;
    shutdown_t shutdown_ptr             = nullptr;
    load_texture_t load_texture_ptr     = nullptr;
    unload_texture_t unload_texture_ptr = nullptr;
};
#endif

struct init_data
{
    std::string title = "";
    ImVec2 resolution = {1280, 720};
    bool vsync        = false;
    std::string backend;
};

class guinea
{
  public:
    ImVec4 clear_color = ImColor(62, 62, 66);

    guinea(init_data init = init_data{}) noexcept;
    guinea(guinea&&)                 = delete;
    guinea& operator=(guinea&&)      = delete;
    guinea(const guinea&)            = delete;
    guinea& operator=(const guinea&) = delete;

  protected:
    virtual void render() noexcept
    {
    }

    virtual void failure(const char* msg) noexcept;

    virtual ~guinea() noexcept = 0;

  public:
    bool loop() noexcept;

    struct impl
    {
        static void init(ui::guinea& self, ImGuiContext* ctx) noexcept;
        static bool loop(ui::guinea& self, ImGuiContext* ctx) noexcept;
        static void shutdown(ui::guinea& self, ImGuiContext* ctx) noexcept;

        static ImTextureID load_texture(ui::guinea& self, const unsigned char* image_data, int image_width, int image_height) noexcept;
        static void unload_texture(ui::guinea&, ImTextureID texture) noexcept;
    };

    struct texture;
#ifdef USE_GUINEA_ASSERT
    friend void ImGui::guinea_assert(const char* msg);
#endif
  private:
#ifndef BUILD_GUINEA_BACKEND_STATIC
    std::unique_ptr<std::remove_pointer_t<lib::handle>, lib::closer> lib;
    guinea_func funcs;
#endif

    void* platform_data = nullptr;
    void* renderer_data = nullptr;
    init_data init;
    void* imgui_ctx;
    void* plot_ctx;
    void* ne_ctx;
};

} // namespace ui

namespace ui
{

struct ctx
{
    static inline guinea* _guinea_context = nullptr;

    static guinea* get_current()
    {
        if (!_guinea_context && ImGui::GetCurrentContext())
            _guinea_context = static_cast<guinea*>(ImGui::GetIO().UserData);
        return _guinea_context;
    }
    static void set_current(guinea* ctx)
    {
        if (ImGui::GetCurrentContext())
            ImGui::GetIO().UserData = ctx;
        _guinea_context = ctx;
    }
};

} // namespace ui
