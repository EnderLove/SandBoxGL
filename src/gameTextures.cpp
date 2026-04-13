#define STB_IMAGE_IMPLEMENTATION
#include "load2DTexture.h"

// TEXTURE CREATION ///////////////////////////////////////////////////////////////////////////////////////////
class LoadTextures{
    public:
        Gen2DTexture woodBoxContainer;
        Gen2DTexture smileyYellowFace;
        Gen2DTexture floor;
        Gen2DTexture pixelRedEye;
        Gen2DTexture metal;
        Gen2DTexture container2;
        Gen2DTexture container2Spec;
        Gen2DTexture container2WoodEmission;
        Gen2DTexture matrix;

        void loadAll(){
            woodBoxContainer.loadTexture("../resources/textures/container.jpg", GL_RGB, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
            pixelRedEye.loadTexture     ("../resources/textures/pixelArtEye.png", GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
            floor.loadTexture           ("../resources/textures/wall.jpg", GL_RGB, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
            metal.loadTexture           ("../resources/textures/wall.jpg", GL_RGB, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
            container2.loadTexture      ("../resources/textures/container2.png", GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
            container2Spec.loadTexture  ("../resources/textures/container2_specular.png", GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
            matrix.loadTexture          ("../resources/textures/matrix.jpg", GL_RGB, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
            container2WoodEmission.loadTexture("../resources/textures/container2_emission.png", GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
            
        }
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
