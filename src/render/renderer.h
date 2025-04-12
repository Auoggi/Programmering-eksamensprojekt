#ifndef RENDERER_CLASS
#define RENDERER_CLASS

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "resource_manager.h"
#include "texture.h"
#include "shader.h"

class Renderer {
public:
    Renderer(Shader &shader);
    ~Renderer(); // Destructor, is called when class is destroyed / deleted

    // Create a new renderer suitable for rendering simple textures
    static Renderer* setupRenderer(unsigned int windowWidth, unsigned int windowHeight);

    void drawTexture(Texture &texture, glm::mat4 view, glm::vec2 pos);
    void drawTexture(Texture &texture, glm::mat4 view, glm::vec2 pos, glm::vec2 size, float rotation);
private:
    Shader shader;
    unsigned int VAO;

    void initRenderData();
};

#endif