#include "grid.h"
#include "../entity/entity.h" // Import Entity to define Entity class after the forward declaration

Grid::Grid(int tileSize, float screenWidth, float screenHeight) : tileSize(tileSize), shader(ResourceManager::setShader(GRID_VERTEX_GLSL, GRID_FRAGMENT_GLSL, "grid")) {
    // Define fullscreen VAO, to let the fragment shader draw on the entiry screen
    float vertices[] = {
        0, 0,  
        screenWidth, 0,  
        0, screenHeight,  
        screenWidth, screenHeight
    };

    
    // Create VAO and VBO
    GLuint VBO;
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    // Send VAO and VBO to the gpu, for use in rendering
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

std::shared_ptr<std::vector<Entity*>> Grid::getEntityList(const glm::ivec2& pos) {
    // Check if entityMap contains pos, returns true if it does not contain pos
    if(entityMap.find(pos) == entityMap.end()) {
        entityMap[pos] = std::make_shared<std::vector<Entity*>>(); // Create new vector at pos in entityMap
    }
    return entityMap[pos];
}

void Grid::draw(glm::mat4 view, int screenWidth, int screenHeight, int centerTileX, int centerTileY) {
    this->shader.use();

    this->shader.setMatrix4("view", view);
    this->shader.setFloat("tileSize", this->tileSize);
    this->shader.setVector2f("screenSize", screenWidth, screenHeight);
    this->shader.setVector2f("centerTile", centerTileX, centerTileY);
    
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}