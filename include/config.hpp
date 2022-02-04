#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS

namespace ImGui
{
extern void (*_assert_handler)(void*, const char*);
extern void* _assert_handler_obj;

} // namespace ImGui
#ifndef NDEBUG
#define IM_ASSERT(_cond) IM_ASSERT_USER_ERROR(_cond, #_cond)
#define IM_ASSERT_USER_ERROR(_cond, _msg)                                      \
    ((!(_cond) && ImGui::_assert_handler)                                      \
         ? ImGui::_assert_handler(ImGui::_assert_handler_obj, _msg)            \
         : ((void)0))

#endif
