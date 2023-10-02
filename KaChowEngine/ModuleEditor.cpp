#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "ImGui/backends/imgui_impl_sdl2.h"

ModuleEditor::ModuleEditor(Application * app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleEditor::~ModuleEditor()
{

}

bool ModuleEditor::Init()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // "pilota"

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    // "pilota"
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style

    // Canviar color de editor
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
    ImGui_ImplOpenGL3_Init();

    mFPSLog.reserve(30);

    cvCounter = 0;

    // Config opengl vars init
   depthTest = false;
   isCullFace = false;
   lightning = false;
   colorMat = false;
   texture2D = false;
   vSync = false;
   isWireframe = false;
   bright = 0.0f;
   bright_aux = 0.0f;

    return true;
}


// Tot lo de ImGui ha de passar per aqui
void ModuleEditor::DrawEditor()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    // OpenGL Stuff

    // ImGui Stuff

    ImGui::NewFrame();

    App->editor->AddFPS(App->GetDT());

    // HERE ALL WINDOW RENDERS
    // Always EndMenu when Begin Menu
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::Text("Hello world!");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Assets"))
        {
            ImGui::Text("Hello world!");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Objects"))
        {
            ImGui::Text("Hello world!");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Configuration"))
        {
            ImGuiIO& io = ImGui::GetIO();

            static int volumeLevel = 0;

            // Pilota borrar quan es tingui variable vsync
            static bool* vsyncBoolTest = false;

            if (ImGui::CollapsingHeader("FPS Histogram"))
            {
                ImGui::PlotHistogram("FPS", mFPSLog.data(), mFPSLog.size());
                //ImGui::PlotHistogram("FPS", mFPSLog.data(), mFPSLog.size(), 0, 0, FLT_MAX, FLT_MAX, (5, 5));
            }


            // Window
            if (ImGui::CollapsingHeader("Window"))
            {
                ImGui::Text("Window Size: %d x %d", SCREEN_WIDTH, SCREEN_HEIGHT);
                    /*ImGui::Text("Brightness: %d", BARIABLE);*/
            }
           

            // Render
            if (ImGui::CollapsingHeader("Render"))
            {
                //ImGui::Checkbox("vSync", vsyncBoolTest);
                if (ImGui::TreeNode("OpenGL"))
                {

                    if (ImGui::Checkbox("Lightning", &lightning))
                    {
                        if (lightning)
                        {
                            glEnable(GL_LIGHTING);
                            LOG("Lights On");
                        }
                        else
                        {
                            glDisable(GL_LIGHTING);
                            LOG("Lights Off");
                        }
                    }

                    if (ImGui::Checkbox("Cull Face", &isCullFace))
                    {
                        if (isCullFace)
                        {
                            glEnable(GL_CULL_FACE);
                            LOG("Cull Face On");
                        }
                        else
                        {
                            glDisable(GL_CULL_FACE);
                            LOG("Cull Face Off");
                        }
                    }

                    if (ImGui::Checkbox("Depth Test", &depthTest))
                    {
                        if (depthTest)
                        {
                            glEnable(GL_DEPTH_TEST);
                            LOG("Depth Test On");
                        }
                        else
                        {
                            glDisable(GL_DEPTH_TEST);
                            LOG("Depth Test Off");
                        }
                    }

                    if (ImGui::Checkbox("Color Material", &colorMat))
                    {
                        if (colorMat)
                        {
                            glEnable(GL_COLOR_MATERIAL);
                            LOG("Color Material On");
                        }
                        else
                        {
                            glDisable(GL_COLOR_MATERIAL);
                            LOG("Color Material Off");
                        }
                    }

                    //Wireframe option
                    if (ImGui::Checkbox("Wireframe", &isWireframe))
                    {
                        if (isWireframe)
                        {
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            LOG("Wireframe Mode On");
                        }
                        else
                        {
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            LOG("Wireframe Mode Off");
                        }
                    }                    

                    if (ImGui::Checkbox("Texture 2D", &texture2D))
                    {
                        if (texture2D)
                        {
                            glEnable(GL_TEXTURE_2D);
                            LOG("Texture 2D On");
                        }
                        else
                        {
                            glDisable(GL_TEXTURE_2D);
                            LOG("Texture 2D Off");
                        }
                    }

                    if (ImGui::Checkbox("Vsync", &vSync))
                    {
                        if (vSync)
                        {
                            SDL_GL_SetSwapInterval(1);
                            LOG("Vsync On");
                        }
                        else
                        {
                            SDL_GL_SetSwapInterval(0);
                            LOG("Vsync Off");
                        }
                    }

                    ImGui::Text("Min");
                    ImGui::SameLine();
                    if (ImGui::SliderFloat(" Max", &bright, 0.100f, 1.000f))
                    {
                        SDL_SetWindowBrightness(App->window->window, bright);
                        bright_aux = bright * 100;
                    }

                    ImGui::TreePop();
                }

            }

            // Input
            if (ImGui::CollapsingHeader("Input"))
            {
                // MOUSE
                ImGui::Text("Mouse");
                if (ImGui::IsMousePosValid())
                    ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
                else
                    ImGui::Text("Mouse pos: <INVALID>");
                ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
                ImGui::Text("Mouse down:");
                for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDown(i)) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
                ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);
                // KEYBOARD
                ImGui::Text("Keyboard");
            }

            if (ImGui::CollapsingHeader("Audio"))
            {
                // Audio
                ImGui::SliderInt("Volume", &volumeLevel, 0, 100);
            }

            ImGui::End();
        }

        if (ImGui::BeginMenu("About"))
        {
            ImGuiIO& io = ImGui::GetIO();

            ImGui::SeparatorText("ABOUT THIS ENGINE:");
            ImGui::Text("KaChow Engine");
            ImGui::Text("I'm faster than fast. Faster than fast. I'm lightning!");
            ImGui::NewLine();

            ImGui::Text("By Arnau Gonzalez & Pau Argiz");
            ImGui::SeparatorText("3rd Party Libraries used");
            ImGui::BulletText("Glew 20.0");
            ImGui::BulletText("ImGui 1.51");
            ImGui::BulletText("MathGeoLib 1.5");
            ImGui::BulletText("OpenGL %s", glewGetString(GLEW_VERSION));

            ImGui::SeparatorText("LICENSE");
            ImGui::BulletText("MIT License");
            ImGui::NewLine();
            ImGui::Text("Copyright (c) Arnau Gonzalez & Pau Argiz");
            ImGui::NewLine();
            ImGui::TextWrapped("Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:");
            ImGui::NewLine();
            ImGui::TextWrapped("The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software");
            ImGui::NewLine();
            ImGui::TextWrapped("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY. FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE");
            ImGui::NewLine();
            ImGui::TextWrapped("This is the MIT License, which is a very permissive license that allows users to do almost anything they want with the software, including using it for commercial purposes. The only requirement is that the copyright notice and permission notice must be included in all copies or substantial portions of the software.");

            if (ImGui::Button("Add 1 HP to the engine"))
            {
                cvCounter++;
            }
            ImGui::Text("Horse Power: %d", cvCounter);

            ImGui::End();
        }

        ImGui::EndMainMenuBar();
    }

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool ModuleEditor::CleanUp()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    return true;
}

void ModuleEditor::AddFPS(const float aFPS)
{
    if (mFPSLog.size() < 30)
    {
        mFPSLog.push_back(aFPS);
    }
    else
    {
        for (unsigned int i = 0; i < mFPSLog.size(); i++)
        {
            if (i + 1 < mFPSLog.size())
            {
                float iCopy = mFPSLog[i + 1];
                mFPSLog[i] = iCopy;
            }
        }
        mFPSLog[mFPSLog.capacity() - 1] = aFPS;
    }
}

/*
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
*/