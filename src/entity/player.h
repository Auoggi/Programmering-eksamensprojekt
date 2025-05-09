#ifndef PLAYER_CLASS
#define PLAYER_CLASS

#include "entity.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class Player: public Entity {
public:
    float dashSpeed, maxDashSpeed, minDashSpeed, dashCooldown, dashTimer, dashDuration, stamina, staminaRegenRate;
    bool dash, isDashing;

    Player();

    void tick(GLFWwindow *window, double deltaTime, Grid *grid, Map *map);
    glm::mat4 getView(int width, int height);
    void onCollision(Entity *otherEntity);
};

#endif