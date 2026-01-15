#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#define ImDrawIdx unsigned int

#ifndef NDEBUG
#define USE_GUINEA_ASSERT
namespace ImGui
{
void guinea_assert(const char* msg);
}

#define IM_ASSERT(_cond) IM_ASSERT_USER_ERROR(_cond, #_cond)

#define IM_ASSERT_USER_ERROR(_cond, _msg) \
    do                                    \
    {                                     \
        if (!(_cond))                     \
            ImGui::guinea_assert((_msg)); \
    } while (0)
#endif
