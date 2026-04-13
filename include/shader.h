#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{
    public:
        unsigned int ID; // Program ID
        
        Shader(const char *vertexPath, const char *fragmentPath);
        void use(); // Activates the shader program

        unsigned int getShaderID(); // Return the shader ID

        // Utility uniform functions
        void setBool (const std::string name, bool  value) const;
        void setInt  (const std::string name, int   value) const;
        void setFloat(const std::string name, float value) const;

        void setVec3 (const std::string name, glm::vec3 value) const;
};

#endif // !SHADER_H
