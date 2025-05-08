#include "enemy.h"

Enemy::Enemy(const char *image, const char *name, int width, int height) : Entity(image, name, width, height, 100) {}

void Enemy::pathfinding(Entity *target) {
    // it just move towards player now
    this->velocity = glm::normalize(-glm::vec2(this->pos.x - target->pos.x, this->pos.y - target->pos.y)) * this->speed;
}