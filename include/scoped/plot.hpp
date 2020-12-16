#pragma once
#ifndef _PLOT_SCOPED_HPP_
#define _PLOT_SCOPED_HPP_

#include "implot.h"

namespace scoped
{
namespace ui
{
namespace plot
{
#define DELETE_MOVE_COPY(Base)                                   \
    Base(Base&&)  = delete;               /* Move not allowed */ \
    Base& operator=(Base&&) = delete;     /* "" */               \
    Base(const Base&)       = delete;     /* Copy not allowed */ \
    Base& operator=(const Base&) = delete /* "" */

struct Plot
{
    bool IsOpen;

    Plot(const char* title_id,
         const char* x_label      = NULL,
         const char* y_label      = NULL,
         const ImVec2& size       = ImVec2(-1, 0),
         ImPlotFlags flags        = ImPlotFlags_None,
         ImPlotAxisFlags x_flags  = ImPlotAxisFlags_None,
         ImPlotAxisFlags y_flags  = ImPlotAxisFlags_None,
         ImPlotAxisFlags y2_flags = ImPlotAxisFlags_NoGridLines,
         ImPlotAxisFlags y3_flags = ImPlotAxisFlags_NoGridLines)
    {
        IsOpen = ImPlot::BeginPlot(title_id,
                                   x_label,
                                   y_label,
                                   size,
                                   flags,
                                   x_flags,
                                   y_flags,
                                   y2_flags,
                                   y3_flags);
    }
    ~Plot()
    {
        if (IsOpen)
            ImPlot::EndPlot();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(Plot);
};

struct LegendDragDropSource
{
    bool IsOpen;
    LegendDragDropSource(const char* label_id, ImGuiDragDropFlags flags = 0)
    {
        IsOpen = ImPlot::BeginLegendDragDropSource(label_id, flags);
    }

    ~LegendDragDropSource()
    {
        if (IsOpen)
            ImPlot::EndLegendDragDropSource();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(LegendDragDropSource);
};

struct LegendPopup
{
    bool IsOpen;
    LegendPopup(const char* label_id, ImGuiMouseButton mouse_button = 1)
    {
        IsOpen = ImPlot::BeginLegendPopup(label_id, mouse_button);
    }

    ~LegendPopup()
    {
        if (IsOpen)
            ImPlot::EndLegendPopup();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(LegendPopup);
};

struct StyleVar
{
    StyleVar(ImPlotStyleVar idx, float val)
    {
        ImPlot::PushStyleVar(idx, val);
    }
    StyleVar(ImPlotStyleVar idx, int val)
    {
        ImPlot::PushStyleVar(idx, val);
    }
    StyleVar(ImPlotStyleVar idx, const ImVec2& val)
    {
        ImPlot::PushStyleVar(idx, val);
    }
    ~StyleVar()
    {
        ImPlot::PopStyleVar();
    }

    DELETE_MOVE_COPY(StyleVar);
};

struct StyleColor
{
    StyleColor(ImPlotCol idx, ImU32 col)
    {
        ImPlot::PushStyleColor(idx, col);
    }
    StyleColor(ImPlotCol idx, const ImVec4& col)
    {
        ImPlot::PushStyleColor(idx, col);
    }
    ~StyleColor()
    {
        ImPlot::PopStyleColor();
    }

    DELETE_MOVE_COPY(StyleColor);
};

struct PlotClipRect
{
    PlotClipRect()
    {
        ImPlot::PushPlotClipRect();
    }

    ~PlotClipRect()
    {
        ImPlot::PopPlotClipRect();
    }
    DELETE_MOVE_COPY(PlotClipRect);
};

struct Colormap
{
    Colormap(ImPlotColormap colormap)
    {
        ImPlot::PushColormap(colormap);
    }

    Colormap(const ImVec4* colormap, int size)
    {
        ImPlot::PushColormap(colormap, size);
    }

    ~Colormap()
    {
        ImPlot::PopColormap();
    }
    DELETE_MOVE_COPY(Colormap);
};

#undef DELETE_MOVE_COPY
} // namespace plot
} // namespace ui
} // namespace scoped

#endif
