#ifndef ENEMY_CLASS
#define ENEMY_CLASS

#include "entity.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

// Forward declaration of Projectile, as to avoid circular include
class Projectile;

class Enemy: public Entity {
public:
    virtual const char* getType() const {return "enemy";};

    Enemy(const char *image, const char *name, int witdh, int heigt);

    void tick(Entity *target, double deltaTime, Grid *grid);
    void onCollision(Entity *otherEntity) override;
};

#endif