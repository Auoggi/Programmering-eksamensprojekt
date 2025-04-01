#ifndef ENTITY_CLASS
#define ENTITY_CLASS

// Renderer needs to be included first, as glad.h cannot be included after glfw3.h
#include "../render/renderer.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class Entity {
public:
    int width, height;
    Texture texture;
    glm::vec2 pos;
    float speed;

    Entity(const char *texture, const char *textureName, int width, int height);

    void draw(Renderer *renderer, glm::mat4 view);
};

#endif