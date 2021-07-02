/**
 * @date 26.12.2020
 * @author Alexander Kaschta
 */
#ifndef GAME_GAME_H
#define GAME_GAME_H
#include <vector>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

class Scene;

class Game {
public:
    Game() = default;

    void Init(const char* title, int width, int height, int n);
    void Clear();

    void ChangeScene(Scene* scene);

    void CalculateTime();
    void HandleEvents();
    void Update();
    void Draw();

    [[nodiscard]] bool isRunning() const;

private:

    static void error_callback(int error, const char* description);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

    std::vector<Scene*> scenes;

    GLFWwindow* window{};

    bool debug = true;

    double deltaTime = 0.0f;
    double lastFrame = 0.0f;
};

#endif //GAME_GAME_H
