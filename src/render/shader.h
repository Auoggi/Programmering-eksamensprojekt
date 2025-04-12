#ifndef SHADER_CLASS
#define SHADER_CLASS

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>

class Shader {
public:
    unsigned int ID;

    Shader() {}

    void use();
    void compile(const char *vertexCode, int vertexSize, const char *fragmentCode, int fragmentSize);

    void setFloat(const char *name, float value);
    void setInteger(const char *name, int value);
    void setVector2f(const char *name, float x, float y);
    void setMatrix4(const char *name, glm::mat4 &matrix);
private:
    void checkCompileErrors(unsigned int object, const char *type);
};

#endif