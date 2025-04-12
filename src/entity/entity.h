#ifndef ENTITY_CLASS
#define ENTITY_CLASS

// Renderer needs to be included first, as glad.h cannot be included after glfw3.h
#include "../render/renderer.h"
#include "../grid/grid.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class Entity {
public:
    glm::ivec2 prevTilePos, currTilePos;
    int width, height, health;
    Texture texture;
    glm::vec2 pos;
    float speed;

    Entity(const char *texture, const char *textureName, int width, int height, int health);

    glm::ivec2 getTileCoords(Grid *grid);

    void tick(Grid *grid);
    void draw(Renderer *renderer, glm::mat4 view);
};

#endif