#ifndef SHADER_CLASS
#define SHADER_CLASS

#include <glad/glad.h>

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

#endif