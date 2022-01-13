#pragma once

#ifndef _GUINEA_HPP_
#define _GUINEA_HPP_

#include "imgui.h"
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4189)
#endif
#include "imgui_markdown.h"
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include "misc/cpp/imgui_stdlib.h"
#include "scoped/ne.hpp"
#include "scoped/plot.hpp"
#include "scoped/scoped.hpp"
#include "scoped/ui.hpp"
#include "widgets.hpp"

#ifndef UI_EXTERN_FILE
#define UI_EXTERN_FILE "EXTERN_FILE"
#endif

#ifndef UI_EXTERN_TEXT
#define UI_EXTERN_TEXT "EXTERN_TEXT"
#endif

namespace ui
{
class guinea
{
  protected:
    virtual const char* setup(int, char**) noexcept
    {
        return "";
    }
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
    virtual int shutdown() noexcept
    {
        return EXIT_SUCCESS;
    }
    virtual void failure(const char*) noexcept
    {
    }
    virtual ~guinea() noexcept = 0;

  public:
    int launch(int, char**) noexcept;

    int launch() noexcept
    {
        return launch(0, nullptr);
    }

  private:
#ifdef __EMSCRIPTEN__
    static void inner_loop(void*) noexcept;
    uint8_t frame_cnt;
    bool done;
#endif
    void create_context() noexcept;
    void destroy_context() noexcept;
};
} // namespace ui

namespace ui
{
using namespace ImGui;
namespace plot
{
using namespace ImPlot;
}
namespace ne
{
using namespace ax::NodeEditor;
}

} // namespace ui

namespace ui
{
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 6385)
#pragma warning(disable : 4189)
#endif
#include "imgui_memory_editor/imgui_memory_editor.h"
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
}// namespace ui
#endif
