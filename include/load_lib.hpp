#pragma once

#include <string>

#if defined(_WIN32) || defined(_WIN64)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace lib
{
#if defined(_WIN32) || defined(_WIN64)
using handle = HINSTANCE;
using symbol = FARPROC;
#else
using handle = void*;
using symbol = void*;
#endif

handle open(const std::string& path) noexcept;
symbol get_symbol(handle handle, const char* name) noexcept;
void close(handle handle) noexcept;
char* get_error_message() noexcept;

struct closer
{
    void operator()(handle handle)
    {
        close(handle);
    }
};

} // namespace lib

namespace lib
{
#if defined(_WIN32) || defined(_WIN64)

inline handle open(const std::string& path) noexcept
{
    return LoadLibraryA(path.c_str());
}
inline symbol get_symbol(handle handle, const char* name) noexcept
{
    return GetProcAddress(handle, name);
}
inline void close(handle handle) noexcept
{
    if (handle)
        FreeLibrary(handle);
}
inline char* get_error_message() noexcept
{
    constexpr size_t buf_size = 512;
    auto error_code           = GetLastError();
    if (!error_code)
        return nullptr;
    static char msg[buf_size];
    const DWORD len = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, error_code, 0, msg, buf_size, nullptr);
    if (len > 0)
        return msg;
    return nullptr;
}
#else

inline handle open(const std::string& path) noexcept
{
    return dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
}
inline symbol get_symbol(handle handle, const char* name) noexcept
{
    return dlsym(handle, name);
}
inline void close(handle handle) noexcept
{
    if (handle)
        dlclose(handle);
}
inline char* get_error_message() noexcept
{
    return dlerror();
}
#endif
} // namespace lib