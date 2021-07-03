/**
 * @date 30.06.2021
 * @author Alexander Kaschta
 */
#include <cmath>
#include <iostream>

#include "KochCurveScene.h"

#include "imgui.h"
#include "glad/glad.h"

/**
 * Constructor to initialize the Koch curve with another value than the default 0. If bigger 10 or smaller 0, it will be
 * set to fit the bounds.
 * @param n int Koch curve value
 */
KochCurveScene::KochCurveScene(int n) {
    this->n = n;
    if (this->n > 10) {
        this->n = 10;
    } else if (this->n < 0) {
        this->n = 0;
    }
}

/**
 * Method to setup everything needed for rendering
 */
void KochCurveScene::Init() {
    // Create the shaders
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    // Check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    // Check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * data.size(), data.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), (void*) nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Set line width
    glLineWidth(1);
}

/**
 * Render the scene
 */
void KochCurveScene::Render() {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_LOOP, 0, (int) data.size()/2);
}

/**
 * Render the (inefficient) graphical user interface
 */
void KochCurveScene::ImGuiRender() {

    ImGui::SetNextWindowPos(ImVec2(12, 70), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(162,90), ImGuiCond_FirstUseEver);

    ImGui::Begin("Figure");

    ImGui::Text("n:");
    ImGui::SliderInt("", &n, 0, 10);
    ImGui::Text("Points: %zu", data.size());

    ImGui::End();
}

/**
 * Clear the scene, when it's not needed any more.
 */
void KochCurveScene::Clear() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

/**
 * Update the contents before the next draw occurs
 */
void KochCurveScene::Update(double dt) {
    if (current != n) {
        current = n;

        // Define a new vector
        cache = std::vector<double>();

        // Update the vertices
        cache.push_back(-0.5);
        cache.push_back(-0.5);
        generatePoint(-0.5, -0.5, 0.5, -0.5, 0);
        cache.push_back(0.5);
        cache.push_back(-0.5);
        generatePoint(0.5, -0.5, 0.0, 0.5, 0);
        cache.push_back(0.0);
        cache.push_back(0.5);
        generatePoint(0.0, 0.5, -0.5, -0.5, 0);

        // Update data array
        data = cache;
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(double) * data.size(), data.data(), GL_DYNAMIC_DRAW);

    }
}

/**
 * Handle events
 */
void KochCurveScene::HandleEvents(Game* game, double dt) {
    // Do nothing over here ;)
}

/**
 * Calculate new Koch curve points recursively. This is by done by creating new points on the passed line to substitute
 * and substituting the lines between the new created points.
 * @param x_start double x value of the start point
 * @param y_start double y value of the start point
 * @param x_end double x value of the end point
 * @param y_end double y value of the end point
 * @param depth int with the current substitution depth
 */
void KochCurveScene::generatePoint(double x_start, double y_start, double x_end, double y_end, int depth = 0) {

    if (current > depth && depth >= 0 && current >= 0) {
        // Generate additional points
        double x_diff = x_end - x_start;
        double y_diff = y_end - y_start;

        double length = std::sqrt(std::pow(x_diff/3.0, 2) + std::pow(y_diff/3.0, 2));
        double height = std::sqrt(3) * length * 0.5;

        double x_norm = -(x_diff/3.0)/length;
        double y_norm = -(y_diff/3.0)/length;

        double center_x = 0.5 * x_diff + x_start;
        double center_y = 0.5 * y_diff + y_start;

        generatePoint(x_start, y_start, x_diff/3.0 + x_start, y_diff/3.0 + y_start, depth + 1);

        // First point
        cache.push_back(x_diff/3.0 + x_start);
        cache.push_back(y_diff/3.0 + y_start);

        generatePoint(x_diff/3.0 + x_start, y_diff/3.0 + y_start, center_x - y_norm * height, center_y + x_norm * height, depth + 1);

        // Second point
        cache.push_back(center_x - y_norm * height);
        cache.push_back(center_y + x_norm * height);

        generatePoint(center_x - y_norm * height, center_y + x_norm * height, (2/3.0) * x_diff + x_start, (2/3.0) * y_diff + y_start, depth + 1);

        // Third point
        cache.push_back((2/3.0) * x_diff + x_start);
        cache.push_back((2/3.0) * y_diff + y_start);

        generatePoint((2/3.0) * x_diff + x_start, (2/3.0) * y_diff + y_start, x_end, y_end, depth + 1);

    }

}