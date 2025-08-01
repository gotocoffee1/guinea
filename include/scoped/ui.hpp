#pragma once
#ifndef _UI_SCOPED_HPP_
#define _UI_SCOPED_HPP_

#include "imgui.h"

namespace scoped
{
namespace ui
{
#define DELETE_MOVE_COPY(Base)                                    \
    Base(Base&&)                 = delete; /* Move not allowed */ \
    Base& operator=(Base&&)      = delete; /* "" */               \
    Base(const Base&)            = delete; /* Copy not allowed */ \
    Base& operator=(const Base&) = delete  /* "" */

struct Window
{
    bool IsContentVisible;

    Window(const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0)
    {
        IsContentVisible = ImGui::Begin(name, p_open, flags);
    }
    ~Window()
    {
        ImGui::End();
    }

    explicit operator bool() const
    {
        return IsContentVisible;
    }

    DELETE_MOVE_COPY(Window);
};

struct Child
{
    bool IsContentVisible;

    Child(const char* str_id, const ImVec2& size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags flags = 0)
    {
        IsContentVisible = ImGui::BeginChild(str_id, size, border, flags);
    }
    Child(ImGuiID id, const ImVec2& size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags flags = 0)
    {
        IsContentVisible = ImGui::BeginChild(id, size, border, flags);
    }
    ~Child()
    {
        ImGui::EndChild();
    }

    explicit operator bool() const
    {
        return IsContentVisible;
    }

    DELETE_MOVE_COPY(Child);
};

struct Font
{
    Font(ImFont* font, float size)
    {
        ImGui::PushFont(font, size);
    }
    ~Font()
    {
        ImGui::PopFont();
    }

    DELETE_MOVE_COPY(Font);
};

struct StyleColor
{
    StyleColor(ImGuiCol idx, ImU32 col)
    {
        ImGui::PushStyleColor(idx, col);
    }
    StyleColor(ImGuiCol idx, const ImVec4& col)
    {
        ImGui::PushStyleColor(idx, col);
    }
    ~StyleColor()
    {
        ImGui::PopStyleColor();
    }

    DELETE_MOVE_COPY(StyleColor);
};

struct StyleVar
{
    StyleVar(ImGuiStyleVar idx, float val)
    {
        ImGui::PushStyleVar(idx, val);
    }
    StyleVar(ImGuiStyleVar idx, const ImVec2& val)
    {
        ImGui::PushStyleVar(idx, val);
    }
    ~StyleVar()
    {
        ImGui::PopStyleVar();
    }

    DELETE_MOVE_COPY(StyleVar);
};

struct ItemWidth
{
    ItemWidth(float item_width)
    {
        ImGui::PushItemWidth(item_width);
    }
    ~ItemWidth()
    {
        ImGui::PopItemWidth();
    }

    DELETE_MOVE_COPY(ItemWidth);
};

struct TextWrapPos
{
    TextWrapPos(float wrap_pos_x = 0.0f)
    {
        ImGui::PushTextWrapPos(wrap_pos_x);
    }
    ~TextWrapPos()
    {
        ImGui::PopTextWrapPos();
    }

    DELETE_MOVE_COPY(TextWrapPos);
};

struct Group
{
    Group()
    {
        ImGui::BeginGroup();
    }
    ~Group()
    {
        ImGui::EndGroup();
    }

    DELETE_MOVE_COPY(Group);
};

struct ID
{
    ID(const char* str_id)
    {
        ImGui::PushID(str_id);
    }
    ID(const char* str_id_begin, const char* str_id_end)
    {
        ImGui::PushID(str_id_begin, str_id_end);
    }
    ID(const void* ptr_id)
    {
        ImGui::PushID(ptr_id);
    }
    ID(int int_id)
    {
        ImGui::PushID(int_id);
    }
    ~ID()
    {
        ImGui::PopID();
    }

    DELETE_MOVE_COPY(ID);
};

struct Combo
{
    bool IsOpen;

    Combo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0)
    {
        IsOpen = ImGui::BeginCombo(label, preview_value, flags);
    }
    ~Combo()
    {
        if (IsOpen)
            ImGui::EndCombo();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(Combo);
};

struct ListBox
{
    bool IsOpen;

