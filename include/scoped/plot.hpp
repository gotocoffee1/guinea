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

    Plot(const char* title_id, const ImVec2& size = ImVec2(-1, 0), ImPlotFlags flags = ImPlotFlags_None)
    {
        IsOpen = ImPlot::BeginPlot(title_id,
                                   size,
                                   flags);
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

struct DragDropTargetPlot
{
    bool IsOpen;
    DragDropTargetPlot()
    {
        IsOpen = ImPlot::BeginDragDropTargetPlot();
    }

    ~DragDropTargetPlot()
    {
        if (IsOpen)
            ImPlot::EndDragDropTarget();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(DragDropTargetPlot);
};

struct DragDropTargetLegend
{
    bool IsOpen;
    DragDropTargetLegend()
    {
        IsOpen = ImPlot::BeginDragDropTargetLegend();
    }

    ~DragDropTargetLegend()
    {
        if (IsOpen)
            ImPlot::EndDragDropTarget();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(DragDropTargetLegend);
};

struct DragDropTargetAxis
{
    bool IsOpen;
    DragDropTargetAxis(ImAxis axis)
    {
        IsOpen = ImPlot::BeginDragDropTargetAxis(axis);
    }

    ~DragDropTargetAxis()
    {
        if (IsOpen)
            ImPlot::EndDragDropTarget();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(DragDropTargetAxis);
};

struct DragDropSourcePlot
{
    bool IsOpen;
    DragDropSourcePlot(ImGuiDragDropFlags flags = 0)
    {
        IsOpen = ImPlot::BeginDragDropSourcePlot(flags);
    }

    ~DragDropSourcePlot()
    {
        if (IsOpen)
            ImPlot::EndDragDropSource();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(DragDropSourcePlot);
};

struct DragDropSourceAxis
{
    bool IsOpen;
    DragDropSourceAxis(ImAxis axis, ImGuiDragDropFlags flags = 0)
    {
        IsOpen = ImPlot::BeginDragDropSourceAxis(axis, flags);
    }

    ~DragDropSourceAxis()
    {
        if (IsOpen)
            ImPlot::EndDragDropSource();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(DragDropSourceAxis);
};

struct DragDropSourceItem
{
    bool IsOpen;
    DragDropSourceItem(const char* label_id, ImGuiDragDropFlags flags = 0)
    {
        IsOpen = ImPlot::BeginDragDropSourceItem(label_id, flags);
    }

    ~DragDropSourceItem()
    {
        if (IsOpen)
            ImPlot::EndDragDropSource();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(DragDropSourceItem);
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

    Colormap(const char* name)
    {
        ImPlot::PushColormap(name);
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
