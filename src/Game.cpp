/**
 * @date 26.12.2020
 * @author Alexander Kaschta
 */
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glad/glad.h"

#include "Game.h"
#include "Scene.h"
#include "scenes/KochCurveScene.h"

/**
 * Windows initialization
 * @param title const char with the title of the image
 * @param width int width of the window. Default is 640
 * @param height int height of the window. Default is 480
 * @param n int value of the Koch curve, default is 0
 */
void Game::Init(const char* title, int width = 640, int height = 480, int n = 0) {

    // Set error callback
    glfwSetErrorCallback(Game::error_callback);

    if (!glfwInit()) {
        std::cout << "GLFW couldn't be initialized!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // High-DPI-support for Windows and X11
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GL_TRUE);

    // Create core context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Link the window to the OpenGL context
    glfwMakeContextCurrent(window);

    // Load glad
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Disable vsync. To activate, set it to 1
    glfwSwapInterval(0);

    if (GLAD_GL_VERSION_4_0) {
        std::cout << "OpenGL 4.0 is supported" << std::endl;
    }

    float highDPIscaleFactor = 1.0;
#ifdef _WIN32
    // If it's a HighDPI monitor, try to scale everything
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    float xscale, yscale;
    glfwGetMonitorContentScale(monitor, &xscale, &yscale);
    if (xscale > 1 || yscale > 1) {
        highDPIscaleFactor = xscale;
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    }
#elif __APPLE__
    // Disable Apple based upscaling
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif

    glfwSetWindowUserPointer(window, this);

    // Set callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(highDPIscaleFactor);

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Create a new scene
    auto* splashScene = new KochCurveScene(n);
    scenes.push_back(splashScene);

    for(auto const& scene: scenes) {
        scene->Init();
    }

}

/**
 * Is the game is running
 * @return bool, if the game is still running
 */
bool Game::isRunning() const {
    return !glfwWindowShouldClose(window);
}

/**
 * Clear everything after rendering up
 */
void Game::Clear() {

    // Clear all scenes
    while (!scenes.empty()) {
        scenes.back()->Clear();
        scenes.pop_back();
    }

    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Destroy the window
    glfwDestroyWindow(window);
    glfwTerminate();
}

/*
 * Calculate delta time between frames
 */
void Game::CalculateTime() {
    double currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

/**
 * Handle all events from the system (window resize, input [keyboard, mouse, controller,...], ...)
 */
void Game::HandleEvents() {

    // Handle scene input
    if (!scenes.empty()) {
        scenes.back()->HandleEvents(this, deltaTime);
    }

    glfwPollEvents();
}

/**
 * Render the window
 */
void Game::Draw() {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(12, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(345,48), ImGuiCond_FirstUseEver);

    // Render the ImGui
    if (debug) {
        ImGui::Begin("Debug");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    if (!scenes.empty()) {
        scenes.back()->ImGuiRender();
    }

    ImGui::Render();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the scene
    if (!scenes.empty()) {
        scenes.back()->Render();
    }

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);

}

/**
 * Update the rendered scene with the delta time
 */
void Game::Update() {
    // Update the scene
    if (!scenes.empty()) {
        scenes.back()->Update(deltaTime);
    }
}

/**
 * Method for error reporting, which prints the error code and the description. Function design is based on the C API.
 * @param error int error code
 * @param description const char* error description
 */
void Game::error_callback(int error, const char *description) {
    std::cerr << "Error(" << error << "): " << description << std::endl;
}

/**
 * Method called when the window is resized. Function call is structured by the C API.
 * @param window pointer to the window instance, that has been changed.
 * @param width int new width of the window
 * @param height int new height of the window
 */
void Game::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

/**
 * Method to handle key inputs
 * @param window pointer to the window instance, where the key has been pressed
 * @param key int code of the key, which has been pressed
 * @param scancode platform specific interpretation of the key code
 * @param action was the key pressed, release or is pressed in a repeating manner
 * @param mode Modifier bits, if additionally NUM_LOCK, SHIFT, CONTROL, ... where pressed.
 */
void Game::key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else if (key == GLFW_KEY_F3 && action == GLFW_RELEASE) {
        Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
        game->debug = !game->debug;
    }
}

/**
 * Method to change the currently rendered scene
 */
void Game::ChangeScene(Scene *scene) {

    if (!scenes.empty()) {
        scenes.pop_back();
    }

    scenes.push_back(scene);

    if (!scenes.empty()) {
        scenes.back()->Init();
    }

}
