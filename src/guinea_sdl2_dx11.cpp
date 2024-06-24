#include "guinea_impl.hpp"

#include "imgui_impl_dx11.h"
#include "imgui_impl_sdl2.h"
#include <SDL.h>
#include <SDL_syswm.h>
#include <d3d11.h>

#ifdef _MSC_VER
#pragma comment(lib, "d3d11")
#endif

#include <cstring>

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

    // Setup SDL
    SDL_SetMainReady();
    SDL_SetHint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER, "1");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        self.failure(SDL_GetError());
        return;
    }

    // Setup window

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MAXIMIZED);
    SDL_Window* window           = SDL_CreateWindow(self.init.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, static_cast<int>(self.init.resolution.x), static_cast<int>(self.init.resolution.y), window_flags);
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
    HWND hwnd = (HWND)wmInfo.info.win.window;

    auto rd            = new renderer_data_t();
    self.platform_data = window;
    self.renderer_data = rd;

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd, rd))
    {
        rd->CleanupDeviceD3D();
        self.failure("");
        return;
    }

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForD3D(window);
    ImGui_ImplDX11_Init(rd->g_pd3dDevice, rd->g_pd3dDeviceContext);
}

void ui::guinea::impl::shutdown(ui::guinea& self, ImGuiContext* ctx) noexcept
{
    ImGui::SetCurrentContext(ctx);
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplSDL2_Shutdown();

    SDL_Window* window = reinterpret_cast<SDL_Window*>(self.platform_data);
    auto rd            = static_cast<renderer_data_t*>(self.renderer_data);

    rd->CleanupDeviceD3D();
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool ui::guinea::impl::loop(ui::guinea& self, ImGuiContext* ctx) noexcept
{
    ImGui::SetCurrentContext(ctx);
    ImGuiIO& io        = ImGui::GetIO();
    SDL_Window* window = reinterpret_cast<SDL_Window*>(self.platform_data);

    auto rd = static_cast<renderer_data_t*>(self.renderer_data);

    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    bool keep_run = true;
    std::string dropfile;
    const char* droptype = nullptr;
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

        switch (event.type)
        {
        case (SDL_QUIT):
            keep_run = false;
            break;
        case (SDL_WINDOWEVENT):
            if (event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                keep_run = false;
            if (event.window.event == SDL_WINDOWEVENT_RESIZED && event.window.windowID == SDL_GetWindowID(window))
            {
                // Release all outstanding references to the swap chain's buffers before resizing.
                rd->CleanupRenderTarget();
                rd->g_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
                rd->CreateRenderTarget();
            }
            break;
        case (SDL_DROPTEXT):
        case (SDL_DROPFILE):
            droptype = (event.type == SDL_DROPFILE) ? UI_EXTERN_FILE : UI_EXTERN_TEXT;
            dropfile.append(event.drop.file, std::strlen(event.drop.file) + 1);
            SDL_free(event.drop.file);
            break;
        }
    }

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplSDL2_NewFrame();
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
    rd->g_pd3dDeviceContext->OMSetRenderTargets(1, &rd->g_mainRenderTargetView, NULL);
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
