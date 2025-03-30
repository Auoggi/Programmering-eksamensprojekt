#ifndef PLAYER_CLASS
#define PLAYER_CLASS

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Player {
public:
    glm::vec2 pos;
    float speed, dashSpeed;
    bool dash;

    Player();

    void processInput(GLFWwindow *window);

};

#endif