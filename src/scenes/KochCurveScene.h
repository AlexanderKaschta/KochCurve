/**
 * @date 30.06.2021
 * @author Alexander Kaschta
 */
#ifndef GAME_MENUSCENE_H
#define GAME_MENUSCENE_H

#include <vector>
#include "../Scene.h"

class KochCurveScene: public Scene {
public:

    // Two constructors
    KochCurveScene() = default;
    explicit KochCurveScene(int n);

    void Init() override;
    void Render() override;
    void ImGuiRender() override;
    void Clear() override;
    void Update(double dt) override;
    void HandleEvents(Game* game, double dt) override;

private:

    // Number for the user interface
    int n = 0;

    // Number of the current status
    int current = 0;

    // Data caches
    std::vector<double> data = {-0.5, -0.5, 0.5, -0.5, 0.0, 0.5};
    std::vector<double> cache;

    unsigned int VBO = 0, VAO = 0;
    int shaderProgram = 0;
    int fragmentShader = 0;
    int vertexShader = 0;

    // Shader code
    const char *vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                     "}\0";
    const char *fragmentShaderSource = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                       "}\n\0";

    void generatePoint(double x_start, double y_start, double x_end, double y_end, int depth);

};


#endif //GAME_MENUSCENE_H
