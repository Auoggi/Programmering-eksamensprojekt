#include "enemy.h"

Enemy::Enemy(const char *image, const char *name, int width, int height) : Entity(image, name, width, height, 100) {
    this->speed = 250;
}

void Enemy::tick(Entity *target, double deltaTime, Grid *grid, Map *map) {
    glm::vec2 targetVelocity;

    if(this->distanceTo(target) < 256) {
        glm::vec2 direction = glm::normalize(-glm::vec2(this->pos.x - target->pos.x, this->pos.y - target->pos.y));
        targetVelocity = glm::normalize(direction) * this->speed;
    } else {
        targetVelocity = glm::vec2(0, 0);
    }
    

    // Accelerate toward desired velocity
    glm::vec2 deltaVelocity = targetVelocity - this->velocity;

    // Accelerate if velocity is not the desired velocity
    if (glm::length(deltaVelocity) != 0) {
        glm::vec2 accelStep = glm::normalize(deltaVelocity) * this->acceleration;

        // Clamp the acceleration step to prevent overshoot
        if (glm::length(accelStep) > glm::length(deltaVelocity)) {
            accelStep = deltaVelocity;
        }

        this->velocity += accelStep;    
    }

    // Call parrent tick function
    Entity::tick(grid, map, deltaTime);
}

void Enemy::onCollision(Entity *otherEntity) {
    
}