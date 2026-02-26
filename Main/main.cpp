/////////////////////////////////
// PROPERTY OF EYESCIRE ENGINE //
/////////////////////////////////

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "NewScript.hpp"
#include "SceneManager.hpp"
#include "portable-file-dialogs.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <GL/gl.h>
#include <tchar.h>
#include <string>
#include <iostream>

void DisplayFPS() {
    ImGui::Text("     |     FPS: %.1f", ImGui::GetIO().Framerate);
}

// Data stored per platform window
struct WGL_WindowData { HDC hDC; };

// Data
static HGLRC            g_hRC;
static WGL_WindowData   g_MainWindow;
static int              g_Width;
static int              g_Height;

// Forward declarations of helper functions
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data);
void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data);
void ResetDeviceWGL();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool Settings;
bool NEW2D;
bool NEW3D;
bool EXISTING_PROJECT;

bool LOADING_PROJECT = false;

static bool ShowDebugConsole = true;
static bool Settings_Open = false;
static bool ShowFPS = true;
static bool PROJECT_OPEN = false;
static bool PROJECT_2D = false;
static bool PROJECT_3D = false;
static bool project_settings_open = false;

// Main code
int main(int, char**)
{
    std::cout << "-----EYESCIRE ENGINE-----\n\n";
    // Make process DPI aware and obtain main monitor scale
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    // Create application window
    WNDCLASSEXW wc = { sizeof(wc), CS_OWNDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Eyescire Engine", WS_OVERLAPPEDWINDOW, 100, 100, (int)(1280 * main_scale), (int)(800 * main_scale), nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize OpenGL
    if (!CreateDeviceWGL(hwnd, &g_MainWindow))
    {
        CleanupDeviceWGL(hwnd, &g_MainWindow);
        ::DestroyWindow(hwnd);
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }
    wglMakeCurrent(g_MainWindow.hDC, g_hRC);

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_InitForOpenGL(hwnd);
    ImGui_ImplOpenGL3_Init();

    // Load Fonts
    // - If fonts are not explicitly loaded, Dear ImGui will select an embedded font: either AddFontDefaultVector() or AddFontDefaultBitmap().
    //   This selection is based on (style.FontSizeBase * style.FontScaleMain * style.FontScaleDpi) reaching a small threshold.
    // - You can load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - If a file cannot be loaded, AddFont functions will return a nullptr. Please handle those errors in your code (e.g. use an assertion, display an error and quit).
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use FreeType for higher quality font rendering.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //style.FontSizeBase = 20.0f;
    //io.Fonts->AddFontDefaultVector();
    //io.Fonts->AddFontDefaultBitmap();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
    //IM_ASSERT(font != nullptr);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    bool open = true;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;
        if (::IsIconic(hwnd))
        {
            ::Sleep(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        //---------------------------------------//
        //           ImGui Rendering             //
        //---------------------------------------//

        ImGuiIO& io = ImGui::GetIO();
        ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);

        //------------------------------------------//
        //              Project Select              //
        //------------------------------------------//
        if (!PROJECT_OPEN) {
            bool MainEditorWindow = ImGui::Begin("Main Window", &open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
            if (MainEditorWindow)
            {
                ImGui::SetWindowFontScale(1.5f); // Font Size For Title & FPS
                ImGui::Text("Eyescire Engine - Main Editor");
                if (ShowFPS) {
                    ImGui::SameLine();
                    DisplayFPS();
                }
                ImGui::SetWindowFontScale(1.0f); // Instantly Revert Font Size
                ImGui::Separator();
                ImGui::SetWindowFontScale(2.0f);
                NEW2D = ImGui::Button("2D-Project-NEW");
                ImGui::SameLine();
                ImGui::SetWindowFontScale(1.15f);
                ImGui::Text(" - Best for Simple & Complex 2D/2.5D Projects");
                ImGui::Spacing();
                ImGui::SetWindowFontScale(2.0f);
                NEW3D = ImGui::Button("3D-Project-NEW");
                ImGui::SameLine();
                ImGui::SetWindowFontScale(1.15f);
                ImGui::Text(" - Best for Simple & Complex 3D Projects");
                ImGui::Spacing();
                ImGui::SetWindowFontScale(2.0f);
                EXISTING_PROJECT = ImGui::Button("Load Project");
                ImGui::SameLine();
                ImGui::SetWindowFontScale(1.15f);
                ImGui::Text(" - Load Existing Eyescire Engine Project");
                ImGui::Spacing();
                ImGui::SetWindowFontScale(2.0f);

                Settings = ImGui::Button("Eyescire Settings");
                ImGui::SetWindowFontScale(1.0f);

                ImGui::End();
            }

            if (Settings) {
                std::cout << "Load-SETTINGS\n";
                Settings_Open = true;
            }
            if (NEW2D) {
                std::cout << "Load-2D_NEW\n";
                CreateNewScene2D();
                PROJECT_OPEN = true;
                PROJECT_2D = true;
                std::cout << "PROJECT-TYPE2D\n";
            }
            if (NEW3D) {
                std::cout << "Load-3D_NEW\n";
                CreateNewScene3D();
                PROJECT_OPEN = true;
                PROJECT_3D = true;
                std::cout << "PROJECT-TYPE3D\n";
            }
            if (EXISTING_PROJECT) {
                std::cout << "Load-EXISTING_PROJECT\n";
                LOADING_PROJECT = true;
            }
            if (LOADING_PROJECT) {
                // 1. Launch the "Regular" OS File Dialog
                // Syntax: pfd::open_file(Title, DefaultPath, Filters)
                auto selection = pfd::open_file("Select a Eyescire Project File", ".",
                    { "Eyescire Project File", "*.eys",
                      "All Files", "*" }).result();

                // 2. Check if user selected something
                if (!selection.empty()) {
                    std::cout << "Project Selected: " << selection[0] << "\n";
                    std::cout << "Load-EXISTING_PROJECT: " << selection[0] << "\n";
                    PROJECT_OPEN = true;
                }
                else {
                    std::cout << "No Project Selected.\n";
                }
                LOADING_PROJECT = false;
            }
            if (Settings_Open) {
                ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
                if (ImGui::Begin("Eyescire Settings", &Settings_Open))
                {
                    ImGui::SetWindowFontScale(1.5f);
                    ImGui::Text("Eyescire Engine v0.0.5");
                    ImGui::SetWindowFontScale(1.05f);
                    ImGui::Checkbox("Show Debug Console", &ShowDebugConsole);
                    ImGui::Checkbox("Show FPS", &ShowFPS);
                    ImGui::SetWindowFontScale(1.0f);

                    // 1. Check for hover BEFORE ImGui::End()
                    // 2. Use flags to ensure interacting with items (sliders/buttons) doesn't count as "clicking outside"
                    bool isHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

                    // 3. Check for click outside
                    if (!isHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                        // Optional: Verify we didn't just click the "Open Settings" button this same frame
                        if (!ImGui::IsAnyItemHovered()) {
                            std::cout << "Unload-SETTINGS\n";
                            Settings_Open = false;
                        }
                    }
                }
                ImGui::End();
            }
        }
        //------------------------------------------//
        //              MAIN EDITOR                 //
        //------------------------------------------//
        if (PROJECT_OPEN) {
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::Begin("Editor Window", &open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

            ImGui::Text("Placehold for main rendering window");

            if (ImGui::BeginMainMenuBar()) {
                // File tab
                if (ImGui::BeginMenu("File")) {
                    if (ImGui::MenuItem("Save")) {
                        std::cout << "PROJECT-SAVE\n";
                    }
                    if (ImGui::MenuItem("Exit")) {
                        PROJECT_OPEN = false;
                    }
                    ImGui::EndMenu();
                }
                // Project Tab
                if (ImGui::BeginMenu("Project")) {
                    if (ImGui::MenuItem("Project Settings")) {
                        project_settings_open = true;
                        std::cout << "Load-PROJECT_SETTINGS\n";
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }

            // Inspector Window
            float menuBarHeight = ImGui::GetFrameHeight();
            ImVec2 windowSize(200.0f, viewport->WorkSize.y * 0.85f);
            ImVec2 windowPos(viewport->Pos.x + viewport->WorkSize.x - windowSize.x, viewport->Pos.y + menuBarHeight);
            ImGui::SetNextWindowPos(windowPos);
            ImGui::SetNextWindowSize(windowSize);
            if (ImGui::Begin("Inspector", &open)) {

                ImGui::Text("No Gameobject Selected.");

                ImGui::End();
            }

            ImGui::End();
        }
        if (project_settings_open) {
            ImGui::SetNextWindowSize(viewport->WorkSize, ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowPos(viewport->WorkPos, ImGuiCond_FirstUseEver);
            if (!ImGui::Begin("Eyescire Engine - Project Settings", &project_settings_open,
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
            {
                ImGui::End();
            }
            else
            {
                if (PROJECT_2D) {
                    ImGui::Text("Project Type Detected: 2D");
                }
                else if (PROJECT_3D) {
                    ImGui::Text("Project Type Detected: 3D");
                }
                else {
                    ImGui::Text("<ERROR> Unknown Project Type.");
                }
                ImGui::Text("Rendering Backend: OpenGL");
                ImGui::End();
            }
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, g_Width, g_Height);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        // ==========================================
        // >>> PUT YOUR OPENGL DRAW CALLS HERE <<<
        // e.g., glUseProgram(myShader); glBindVertexArray(myVAO); glDrawArrays(...);
        // ==========================================

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Present
        ::SwapBuffers(g_MainWindow.hDC);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceWGL(hwnd, &g_MainWindow);
    wglDeleteContext(g_hRC);
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    HDC hDc = ::GetDC(hWnd);
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;

    const int pf = ::ChoosePixelFormat(hDc, &pfd);
    if (pf == 0)
        return false;
    if (::SetPixelFormat(hDc, pf, &pfd) == FALSE)
        return false;
    ::ReleaseDC(hWnd, hDc);

    data->hDC = ::GetDC(hWnd);
    if (!g_hRC)
        g_hRC = wglCreateContext(data->hDC);
    return true;
}

void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    wglMakeCurrent(nullptr, nullptr);
    ::ReleaseDC(hWnd, data->hDC);
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            g_Width = LOWORD(lParam);
            g_Height = HIWORD(lParam);
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
