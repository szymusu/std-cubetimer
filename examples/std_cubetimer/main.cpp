// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "../../imgui.h"
#include "../../backends/imgui_impl_glfw.h"
#include "../../backends/imgui_impl_opengl3.h"
#include <cstdio>
#include <iostream>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "time/Timer.h"
#include "cube/Cube.h"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

//void drawCube() {
//    GLfloat vertices[] = {
//            -1, -1, -1,   -1, -1,  1,   -1,  1,  1,   -1,  1, -1,
//            1, -1, -1,    1, -1,  1,    1,  1,  1,    1,  1, -1,
//            -1, -1, -1,   -1, -1,  1,    1, -1,  1,    1, -1, -1,
//            -1,  1, -1,   -1,  1,  1,    1,  1,  1,    1,  1, -1,
//            -1, -1, -1,   -1,  1, -1,    1,  1, -1,    1, -1, -1,
//            -1, -1,  1,   -1,  1,  1,    1,  1,  1,    1, -1,  1
//    };
//
//    GLfloat colors[] = {
//            0, 0, 0,   0, 0, 1,   0, 1, 1,   0, 1, 0,
//            1, 0, 0,   1, 0, 1,   1, 1, 1,   1, 1, 0,
//            0, 0, 0,   0, 0, 1,   1, 0, 1,   1, 0, 0,
//            0, 1, 0,   0, 1, 1,   1, 1, 1,   1, 1, 0,
//            0, 0, 0,   0, 1, 0,   1, 1, 0,   1, 0, 0,
//            0, 0, 1,   0, 1, 1,   1, 1, 1,   1, 0, 1
//    };
//
//    static float alpha = 0;
//    //attempt to rotate cube
//    glRotatef(alpha, 0, 1, 0);
//
//    /* We have a color array and a vertex array */
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glEnableClientState(GL_COLOR_ARRAY);
//    glVertexPointer(3, GL_FLOAT, 0, vertices);
//    glColorPointer(3, GL_FLOAT, 0, colors);
//
//    /* Send data : 24 vertices */
//    glDrawArrays(GL_QUADS, 0, 24);
//
//    /* Cleanup states */
//    glDisableClientState(GL_COLOR_ARRAY);
//    glDisableClientState(GL_VERTEX_ARRAY);
//    alpha += 1;
//}
//
//static void draw_callback(const ImDrawList* parent_list, const ImDrawCmd* cmd) {
//    glViewport(0, 0, 100, 300);
//
//    // Draw stuff
//    glClearColor(0.0, 0.8, 0.3, .3);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    glMatrixMode(GL_PROJECTION_MATRIX);
//    glLoadIdentity();
////    gluPerspective(60, 500.0 / 300.0, 0.1, 100);
//
//    glMatrixMode(GL_MODELVIEW_MATRIX);
////    glTranslatef(0,0,-5);
//
//    drawCube();
//}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "std::cubetimer", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwMaximizeWindow(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    ImFont* droidSansNormal = io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 20.0f);
    ImFont* droidSansMedium = io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 30.0f);
    ImFont* droidSansHuge = io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 300.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    io.FontDefault = droidSansNormal;
    CubeTimer cubeTimer;
    cubeTimer.timeFont = droidSansHuge;
    cubeTimer.secondaryFont = droidSansMedium;

//    Cube cube;
//    std::cout << "Initial state: " << cube.isSolved() << std::endl;

