#ifndef GRID_CLASS
#define GRID_CLASS

#include "../entity/entity.h"

#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class Grid {
public:
    const int tileSize;
    std::map<glm::ivec2, std::vector<Entity>> entityMap;
    Shader shader;
    GLuint VAO;

    Grid(const int tileSize, float screenWidth, float screenHeight);
    glm::ivec2 getTileCoords(Entity entity);
    void draw(glm::mat4 view, int screenWidth, int screenHeight, int centerTileX, int centerTileY);
};

#endif