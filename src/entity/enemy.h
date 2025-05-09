#ifndef ENEMY_CLASS
#define ENEMY_CLASS

#include "entity.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class Enemy: public Entity {
public:
    Enemy(const char *image, const char *name, int witdh, int heigt);

    void tick(Entity *target, double deltaTime, Grid *grid);
    void onCollision(Entity *otherEntity);
};

#endif