#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "OurPrimitive.h"
#include "GameObject.h"

#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "ImGui/backends/imgui_impl_sdl2.h"

ModuleEditor::ModuleEditor(Application * app, bool start_enabled) : Module(app, start_enabled)
{
    logVector = new std::vector<std::string>();
}

ModuleEditor::~ModuleEditor()
{

}

bool ModuleEditor::Init()
{
    io = nullptr;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls


    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    // "pilota"
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    // Canviar color de editor
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
    ImGui_ImplOpenGL3_Init();

    mFPSLog.reserve(30);
    mMsLog.reserve(30);

    cvCounter = 0;

    // Config window
    fullscreen = false;
    screenWidth = SCREEN_WIDTH;

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

   isActivatedHierarchy = true;
   isActivatedDemo = false;
   isActivatedInspector = true;
   isActivatedConsole = true;

   GetHardwareInfo();

    return true;
}


// Tot lo de ImGui ha de passar per aqui
update_status ModuleEditor::DrawEditor()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    // OpenGL Stuff

    // ImGui Stuff

    ImGui::NewFrame();

    AddHistogramData(App->GetDT() * 1000.0f, mMsLog);
    AddHistogramData(1 / App->GetDT(), mFPSLog);

    // Docking for all windows
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoNavFocus;

    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(main_viewport->WorkPos);
    ImGui::SetNextWindowSize(main_viewport->Size);
    ImGui::SetNextWindowViewport(main_viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin(" ", nullptr, flags);

    ImGui::PopStyleVar(3);

    ImGuiID dockID = ImGui::GetID("MainDockZone");
    ImGui::DockSpace(dockID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::End();
    // Finish docking for all windows

    // For Quit button on file
    update_status ret = update_status::UPDATE_CONTINUE;
    // HERE ALL WINDOW RENDERS
    // Always EndMenu when Begin Menu
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Quit", "ESC"))
                ret = UPDATE_STOP;
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Assets"))
        {
            ImGui::Text("Hello world!");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Objects"))
        {
            if (ImGui::MenuItem("Empty Object")) {
                OurPrimitive::CreatePrimitive(ShapeType::EMPTY);
            }

            if (ImGui::MenuItem("Plane")) {
                OurPrimitive::CreatePrimitive(ShapeType::PLANE);
            }

            if (ImGui::MenuItem("Cube")) {
                OurPrimitive::CreatePrimitive(ShapeType::CUBE);
            }

            if (ImGui::MenuItem("Sphere")) {
                OurPrimitive::CreatePrimitive(ShapeType::SPHERE);
            }

            if (ImGui::MenuItem("Cylinder")) {
                OurPrimitive::CreatePrimitive(ShapeType::CYLINDER);
            }

            if (ImGui::MenuItem("Monkey")) {
                OurPrimitive::CreatePrimitive(ShapeType::MONKEY);
            }

            if (ImGui::MenuItem("Icosphere")) {
                OurPrimitive::CreatePrimitive(ShapeType::ICOSHPERE);
            }

            if (ImGui::MenuItem("Cone")) {
                OurPrimitive::CreatePrimitive(ShapeType::CONE);
            }

            if (ImGui::MenuItem("Torus")) {
                OurPrimitive::CreatePrimitive(ShapeType::TORUS);
            }


            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Configuration"))
        {
            ImGuiIO& io = ImGui::GetIO();

            static int volumeLevel = 0;

            if (ImGui::CollapsingHeader("FPS Histogram"))
            {
                ImGui::PlotHistogram("FPS", &mFPSLog[0], mFPSLog.size(), 0, "FPS", 0.0f, 100.0f, ImVec2(450, 100));
                ImGui::PlotHistogram("Ms.", &mMsLog[0], mMsLog.size(), 0, "Miliseconds", 0.0f, 100.0f, ImVec2(450, 100));
            }


            // Window
            if (ImGui::CollapsingHeader("Window"))
            {
                ImGui::Text("Window Size: %d x %d", App->window->e_width, App->window->e_height);
                if(ImGui::Checkbox("Fullscreen", &fullscreen))
                {
                    if (fullscreen)
                    {
                        App->window->SetFullscreen();
                    }
                    else
                    {
                        App->window->SetResizable();
                    }
                }

                /*int preWidth = App->window->e_width;
                int preHeight = App->window->e_height;

                ImGui::SliderInt("Window Width", &App->window->e_width, 320, 2560);
                if (preWidth != App->window->e_width)
                {
                    App->window->ChangeWidth();
                }

                ImGui::SliderInt("Window Height", &App->window->e_height, 256, 2048);
                if (preHeight != App->window->e_height)
                {
                    App->window->ChangeHeight();
                }*/
               

                ImGui::Text("View");
                if (ImGui::Checkbox("DemoWindow", &isActivatedDemo))
                {
                    isActivatedDemo != isActivatedDemo;
                }
                if (ImGui::Checkbox("Hierarchy", &isActivatedHierarchy))
                {
                    isActivatedHierarchy != isActivatedHierarchy;
                }
                if (ImGui::Checkbox("Inspector", &isActivatedInspector))
                {
                    isActivatedInspector != isActivatedInspector;
                }
                if (ImGui::Checkbox("Console", &isActivatedConsole))
                {
                    isActivatedConsole != isActivatedConsole;
                }
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
                    ImGui::Text("Brightness");
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

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Hardware Info"))
        {
            ImGui::Text("GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
            ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
            ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));

            ImGui::Separator();
            ImGui::Text("CPUs: %d (Cache: %dkb)", CPUCount, CPUCache);
            ImGui::Text("System RAM: %.1fGb", systemRAM);
            ImGui::Text("Caps: %s", caps.c_str());

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("About"))
        {
            ImGuiIO& io = ImGui::GetIO();

            ImGui::SeparatorText("ABOUT THIS ENGINE:");
            ImGui::Text("KaChow Engine");
            ImGui::Text("I'm faster than fast. Faster than fast. I'm lightning!");
            ImGui::NewLine();

            ImGui::Text("By Arnau Gonzalez & Pau Argiz");
            if (ImGui::Button("Github Repository"))
            {
                ShellExecute(0, 0, "https://github.com/arinWald/KaChowEngine", 0, 0, SW_SHOW);
            }

            ImGui::SeparatorText("3rd Party Libraries used"); 

            if (ImGui::Button("SDL"))
            {
                ShellExecute(0, 0, "https://libsdl.org/index.php", 0, 0, SW_SHOW);
            }
            ImGui::SameLine();
            ImGui::Text("Version: %s", SDLVersion.c_str());
            if (ImGui::Button("OpenGL"))
            {
                ShellExecute(0, 0, "https://www.opengl.org/", 0, 0, SW_SHOW);
            }
            ImGui::SameLine();
            ImGui::Text("Version: %s", glGetString(GL_VERSION));
            
            if(ImGui::Button("Glew"))
            {
                ShellExecute(0, 0, "https://www.opengl.org/", 0, 0, SW_SHOW);
            }
            ImGui::SameLine();
            ImGui::Text("Version: %s", glewGetString(GLEW_VERSION));
            
            if(ImGui::Button("ImGui"))
            {
                ShellExecute(0, 0, "https://github.com/ocornut/imgui/", 0, 0, SW_SHOW);
            }
            ImGui::SameLine();
            ImGui::Text("Version: ");
            ImGui::SameLine();
            ImGui::Text(ImGui::GetVersion());

            if (ImGui::Button("MathGeoLib"))
            {
                ShellExecute(0, 0, "https://github.com/juj/MathGeoLib", 0, 0, SW_SHOW);
            }
            ImGui::SameLine();
            ImGui::Text("Version 1.5");

            if (ImGui::Button("DevIL"))
            {
                ShellExecute(0, 0, "http://openil.sourceforge.net/", 0, 0, SW_SHOW);
            }
            ImGui::SameLine();
            ImGui::SameLine();
            ImGui::Text("Version v1.8.0");

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


            ImGui::EndMenu();
        }
       

        ImGui::EndMainMenuBar();
    }
    if (isActivatedHierarchy) {
        if (ImGui::Begin("Hierarchy"))
        {
            App->scene->PrintHierarchy(App->scene->rootGameObject, 0);
        }
        ImGui::End();
    }

    if (isActivatedInspector)
    {
        if (App->scene->selectedGameObj != nullptr)
        {
            if (ImGui::Begin("Inspector"))
            {
                App->scene->selectedGameObj->PrintOnInspector();
            }
            ImGui::End();
        }
    }
    
    if(isActivatedConsole)
    {
        if (ImGui::Begin("Console Log"))
        {
            ImGui::NewLine();

            int size = logVector->size();
            for (int i = 0; i < size; i++)
            {
                ImGui::Text((*logVector)[i].c_str());
            }

            ImGui::End();
        }
    }

    if (isActivatedDemo) {
        ImGui::ShowDemoWindow();
    }

    App->scene->WindowScene();
    
    App->scene->WindowGame();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return ret;
}

bool ModuleEditor::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    RELEASE(logVector);

    return true;
}

void ModuleEditor::AddHistogramData(const float aFPS, std::vector<float>& data_vector)
{
    if (data_vector.size() == data_vector.capacity())
    {

        for (unsigned int i = 0; i < data_vector.size(); i++)
        {
            if (i + 1 < data_vector.size())
            {
                float iCopy = data_vector[i + 1];
                data_vector[i] = iCopy;
            }
        }
        data_vector[data_vector.capacity() - 1] = aFPS;

    }
    else
    {
        data_vector.push_back(aFPS);
    }
}


void ModuleEditor::ConsoleLog(const char* tmp_string)
{
    if (logVector == nullptr) return;
    logVector->push_back(tmp_string);
}

void ModuleEditor::GetHardwareInfo()
{
    caps += (SDL_HasRDTSC()) ? "RDTSC," : "";
    caps += (SDL_HasMMX()) ? "MMX, " : "";
    caps += (SDL_HasSSE()) ? "SSE, " : "";
    caps += (SDL_HasSSE2()) ? "SSE2, " : "";
    caps += (SDL_HasSSE3()) ? "SSE3, " : "";
    caps += "\n";
    caps += (SDL_HasSSE41()) ? "SSE41, " : "";
    caps += (SDL_HasSSE42()) ? "SSE42, " : "";
    caps += (SDL_HasAVX()) ? "AVX, " : "";
    caps += (SDL_HasAltiVec()) ? "AltiVec, " : "";
    caps += (SDL_Has3DNow()) ? "3DNow, " : "";

    SDL_version version;
    SDL_GetVersion(&version);
    SDLVersion = std::to_string(version.major) + '.' + std::to_string(version.minor) + '.' + std::to_string(version.patch);
    CPUCount = SDL_GetCPUCount();
    CPUCache = SDL_GetCPUCacheLineSize();
    systemRAM = SDL_GetSystemRAM() / 1024.f;

    uint vendor, deviceId;
    std::wstring brand;
    unsigned __int64 videoMemBudget;
    unsigned __int64 videoMemUsage;
    unsigned __int64 videoMemAvailable;
    unsigned __int64 videoMemReserved;

    // GPU INFO TOO COMPLICATED
}
