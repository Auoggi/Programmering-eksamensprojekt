#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>

class Shader {
public:
    unsigned int ID;

    Shader() {}

    void use();
    void compile(const char *vertexCode, const char *fragmentCode);
private:
    void checkCompileErrors(unsigned int object, const char *type);
};