#pragma once

#include "imgui.h"
#include "imgui_scoped.h"

namespace gn
{
    using namespace ImGui;

    class app
    {
    protected:
        virtual void on_setup(int, char**);
        virtual void on_load();
        virtual void on_update(bool&) = 0;
        virtual void on_unload();
        virtual int on_shutdown();
    public:
        int launch(int, char**);
        virtual ~app() = 0;
    };
}


#define CONCAT_(x, y) x##y
#define CONCAT(x, y) CONCAT_(x, y)
#define GN(x, ...) \
    ImScoped::x CONCAT(_gui_element, __LINE__) { __VA_ARGS__ }
