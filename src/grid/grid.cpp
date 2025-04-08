#include "grid.h"

Grid::Grid(int tileSize, float screenWidth, float screenHeight) : tileSize(tileSize), shader(ResourceManager::setShader(GRID_VERTEX_GLSL, GRID_FRAGMENT_GLSL, "grid")) {
    float vertices[] = {
        0, 0,  
        screenWidth, 0,  
        0, screenHeight,  
        screenWidth, screenHeight
    };

    GLuint VBO;
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Grid::draw(glm::mat4 view, int screenWidth, int screenHeight, int centerTileX, int centerTileY) {
    this->shader.use();

    glUniformMatrix4fv(glGetUniformLocation(this->shader.ID, "view"), 1, false, glm::value_ptr(view));
    glUniform1f(glGetUniformLocation(this->shader.ID, "tileSize"), 64.f);
    glUniform2f(glGetUniformLocation(this->shader.ID, "screenSize"), screenWidth, screenHeight);
    glUniform2f(glGetUniformLocation(this->shader.ID, "centerTile"), centerTileX, centerTileY);
    
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}