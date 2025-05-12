#ifndef PROJEKTIL_CLASS
#define PROJEKTIL_CLASS

#include "entity.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class Projektil : public Entity {
public:
    virtual const char* getType() const {return "projectile";}

    Projektil(glm::vec2 startPos, glm::vec2 targetPos);

    void tick(Grid *grid, double deltaTime);
    void onCollision(Entity *otherEntity) override;
};

#endif