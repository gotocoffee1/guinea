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