    ListBox(const char* label, const ImVec2& size = ImVec2(0, 0))
    {
        IsOpen = ImGui::BeginListBox(label, size);
    }
    ~ListBox()
    {
        if (IsOpen)
            ImGui::EndListBox();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(ListBox);
};

struct Table
{
    bool IsOpen;

    Table(const char* str_id, int column, ImGuiTableFlags flags = 0, const ImVec2& outer_size = ImVec2(0.0f, 0.0f), float inner_width = 0.0f)
    {
        IsOpen = ImGui::BeginTable(str_id, column, flags, outer_size, inner_width);
    }
    ~Table()
    {
        if (IsOpen)
            ImGui::EndTable();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(Table);
};

struct TabBar
{
    bool IsOpen;

    TabBar(const char* str_id, ImGuiTabBarFlags flags = 0)
    {
        IsOpen = ImGui::BeginTabBar(str_id, flags);
    }
    ~TabBar()
    {
        if (IsOpen)
            ImGui::EndTabBar();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(TabBar);
};

struct TabItem
{
    bool IsOpen;

    TabItem(const char* label, bool* p_open = NULL, ImGuiTabItemFlags flags = 0)
    {
        IsOpen = ImGui::BeginTabItem(label, p_open, flags);
    }
    ~TabItem()
    {
        if (IsOpen)
            ImGui::EndTabItem();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(TabItem);
};

struct TreeNode
{
    bool IsOpen;

    TreeNode(const char* label)
    {
        IsOpen = ImGui::TreeNode(label);
    }
    TreeNode(const char* str_id, const char* fmt, ...) IM_FMTARGS(3)
    {
        va_list ap;
        va_start(ap, fmt);
        IsOpen = ImGui::TreeNodeV(str_id, fmt, ap);
        va_end(ap);
    }
    TreeNode(const void* ptr_id, const char* fmt, ...) IM_FMTARGS(3)
    {
        va_list ap;
        va_start(ap, fmt);
        IsOpen = ImGui::TreeNodeV(ptr_id, fmt, ap);
        va_end(ap);
    }
    ~TreeNode()
    {
        if (IsOpen)
            ImGui::TreePop();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(TreeNode);
};

struct TreeNodeV
{
    bool IsOpen;

    TreeNodeV(const char* str_id, const char* fmt, va_list args) IM_FMTLIST(3)
    {
        IsOpen = ImGui::TreeNodeV(str_id, fmt, args);
    }
    TreeNodeV(const void* ptr_id, const char* fmt, va_list args) IM_FMTLIST(3)
    {
        IsOpen = ImGui::TreeNodeV(ptr_id, fmt, args);
    }
    ~TreeNodeV()
    {
        if (IsOpen)
            ImGui::TreePop();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(TreeNodeV);
};

struct TreeNodeEx
{
    bool IsOpen;

    TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags = 0)
    {
        IM_ASSERT(!(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen));
        IsOpen = ImGui::TreeNodeEx(label, flags);
    }
    TreeNodeEx(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) IM_FMTARGS(4)
    {
        va_list ap;
        va_start(ap, fmt);
        IM_ASSERT(!(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen));
        IsOpen = ImGui::TreeNodeExV(str_id, flags, fmt, ap);
        va_end(ap);
    }
    TreeNodeEx(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) IM_FMTARGS(4)
    {
        va_list ap;
        va_start(ap, fmt);
        IM_ASSERT(!(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen));
        IsOpen = ImGui::TreeNodeExV(ptr_id, flags, fmt, ap);
        va_end(ap);
    }
    ~TreeNodeEx()
    {
        if (IsOpen)
            ImGui::TreePop();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(TreeNodeEx);
};

struct TreeNodeExV
{
    bool IsOpen;

    TreeNodeExV(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args) IM_FMTLIST(4)
    {
        IM_ASSERT(!(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen));
        IsOpen = ImGui::TreeNodeExV(str_id, flags, fmt, args);
    }
    TreeNodeExV(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args) IM_FMTLIST(4)
    {
        IM_ASSERT(!(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen));
        IsOpen = ImGui::TreeNodeExV(ptr_id, flags, fmt, args);
    }
    ~TreeNodeExV()
    {
        if (IsOpen)
            ImGui::TreePop();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(TreeNodeExV);
};

struct MainMenuBar
{
    bool IsOpen;

