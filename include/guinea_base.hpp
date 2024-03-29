#pragma once

#include "imgui.h"

#ifndef UI_EXTERN_FILE
#define UI_EXTERN_FILE "EXTERN_FILE"
#endif

#ifndef UI_EXTERN_TEXT
#define UI_EXTERN_TEXT "EXTERN_TEXT"
#endif

#include <string>

namespace ui
{
class guinea
{
  public:
    std::string title  = "";
    ImVec4 clear_color = ImColor(62, 62, 66);
#ifdef __EMSCRIPTEN__
    int fps = 0;
#else
    int fps = 30;
#endif
    ImVec2 resolution = {1280, 720};

    guinea()                         = default;
    guinea(guinea&&)                 = delete;
    guinea& operator=(guinea&&)      = delete;
    guinea(const guinea&)            = delete;
    guinea& operator=(const guinea&) = delete;

  protected:
    virtual const char* setup(int, char**) noexcept;

    virtual void load(void) noexcept
    {
    }
    virtual void render() noexcept
    {
    }
    virtual bool update(bool&) noexcept
    {
        return false;
    }
    virtual void unload(void) noexcept
    {
    }
    virtual int shutdown() noexcept;

    virtual void failure(const char* msg) noexcept;

    virtual ~guinea() noexcept = 0;

  public:
    int launch(int, char**) noexcept;

    int launch() noexcept
    {
        return launch(0, nullptr);
    }

    struct impl;
    struct texture;
#ifdef USE_GUINEA_ASSERT
    friend void ImGui::guinea_assert(const char* msg);
#endif
#ifndef BUILD_GUINEA_BACKEND_STATIC
    using load_texture_t   = ImTextureID (*)(const unsigned char* image_data, int out_width, int out_height) noexcept;
    using unload_texture_t = void (*)(ImTextureID out_texture) noexcept;

  private:
    load_texture_t load_texture_ptr     = nullptr;
    unload_texture_t unload_texture_ptr = nullptr;
#endif

  private:
#ifdef __EMSCRIPTEN__
    int frame_cnt;
    bool done;
#endif
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
