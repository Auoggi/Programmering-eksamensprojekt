#ifndef MAP_CLASS
#define MAP_CLASS

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../render/resource_manager.h"

class Map {
public:
    GLuint VAO;
    int tileSize;
    Shader shader;
    Texture tilemap;
    Texture map_data;

    Map(int tileSize, float screenWidth, float screenHeight, int mapWidth, int mapHeight, int *map);

    void draw(glm::mat4 view);
};

#endif