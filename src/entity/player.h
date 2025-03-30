#ifndef PLAYER_CLASS
#define PLAYER_CLASS

// Renderer needs to be included first, as glad.h cannot be included after glfw3.h
#include "../render/renderer.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <iostream>

class Player {
public:
    Texture texture;
    glm::vec2 pos;
    float speed, dashSpeed;
    bool dash;

    Player();

    void processInput(GLFWwindow *window, std::chrono::duration<float> deltaTime);

};

#endif