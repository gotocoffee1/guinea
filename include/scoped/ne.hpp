#pragma once
#ifndef _NE_SCOPED_HPP_
#define _NE_SCOPED_HPP_

#include "imgui_node_editor.h"

namespace scoped
{
namespace ui
{
namespace ne
{
#define DELETE_MOVE_COPY(Base)                                   \
    Base(Base&&)  = delete;               /* Move not allowed */ \
    Base& operator=(Base&&) = delete;     /* "" */               \
    Base(const Base&)       = delete;     /* Copy not allowed */ \
    Base& operator=(const Base&) = delete /* "" */

using namespace ax::NodeEditor;

struct Editor
{
    Editor(const char* id, const ImVec2& size = ImVec2(0, 0))
    {
        Begin(id, size);
    }

    ~Editor()
    {
        End();
    }

    DELETE_MOVE_COPY(Editor);
};

struct Node
{
    Node(NodeId id)
    {
        BeginNode(id);
    }

    ~Node()
    {
        EndNode();
    }
    DELETE_MOVE_COPY(Node);
};

struct Pin
{
    Pin(PinId id, PinKind kind)
    {
        BeginPin(id, kind);
    }

    ~Pin()
    {
        EndPin();
    }
    DELETE_MOVE_COPY(Pin);
};

struct GroupHint
{
    bool IsOpen;
    GroupHint(NodeId nodeId)
    {
        IsOpen = BeginGroupHint(nodeId);
    }

    ~GroupHint()
    {
        EndGroupHint();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(GroupHint);
};

struct Create
{
    bool IsOpen;

    Create(const ImVec4& color = ImVec4(1, 1, 1, 1), float thickness = 1.0f)
    {
        IsOpen = BeginCreate(color, thickness);
    }

    ~Create()
    {
        EndCreate();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(Create);
};

struct Delete
{
    bool IsOpen;

    Delete()
    {
        IsOpen = BeginDelete();
    }

    ~Delete()
    {
        EndDelete();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(Delete);
};

struct Shortcut
{
    bool IsOpen;

    Shortcut()
    {
        IsOpen = BeginShortcut();
    }

    ~Shortcut()
    {
        EndShortcut();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(Shortcut);
};

struct StyleColor
{
    StyleColor(ax::NodeEditor::StyleColor colorIndex, const ImVec4& color)
    {
        PushStyleColor(colorIndex, color);
    }

    ~StyleColor()
    {
        PopStyleColor();
    }
    DELETE_MOVE_COPY(StyleColor);
};

struct StyleVar
{
    StyleVar(ax::NodeEditor::StyleVar varIndex, float value)
    {
        PushStyleVar(varIndex, value);
    }

    StyleVar(ax::NodeEditor::StyleVar varIndex, const ImVec2& value)
    {
        PushStyleVar(varIndex, value);
    }

    StyleVar(ax::NodeEditor::StyleVar varIndex, const ImVec4& value)
    {
        PushStyleVar(varIndex, value);
    }

    ~StyleVar()
    {
        PopStyleVar();
    }
    DELETE_MOVE_COPY(StyleVar);
};

#undef DELETE_MOVE_COPY
} // namespace ne
} // namespace ui
} // namespace scoped

#endif
