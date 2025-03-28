#include "renderer.h"

Renderer::Renderer(Shader &shader) {
    this->shader = shader;
    this->initRenderData();
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &this->VAO);
}

const char *textureVertexCode = 
"#version 330 core\n"
"layout (location = 0) in vec4 vertex;\n"
"out vec2 TexCoords;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main() {\n"
"TexCoords = vertex.zw;\n"
"gl_Position = projection * view * model * vec4(vertex.xy, 0.0, 1.0);\n"
"}\n";

const char *textureFragmentCode = 
"#version 330 core\n"
"in vec2 TexCoords;\n"
"out vec4 color;\n"
"uniform sampler2D image;\n"
"void main() {\n"
"color = texture(image, TexCoords);\n"
"}\n";

Renderer* Renderer::setupRenderer(unsigned int windowWidth, unsigned int windowHeight) {
    Shader shader = ResourceManager::setShader(textureVertexCode, textureFragmentCode, "sprite");
    shader.use();
    glUniform1i(glGetUniformLocation(shader.ID, "image"), 0);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(windowWidth), static_cast<float>(windowHeight), 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, false, glm::value_ptr(projection));

    return new Renderer(shader);
}

void Renderer::drawTexture(Texture &texture, glm::mat4 view, glm::vec2 pos) {
    drawTexture(texture, view, pos, glm::vec2(texture.width, texture.height), 0);
}

void Renderer::drawTexture(Texture &texture, glm::mat4 view, glm::vec2 pos, glm::vec2 size, float rotation) {
    // prepare transformations
    this->shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(pos, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, false, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, false, glm::value_ptr(view));

    glActiveTexture(GL_TEXTURE0);
    texture.bind();

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Renderer::initRenderData() {
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}