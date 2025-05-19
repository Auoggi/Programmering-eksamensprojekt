#ifndef GRID_CLASS
#define GRID_CLASS

#include "../render/shader.h"

#include <map>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

// Forward declaration of Entity, as to avoid circular include
class Entity;

class Grid {
public:
    const int tileSize;

    struct ivec2Compare {
        bool operator()(const glm::ivec2& a, const glm::ivec2& b) const {
            return a.x != b.x ? a.x < b.x : a.y < b.y;
        }
    };

    // shared_ptr is used instead of raw pointer to automate new and delete for vectors in the map
    std::map<glm::ivec2, std::shared_ptr<std::vector<Entity*>>, ivec2Compare> entityMap;
    std::shared_ptr<std::vector<Entity*>> getEntityList(const glm::ivec2& pos);

    Shader shader;
    GLuint VAO; // OpenGL unsigned integer

    Grid(const int tileSize, float screenWidth, float screenHeight);
    void draw(glm::mat4 view, int centerTileX, int centerTileY);
    void removeEntity(const glm::ivec2 &pos, Entity *entity);
};

#endif