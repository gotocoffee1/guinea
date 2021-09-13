#include "guinea.hpp"

#include "imgui.h"
#include "imgui_impl_sdl.h"

#include "imgui_impl_opengl3.h"
#include <SDL.h>
#include <SDL_opengles2.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <cstring>

namespace ui
{
// Emscripten requires to have full control over the main loop. We're going to store our SDL book-keeping variables globally.
// Having a single function that acts as a loop prevents us to store state in the stack of said function. So we need some location for this.
static SDL_Window* g_Window      = NULL;
static SDL_GLContext g_GLContext = NULL;

// For clarity, our main loop code is declared at the end.
static void main_loop(void*);

int guinea::launch(int argc, char** argv) noexcept
{
    auto name = setup(argc, argv);

    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        failure(SDL_GetError());
        return -1;
    }

    // For the browser using Emscripten, we are going to use WebGL1 with GL ES2. See the Makefile. for requirement details.
    // It is very likely the generated file won't work in many browsers. Firefox is the only sure bet, but I have successfully
    // run this code on Chrome for Android for example.
    const char* glsl_version = "#version 100";
    //const char* glsl_version = "#version 300 es";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MAXIMIZED);
    g_Window                     = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    g_GLContext                  = SDL_GL_CreateContext(g_Window);
    if (!g_GLContext)
    {
        failure("Failed to initialize WebGL context!");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    create_context();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking

    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = NULL;

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(g_Window, g_GLContext);
    ImGui_ImplOpenGL3_Init(glsl_version);

    load();

    done      = false;
    frame_cnt = 0;

    emscripten_set_beforeunload_callback(
        this,
        [](int, const void*, void* arg) -> const char*
        {
            guinea& self = *static_cast<guinea*>(arg);
            self.done    = true;
            self.unload();
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplSDL2_Shutdown();
            self.destroy_context();
            SDL_GL_DeleteContext(g_GLContext);
            SDL_DestroyWindow(g_Window);
            SDL_Quit();

            self.shutdown();

            return nullptr;
        });

    emscripten_set_timeout_loop([](double, void* arg) -> EM_BOOL
                                {
                                    guinea& self = *static_cast<guinea*>(arg);
                                    if (self.update(self.done))
                                        self.frame_cnt = 0;
                                    return EM_TRUE;
                                },
                                1000.0 / 30.0,
                                this);
    emscripten_set_main_loop_arg(inner_loop, this, 0, true);
    return EXIT_SUCCESS;
}

void guinea::inner_loop(void* arg) noexcept
{
    ImGuiIO& io  = ImGui::GetIO();
    guinea& self = *static_cast<guinea*>(arg);

    ImVec4 clear_color = ImColor(62, 62, 66);

    self.done = false;
    std::string dropfile;
    const char* droptype = nullptr;
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type)
        {
        case (SDL_QUIT):
            self.done = true;
            break;
        case (SDL_WINDOWEVENT):
            if (event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                self.done = true;
            break;
        case (SDL_DROPTEXT):
        case (SDL_DROPFILE):
            droptype = (event.type == SDL_DROPFILE) ? UI_EXTERN_FILE : UI_EXTERN_TEXT;
            dropfile.append(event.drop.file, std::strlen(event.drop.file) + 1);
            SDL_free(event.drop.file);
            break;
        }
        self.frame_cnt = 0;
    }
    if (self.frame_cnt++ < 5)
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(g_Window);
        ImGui::NewFrame();
        if (droptype)
            if (GUI(DragDropSource, ImGuiDragDropFlags_SourceExtern))
                ImGui::SetDragDropPayload(droptype, &dropfile[0], std::size(dropfile));
        self.render();
        // Rendering
        ImGui::Render();
        SDL_GL_MakeCurrent(g_Window, g_GLContext);
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(g_Window);
    }
}
} // namespace ui