    MainMenuBar()
    {
        IsOpen = ImGui::BeginMainMenuBar();
    }
    ~MainMenuBar()
    {
        if (IsOpen)
            ImGui::EndMainMenuBar();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(MainMenuBar);
};

struct MenuBar
{
    bool IsOpen;

    MenuBar()
    {
        IsOpen = ImGui::BeginMenuBar();
    }
    ~MenuBar()
    {
        if (IsOpen)
            ImGui::EndMenuBar();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(MenuBar);
};

struct Menu
{
    bool IsOpen;

    Menu(const char* label, bool enabled = true)
    {
        IsOpen = ImGui::BeginMenu(label, enabled);
    }
    ~Menu()
    {
        if (IsOpen)
            ImGui::EndMenu();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(Menu);
};

struct Tooltip
{
    bool IsOpen;

    Tooltip()
    {
        IsOpen = ImGui::BeginTooltip();
    }
    ~Tooltip()
    {
        if (IsOpen)
            ImGui::EndTooltip();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(Tooltip);
};

struct Popup
{
    bool IsOpen;

    Popup(const char* str_id, ImGuiWindowFlags flags = 0)
    {
        IsOpen = ImGui::BeginPopup(str_id, flags);
    }
    ~Popup()
    {
        if (IsOpen)
            ImGui::EndPopup();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(Popup);
};

struct PopupContextItem
{
    bool IsOpen;

    PopupContextItem(const char* str_id = NULL, int mouse_button = 1)
    {
        IsOpen = ImGui::BeginPopupContextItem(str_id, mouse_button);
    }
    ~PopupContextItem()
    {
        if (IsOpen)
            ImGui::EndPopup();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(PopupContextItem);
};

struct PopupContextWindow
{
    bool IsOpen;

    PopupContextWindow(const char* str_id = NULL, ImGuiPopupFlags flags = 1)
    {
        IsOpen = ImGui::BeginPopupContextWindow(str_id, flags);
    }
    ~PopupContextWindow()
    {
        if (IsOpen)
            ImGui::EndPopup();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(PopupContextWindow);
};

struct PopupContextVoid
{
    bool IsOpen;

    PopupContextVoid(const char* str_id = NULL, int mouse_button = 1)
    {
        IsOpen = ImGui::BeginPopupContextVoid(str_id, mouse_button);
    }
    ~PopupContextVoid()
    {
        if (IsOpen)
            ImGui::EndPopup();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(PopupContextVoid);
};

struct PopupModal
{
    bool IsOpen;

    PopupModal(const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0)
    {
        IsOpen = ImGui::BeginPopupModal(name, p_open, flags);
    }
    ~PopupModal()
    {
        if (IsOpen)
            ImGui::EndPopup();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(PopupModal);
};

struct DragDropSource
{
    bool IsOpen;

    DragDropSource(ImGuiDragDropFlags flags = 0)
    {
        IsOpen = ImGui::BeginDragDropSource(flags);
    }
    ~DragDropSource()
    {
        if (IsOpen)
            ImGui::EndDragDropSource();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(DragDropSource);
};

struct DragDropTarget
{
    bool IsOpen;

    DragDropTarget()
    {
        IsOpen = ImGui::BeginDragDropTarget();
    }
    ~DragDropTarget()
    {
        if (IsOpen)
            ImGui::EndDragDropTarget();
    }

    explicit operator bool() const
    {
        return IsOpen;
    }

    DELETE_MOVE_COPY(DragDropTarget);
};

struct Disabled
{
    Disabled(bool disabled = true)
    {
        ImGui::BeginDisabled(disabled);
    }
    ~Disabled()
    {
        ImGui::EndDisabled();
    }

    DELETE_MOVE_COPY(Disabled);
};

struct ClipRect
{
    ClipRect(const ImVec2& clip_rect_min, const ImVec2& clip_rect_max, bool intersect_with_current_clip_rect)
    {
        ImGui::PushClipRect(clip_rect_min, clip_rect_max, intersect_with_current_clip_rect);
    }
    ~ClipRect()
    {
        ImGui::PopClipRect();
    }

    DELETE_MOVE_COPY(ClipRect);
};

#undef DELETE_MOVE_COPY

} // namespace ui
} // namespace scoped
#endif
