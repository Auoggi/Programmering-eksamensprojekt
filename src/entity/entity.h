#ifndef ENTITY_CLASS
#define ENTITY_CLASS

// Renderer needs to be included first, as glad.h cannot be included after glfw3.h
#include "../render/renderer.h"
#include "../grid/grid.h"

#include <cstring>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class Entity {
public:
    float speed, acceleration, deacceleration;
    glm::ivec2 prevTilePos, currTilePos;
    int width, height, health;
    glm::vec2 pos, velocity;
    Texture texture;

    virtual const char* getType() const {return "";} 

    Entity(const char *texture, const char *textureName, int width, int height, int health);

    void handleCollision(Grid *grid);
    glm::ivec2 getTileCoords(Grid *grid);
    float distanceTo(Entity *otherEntity); 
    void tick(Grid *grid, double deltaTime);
    bool collisionDetection(Entity *entity);
    void draw(Renderer *renderer, glm::mat4 view);
    virtual void onCollision(Entity *otherEntity);
};

#endif