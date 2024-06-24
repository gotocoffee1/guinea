#include "guinea_impl.hpp"

#include "imgui_impl_opengl2.h"
#include "imgui_impl_sdl2.h"
#include <SDL.h>
#include <SDL_opengl.h>

#include <cstring>

bool ui::guinea::impl::loop(ui::guinea& self, ImGuiContext* ctx) noexcept
{
    ImGui::SetCurrentContext(ctx);
    ImGuiIO& io              = ImGui::GetIO();
    SDL_Window* window       = reinterpret_cast<SDL_Window*>(self.platform_data);
    SDL_GLContext gl_context = reinterpret_cast<SDL_GLContext>(self.renderer_data);

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
    ImGui_ImplOpenGL2_NewFrame();

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

    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(self.clear_color.x, self.clear_color.y, self.clear_color.z, self.clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SwapWindow(window);

    return keep_run;
}

void ui::guinea::impl::shutdown(ui::guinea& self, ImGuiContext* ctx) noexcept
{
    ImGui::SetCurrentContext(ctx);
    SDL_Window* window       = reinterpret_cast<SDL_Window*>(self.platform_data);
    SDL_GLContext gl_context = reinterpret_cast<SDL_GLContext>(self.renderer_data);
    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplSDL2_Shutdown();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
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
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MAXIMIZED);
    SDL_Window* window           = SDL_CreateWindow(self.init.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, static_cast<int>(self.init.resolution.x), static_cast<int>(self.init.resolution.y), window_flags);
    SDL_GLContext gl_context     = SDL_GL_CreateContext(window);

    self.platform_data = window;
    self.renderer_data = gl_context;
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(self.init.vsync); // vsync

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL2_Init();
}

#include "gl_texture.hpp"

#ifndef BUILD_GUINEA_BACKEND_STATIC
extern "C" EXPORT ui::guinea_func guinea{
    ui::guinea::impl::init,
    ui::guinea::impl::loop,
    ui::guinea::impl::shutdown,
    ui::guinea::impl::load_texture,
    ui::guinea::impl::unload_texture,
};
#endif
