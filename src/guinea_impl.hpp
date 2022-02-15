#pragma once

#ifdef BUILD_GUINEA_BACKEND_STATIC
#define EXPORT
#else
#if defined(_MSC_VER)
#define EXPORT __declspec(dllexport)
#else 
#define EXPORT __attribute__((visibility("default")))
#endif
#endif

#include "guinea_base.hpp"

#ifndef BUILD_GUINEA_BACKEND_STATIC
namespace ImGui
{
void (*_assert_handler)(void*, const char*) = nullptr;
void* _assert_handler_obj                   = nullptr;
} // namespace ImGui
#endif
