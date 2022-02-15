#pragma once

#include "imgui.h"

#ifndef UI_EXTERN_FILE
#define UI_EXTERN_FILE "EXTERN_FILE"
#endif

#ifndef UI_EXTERN_TEXT
#define UI_EXTERN_TEXT "EXTERN_TEXT"
#endif

namespace ui
{
class guinea;
}

extern "C" EXPORT void loop(ui::guinea& self, ImGuiContext* ctx) noexcept;

namespace ui
{
class guinea
{
  public:
    const char* title   = "";
    ImVec4 clear_color  = ImColor(62, 62, 66);
    int fps             = 30;
    ImVec2 resolution   = {1280, 720};

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

    friend void ::loop(guinea& self, ImGuiContext* ctx) noexcept;

  public:
    int launch(int, char**) noexcept;

    int launch() noexcept
    {
        return launch(0, nullptr);
    }

  private:
#ifdef __EMSCRIPTEN__
    static void inner_loop(void*) noexcept;
    int  frame_cnt;
    bool done;
#endif
};
} // namespace ui
