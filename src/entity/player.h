#ifndef PLAYER_CLASS
#define PLAYER_CLASS

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>

class Player {
public:
    glm::vec2 pos;
    float speed, dashSpeed;
    bool dash;

    Player();

    void processInput(GLFWwindow *window, std::chrono::duration<float> deltaTime);

};

#endif