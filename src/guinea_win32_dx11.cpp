#include "guinea_impl.hpp"

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include <d3d11.h>
#include <tchar.h>

#ifdef _MSC_VER
#pragma comment(lib, "d3d11")
#endif

#include <cstring>

// Forward declarations of helper functions
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;

struct renderer_data_t
{
    ID3D11Device* g_pd3dDevice                     = nullptr;
    ID3D11DeviceContext* g_pd3dDeviceContext       = nullptr;
    IDXGISwapChain* g_pSwapChain                   = nullptr;
    ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

    void CleanupDeviceD3D()
    {
        CleanupRenderTarget();
        if (g_pSwapChain)
        {
            g_pSwapChain->Release();
            g_pSwapChain = nullptr;
        }
        if (g_pd3dDeviceContext)
        {
            g_pd3dDeviceContext->Release();
            g_pd3dDeviceContext = nullptr;
        }
        if (g_pd3dDevice)
        {
            g_pd3dDevice->Release();
            g_pd3dDevice = nullptr;
        }
    }

    void CreateRenderTarget()
    {
        ID3D11Texture2D* pBackBuffer;
        g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
        pBackBuffer->Release();
    }

    void CleanupRenderTarget()
    {
        if (g_mainRenderTargetView)
        {
            g_mainRenderTargetView->Release();
            g_mainRenderTargetView = nullptr;
        }
    }
};

struct platform_data_t
{
    WNDCLASSEX wc;
    HWND hwnd;
};

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth  = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            //const int dpi = HIWORD(wParam);
            //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

static bool CreateDeviceD3D(HWND hWnd, renderer_data_t* rd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount                        = 2;
    sd.BufferDesc.Width                   = 0;
    sd.BufferDesc.Height                  = 0;
    sd.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator   = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags                              = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow                       = hWnd;
    sd.SampleDesc.Count                   = 1;
    sd.SampleDesc.Quality                 = 0;
    sd.Windowed                           = TRUE;
    sd.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
    };
    if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &rd->g_pSwapChain, &rd->g_pd3dDevice, &featureLevel, &rd->g_pd3dDeviceContext) != S_OK)
        return false;

    rd->CreateRenderTarget();
    return true;
}

void ui::guinea::impl::init(ui::guinea& self, ImGuiContext* ctx) noexcept
{
    ImGui::SetCurrentContext(ctx);

    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
#ifdef UNICODE
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &self.title[0], (int)self.title.size(), nullptr, 0);
    std::wstring title(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &self.title[0], (int)self.title.size(), &title[0], size_needed);
#else
    std::string title = self.init.title;
#endif // UNICODE

    auto pd            = new platform_data_t();
    auto rd            = new renderer_data_t();
    self.platform_data = pd;
    self.renderer_data = rd;

    pd->wc = {sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, _T("guinea"), nullptr};
    ::RegisterClassEx(&pd->wc);
    pd->hwnd = ::CreateWindow(pd->wc.lpszClassName, title.c_str(), WS_OVERLAPPEDWINDOW, 100, 100, static_cast<int>(self.init.resolution.x), static_cast<int>(self.init.resolution.y), nullptr, nullptr, pd->wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(pd->hwnd, rd))
    {
        rd->CleanupDeviceD3D();
        ::UnregisterClass(pd->wc.lpszClassName, pd->wc.hInstance);
        self.failure("");
        return;
    }

    // Show the window
    ::ShowWindow(pd->hwnd, SW_SHOWMAXIMIZED);
    ::UpdateWindow(pd->hwnd);

    // Setup Platform/Renderer bindings
    ImGui_ImplWin32_Init(pd->hwnd);
    ImGui_ImplDX11_Init(rd->g_pd3dDevice, rd->g_pd3dDeviceContext);
}

bool ui::guinea::impl::loop(ui::guinea& self, ImGuiContext* ctx) noexcept
{
    ImGui::SetCurrentContext(ctx);

    ImGuiIO& io = ImGui::GetIO();

    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    bool keep_run = true;
    std::string dropfile;
    const char* droptype = nullptr;
    MSG msg;
    while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
        if (msg.message == WM_QUIT)
            keep_run = false;
    }
    auto rd = static_cast<renderer_data_t*>(self.renderer_data);

    // Handle window resize (we don't resize directly in the WM_SIZE handler)
    if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
    {
        rd->CleanupRenderTarget();
        rd->g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
        g_ResizeWidth = g_ResizeHeight = 0;
        rd->CreateRenderTarget();
    }

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    if (droptype)
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceExtern))
        {
            ImGui::SetDragDropPayload(droptype, &dropfile[0], std::size(dropfile));
            ImGui::EndDragDropSource();
        }
    self.render();
    // Rendering
    ImGui::Render();

    const float clear_color_with_alpha[4] = {self.clear_color.x * self.clear_color.w, self.clear_color.y * self.clear_color.w, self.clear_color.z * self.clear_color.w, self.clear_color.w};
    rd->g_pd3dDeviceContext->OMSetRenderTargets(1, &rd->g_mainRenderTargetView, nullptr);
    rd->g_pd3dDeviceContext->ClearRenderTargetView(rd->g_mainRenderTargetView, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
    rd->g_pSwapChain->Present(self.init.vsync, 0); // Preset (vsync)?

    return keep_run;
}

void ui::guinea::impl::shutdown(ui::guinea& self, ImGuiContext* ctx) noexcept
{
    ImGui::SetCurrentContext(ctx);

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();

    auto pd = static_cast<platform_data_t*>(self.platform_data);
    auto rd = static_cast<renderer_data_t*>(self.renderer_data);

    ::DestroyWindow(pd->hwnd);
    ::UnregisterClass(pd->wc.lpszClassName, pd->wc.hInstance);

    delete pd;
    delete rd;
}

#include "dx11_texture.hpp"

#ifndef BUILD_GUINEA_BACKEND_STATIC
extern "C" EXPORT ui::guinea_func guinea{
    ui::guinea::impl::init,
    ui::guinea::impl::loop,
    ui::guinea::impl::shutdown,
    ui::guinea::impl::load_texture,
    ui::guinea::impl::unload_texture,
};
#endif
