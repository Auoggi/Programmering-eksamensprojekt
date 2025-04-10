#ifndef PLAYER_CLASS
#define PLAYER_CLASS

#include "entity.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class Player: public Entity {
public:
    float dashSpeed, dashCooldown, dashTimer, dashDuration, dashSpeedChange;
    bool dash, isDashing;

    Player();

    void processInput(GLFWwindow *window, double deltaTime);
    glm::mat4 getView(int width, int height);
};

#endif