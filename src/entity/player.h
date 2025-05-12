#ifndef PLAYER_CLASS
#define PLAYER_CLASS

#include "entity.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

// Forward declaration of Projektil, as to avoid circular include
class Projektil;

class Player: public Entity {
public:
    float dashSpeed, maxDashSpeed, minDashSpeed, dashCooldown, dashTimer, dashDuration, stamina, staminaRegenRate;
    bool dash, isDashing;

    virtual const char* getType() const {return "player";}

    Player();

    void tick(GLFWwindow *window, glm::mat4 view, double deltaTime, Grid *grid, std::vector<Entity*> *entityList);
    glm::mat4 getView(int width, int height);
    void onCollision(Entity *otherEntity) override;
};

#endif