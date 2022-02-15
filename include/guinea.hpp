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

#ifdef BUILD_GUINEA_BACKEND_STATIC
#define EXPORT
#else
#if defined(_MSC_VER)
#define EXPORT __declspec(dllimport)
#else
#define EXPORT
#endif
#endif

#include "guinea_base.hpp"

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
} // namespace ui
#endif
