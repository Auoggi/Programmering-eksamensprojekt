#ifndef TEXTURE_CLASS
#define TEXTURE_CLASS

#include <glad/glad.h>

class Texture {
public:
    unsigned int ID;
    unsigned int width, height;

    Texture();

    void generate(unsigned int width, unsigned int height, unsigned char *data);
    void bind();
};

#endif