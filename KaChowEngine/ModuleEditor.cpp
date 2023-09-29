#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
// "pilota" Hauria de ser imgui_impl_sdl2
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

    return true;
}


// Tot lo de ImGui ha de passar per aqui
void ModuleEditor::DrawEditor()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

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
        if (ImGui::BeginMenu("About"))
        {
            ImGui::Text("Hello world!");
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // "pilota"
    if (ImGui::Begin("Configuration"))
    {
        ImGuiIO& io = ImGui::GetIO();

        static float volumeLevel = 0.0f;

        // Pilota borrar quan es tingui variable vsync
        static bool* vsyncBoolTest = false;

        ImGui::PlotHistogram("FPS", mFPSLog.data(), mFPSLog.size());


        // Window
        ImGui::SeparatorText("WINDOW");
        ImGui::Text("Window Size: %d x %d", SCREEN_WIDTH, SCREEN_HEIGHT);
        /*ImGui::Text("Brightness: %d", BARIABLE);*/

        // Render
        ImGui::SeparatorText("RENDER");
        //ImGui::Checkbox("vSync", vsyncBoolTest);

        // Input
        ImGui::SeparatorText("INPUT");

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

        // Audio
        ImGui::SeparatorText("AUDIO");
        ImGui::SliderFloat("Volume", &volumeLevel, 0.0f, 100.0f);
        ImGui::End();

    }

    if (ImGui::Begin("Help"))
    {
        ImGuiIO& io = ImGui::GetIO();

        int cvCounter = 0;

        ImGui::SeparatorText("ABOUT THIS ENGINE:");
        ImGui::Text("KaChow Engine");
        ImGui::Text("I'm faster than fast. Faster than fast. I'm lightning!");
        ImGui::Text("");

        ImGui::Text("By Arnau Gonzalez & Pau Argiz");
        ImGui::SeparatorText("3rd Party Libraries used");
        ImGui::BulletText("Glew 20.0");
        ImGui::BulletText("ImGul 1.51");
        ImGui::BulletText("MathGeoLib 1.5");
        ImGui::BulletText("OpenGL 3.1");
        ImGui::BulletText("OpenGL 3.1");

        ImGui::SeparatorText("LICENSE");
        ImGui::BulletText("MIT License");
        ImGui::Text(" ");
        ImGui::Text("Copyright (c) Arnau Gonzalez & Pau Argiz");
        ImGui::Text(" ");
        ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:");
        ImGui::Text(" ");
        ImGui::Text("The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software");
        ImGui::Text(" ");
        ImGui::Text("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY. FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE");
        ImGui::Text(" ");
        ImGui::Text("This is the MIT License, which is a very permissive license that allows users to do almost anything they want with the software, including using it for commercial purposes. The only requirement is that the copyright notice and permission notice must be included in all copies or substantial portions of the software.");

        if (ImGui::Button("Add 1 HP to the engine"))
        {
            cvCounter++;
            ImGui::Text("Horse Power: &d", cvCounter);
        }

        ImGui::End();

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