//    cube.makeMove({F, NONE});
//    cube.makeMove({B, NONE});
//    cube.makeMove({L, NONE});
//    cube.makeMove({U, TWO});
//    cube.makeMove({R, NONE});
//    cube.makeMove({F, PRIME});
//    cube.makeMove({B, PRIME});
//    cube.makeMove({L, NONE});
//    cube.makeMove({D, TWO});
//    cube.makeMove({R, TWO});
//    cube.makeMove({B, NONE});
//    cube.makeMove({L, TWO});
//    cube.makeMove({F, NONE});
//    cube.makeMove({D, TWO});
//    cube.makeMove({B, NONE});
//    cube.makeMove({R, TWO});
//    cube.makeMove({B, NONE});
//    cube.makeMove({D, TWO});
//    cube.makeMove({R, TWO});
//    cube.makeMove({F, PRIME});
//    //F B L U2 R F' B' L D2 R2 B L2 F D2 B R2 B D2 R2 F'
//
//    std::cout << cube.isSolved() << std::endl;
//
//    cube.makeMove({U, TWO});
//    cube.makeMove({L, TWO});
//    cube.makeMove({U, TWO});
//    cube.makeMove({F, TWO});
//    cube.makeMove({L, TWO});
//    cube.makeMove({D, TWO});
//    cube.makeMove({B, PRIME});
//    cube.makeMove({R, PRIME});
//    cube.makeMove({L, PRIME});
//    cube.makeMove({B, PRIME});
//    cube.makeMove({L, TWO});
//    cube.makeMove({U, NONE});
//    cube.makeMove({B, TWO});
//    cube.makeMove({U, PRIME});
//    cube.makeMove({R, TWO});
//    cube.makeMove({U, NONE});
//    cube.makeMove({R, TWO});
//    cube.makeMove({B, TWO});
//    cube.makeMove({R, TWO});
//    cube.makeMove({U, NONE});
//
//    std::cout << cube.isSolved() << std::endl;

//    //R U R' U' R' F R2 U' R' U' R U R' F'
//    cube.makeMove({R, NONE});
//    cube.makeMove({U, NONE});
//    cube.makeMove({R, PRIME});
//    cube.makeMove({U, PRIME});
//    cube.makeMove({R, PRIME});
//    cube.makeMove({F, NONE});
//    cube.makeMove({R, TWO});
//    cube.makeMove({U, PRIME});
//    cube.makeMove({R, PRIME});
//    cube.makeMove({U, PRIME});
//    cube.makeMove({R, NONE});
//    cube.makeMove({U, NONE});
//    cube.makeMove({R, PRIME});
//    cube.makeMove({F, PRIME});
//    std::cout << "T perm 1 " << cube.isSolved() << std::endl;
//
//    cube.makeMove({R, NONE});
//    cube.makeMove({U, NONE});
//    cube.makeMove({R, PRIME});
//    cube.makeMove({U, PRIME});
//    cube.makeMove({R, PRIME});
//    cube.makeMove({F, NONE});
//    cube.makeMove({R, TWO});
//    cube.makeMove({U, PRIME});
//    cube.makeMove({R, PRIME});
//    cube.makeMove({U, PRIME});
//    cube.makeMove({R, NONE});
//    cube.makeMove({U, NONE});
//    cube.makeMove({R, PRIME});
//    cube.makeMove({F, PRIME});
//    std::cout << "T perm 2 " << cube.isSolved() << std::endl;

//    cube.makeMove({F, TWO});
//    cube.makeMove({F, PRIME});
//    cube.makeMove({F, PRIME});
//    std::cout << "R2 R' R': " << cube.isSolved() << std::endl;

//    cube.makeMove({R, NONE});
//    cube.makeMove({U, TWO});
//    cube.makeMove({L, PRIME});
//    std::cout << cube.isSolved() << std::endl;
//    cube.makeMove({L, NONE});
//    cube.makeMove({U, TWO});
//    cube.makeMove({R, PRIME});
//    std::cout << "chuj" << cube.isSolved() << std::endl;


//    for (int i = 1; i <= 110; ++i) {
//        cube.makeMove({R, PRIME});
//        cube.makeMove({U, PRIME});
//        if (cube.isSolved()) {
//            std::cout << "Solved in " << i << std::endl;
//            break;
//        }
//    }


    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        cubeTimer.renderFrame();

//        bool show_another_window = true;
//        ImGui::SetNextWindowSize(ImVec2(500, 300), 4);
//        ImGui::Begin("Another Window", &show_another_window);
//
//        ImGui::GetWindowDrawList()->AddCallback(draw_callback, NULL);

//        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    	
        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
