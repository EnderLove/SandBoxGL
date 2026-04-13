#include "load2DTexture.h"

void Gen2DTexture::loadTexture(char const *path, int sourceColorFormat, int wrapS, int wrapT, int minFilter, int magFilter){
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    int width, height, nrChannels;
    unsigned char *texBoxData = stbi_load(path, &width, &height, &nrChannels, 0);
    if (texBoxData){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, sourceColorFormat, GL_UNSIGNED_BYTE, texBoxData);
        glGenerateMipmap(GL_TEXTURE_2D);
        printf("TEXTURE LOADED: %s\n", path);
    } else {
        printf("FAILED TO LOAD TEXTURE: %s\n", path);
    }
    stbi_image_free(texBoxData);
}

void Gen2DTexture::bindTexture(unsigned int unit){
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID);
}
