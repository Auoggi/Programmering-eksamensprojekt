#ifndef PROJEKTIL_CLASS
#define PROJEKTIL_CLASS

#include "entity.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class Projektil : public Entity {
    Projektil();

    void tick(double deltaTime, Grid *grid);
    void onCollision(Entity *otherEntity);
};

#endif