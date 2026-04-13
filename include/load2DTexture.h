#ifndef LOAD_2D_TEXTURE_H
#define LOAD_2D_TEXTURE_H

#include <glad/glad.h>
#include <stdio.h>

#include "stb_image.h"

class Gen2DTexture{
    public:
        unsigned int ID; // Texture ID
        Gen2DTexture() : ID(0){}
    
        void loadTexture(char const *path, int sourceColorFormat, int wrapS, int wrapT, int minFilter, int magFilter); // Sets parameters for the texture
        void bindTexture(unsigned int unit);
};

#endif // LOAD_2D_TEXTURE_H!
