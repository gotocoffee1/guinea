#include "guinea_impl.hpp"

#include "imgui.h"
#include "imgui_impl_sdl.h"

#include "imgui_impl_opengl3.h"
#include <SDL.h>
#include <SDL_opengles2.h>
#include <emscripten.h>
#include <emscripten/html5.h>

#include <cstring>

struct ui::guinea::impl
{
    // Emscripten requires to have full control over the main loop. We're going to store our SDL book-keeping variables globally.
    // Having a single function that acts as a loop prevents us to store state in the stack of said function. So we need some location for this.
    static inline SDL_Window* g_Window      = NULL;
    static inline SDL_GLContext g_GLContext = NULL;

    static void loop(ui::guinea& self) noexcept
    {
        // Setup SDL
        SDL_SetMainReady();
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
        {
            self.failure(SDL_GetError());
            return;
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
        g_Window                     = SDL_CreateWindow(self.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, static_cast<int>(self.resolution.x), static_cast<int>(self.resolution.y), window_flags);
        g_GLContext                  = SDL_GL_CreateContext(g_Window);
        if (!g_GLContext)
        {
            self.failure("Failed to initialize WebGL context!");
            return;
        }

        // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
        // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
        ImGuiIO& io = ImGui::GetIO();

        io.IniFilename = NULL;

        // Setup Platform/Renderer bindings
        ImGui_ImplSDL2_InitForOpenGL(g_Window, g_GLContext);
        ImGui_ImplOpenGL3_Init(glsl_version);

        self.load();

        self.done      = false;
        self.frame_cnt = 0;

        emscripten_set_beforeunload_callback(
            &self,
            [](int, const void*, void* arg) -> const char*
            {
                guinea& self = *static_cast<guinea*>(arg);
                self.done    = true;

                return nullptr;
            });

        emscripten_set_timeout_loop([](double, void* arg) -> EM_BOOL
                                    {
                                        guinea& self = *static_cast<guinea*>(arg);
                                        if (self.update(self.done))
                                            self.frame_cnt = 0;

                                        if (self.done)
                                        {
                                            emscripten_cancel_main_loop();
                                            self.unload();
                                            ImGui_ImplOpenGL3_Shutdown();
                                            ImGui_ImplSDL2_Shutdown();
                                            SDL_GL_DeleteContext(g_GLContext);
                                            SDL_DestroyWindow(g_Window);
                                            SDL_Quit();
                                            self.shutdown();
                                            return EM_FALSE;
                                        }
                                        return EM_TRUE;
                                    },
                                    1000.0 / 30.0,
                                    &self);
        self.update(self.done);
        emscripten_set_main_loop_arg(inner_loop, &self, self.fps, true);
    }

    static void inner_loop(void* arg) noexcept
    {
        guinea& self = *static_cast<guinea*>(arg);
        ImGuiIO& io  = ImGui::GetIO();

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
                if (event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(g_Window))
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

        static bool WantTextInputLast = false;
        if (io.WantTextInput && !WantTextInputLast)
            emscripten_run_script("if (typeof show_keyboard == 'function') { show_keyboard(); }");
        else if (!io.WantTextInput && WantTextInputLast)
            emscripten_run_script("if (typeof hide_keyboard == 'function') { hide_keyboard(); }");
        WantTextInputLast = io.WantTextInput;

        if (self.frame_cnt++ < 5)
        {
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame(g_Window);
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
            SDL_GL_MakeCurrent(g_Window, g_GLContext);
            glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
            glClearColor(self.clear_color.x, self.clear_color.y, self.clear_color.z, self.clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(g_Window);
        }
    }
};

extern "C" EXPORT void loop(ui::guinea& self, ImGuiContext* ctx) noexcept
{
    ImGui::SetCurrentContext(ctx);
    return ui::guinea::impl::loop(self);
}

#include "gl_texture.hpp"
