#include "map.h"

Map::Map(int tileSize, float screenWidth, float screenHeight, int mapWidth, int mapHeight, int *map) : tileSize(tileSize), shader(ResourceManager::setShader(FULLSCREEN_VERTEX_GLSL, MAP_FRAGMENT_GLSL, "map")), tilemap(ResourceManager::loadTexture("assets/textures/tilemap.png", "tilemap")) {
    // the tilemap is an 8x8 grid of 64x64 textures for use in rendering the map
    int tilemapColumnCount = 8, tilemapRowCount = 8;

    shader.use();
    // Map is sent to shader as a texture, and should therefore have a different value from image
    shader.setInteger("map", 1);
    shader.setInteger("image", 0);
    shader.setInteger("tileSize", tileSize);
    shader.setVector2f("screenSize", screenWidth, screenHeight);

    shader.setInteger("columnCount", tilemapColumnCount);
    shader.setInteger("rowCount", tilemapRowCount);

    GLubyte data[mapWidth * mapHeight * 4]; // RGBA

    for(int i = 0; i < mapWidth * mapHeight; i++) {
        data[i * 4] = (GLubyte) map[i];
        data[i * 4 + 1] = 0;
        data[i * 4 + 2] = 0;
        data[i * 4 + 3] = 0;
    }

    this->map_data.generate(mapWidth, mapHeight, data);

    float vertices[] = {
        0, 0,  
        screenWidth, 0,  
        0, screenHeight,  
        screenWidth, screenHeight
    };

    // Create VAO and VBO
    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Send VAO and VBO to the gpu, for use in rendering
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Map::draw(glm::mat4 view) {
    this->shader.use();
    this->shader.setMatrix4("view", view);

    // Bind tilemap to texture0
    this->tilemap.bind(GL_TEXTURE0);

    // Bind map to texture1
    this->map_data.bind(GL_TEXTURE1);

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}