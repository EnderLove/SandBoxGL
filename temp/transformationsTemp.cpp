#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"

const int SCR_SOURCE_WIDTH  = 1920;
const int SCR_SOURCE_HEIGHT = 1080;

const int SCR_WIDTH  = 16 * 80;
const int SCR_HEIGHT =  9 * 90;

void frameBufferSizeCallback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
    printf("RESIZE: %4d | %4d\n", width, height);
}

void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void processColorScreen(GLFWwindow *window, float *r, float *g, float *b){
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) { *r += 0.01f; if (*r > 1) *r = 0; }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) { *g += 0.01f; if (*g > 1) *g = 0; }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) { *b += 0.01f; if (*b > 1) *b = 0; }
}

void processAlphaBlend(GLFWwindow *window, float *alphaBlendVal){
    if (glfwGetKey(window, GLFW_KEY_UP)   == GLFW_PRESS) { *alphaBlendVal += 0.01f; if (*alphaBlendVal > 2) *alphaBlendVal = 0; }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) { *alphaBlendVal -= 0.01f; if (*alphaBlendVal < 0) *alphaBlendVal = 2; }
}

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core-profile setting
  

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MINECRAFT KILLER (DLSS 5)", NULL, NULL);
    if (window == NULL){
        printf("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    //glfwSetWindowPos(window, SCR_SOURCE_WIDTH / 4, SCR_SOURCE_HEIGHT / 4); // TODO : Set the window at the center 
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        printf("Failed to initialize GLAD");
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    printf("Maximun number of vertex attributes supported: %d\n", nrAttributes);
   
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
  
    float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePosition[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    float twoTriangleVertices[] = {
         0.5f,  0.5f, 0.0f,
         1.0f, -0.5f, 0.0f,
         0.0f, -0.5f, 0.0f,
         0.0f, -0.5f, 0.0f,
        -1.0f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    unsigned int twoTriagIndices[] = {
        0, 1, 2,
        2, 3, 4
    };

    float triangle_1[] = {
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    float triangle_2[] = {
         0.0f, -0.5f, 0.0f,
        -1.0f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    unsigned int triangleIndex[] = { 0, 1, 2 };

    float squareVertices[] = {
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
    };

    unsigned int squareIndices[] = {
        0, 1, 3,
        1, 2, 3 
    };

    float texCoords[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.5f, 1.0f
    };

    // TEXTURE CREATION ///////////////////////////////////////////////////////////////////////////////////////////
    unsigned int boxTexture;
    glGenTextures(1, &boxTexture);
    glBindTexture(GL_TEXTURE_2D, boxTexture);

    // SETTING THE WRAPPING/FILTERING OPTIONS
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_set_flip_vertically_on_load(true); // FLIP Y VALUE
    // LOAD AND GENERATE THE TEXTURE
    int width, height, nrChannels;
    unsigned char *texBoxData = stbi_load("../resources/textures/container.jpg", &width, &height, &nrChannels, 0);
    if (texBoxData){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texBoxData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("FAILED TO LOAD TEXTURE\n");
    }
    stbi_image_free(texBoxData);

    unsigned int faceTexture;
    glGenTextures(1, &faceTexture);
    glBindTexture(GL_TEXTURE_2D, faceTexture);

    // SETTING THE WRAPPING/FILTERING OPTIONS
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
 
    unsigned char *texFaceData = stbi_load("../resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (texFaceData){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texFaceData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("FAILED TO LOAD TEXTURE\n");
    }
    stbi_image_free(texFaceData);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // FIRST WE CREATE THE VERTEX ARRAY OBJECT THAT WILL STORE ALL THE SETTINGS
    unsigned int VAO[2], VBO[2], EBO[2]; 
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, EBO);
    glGenBuffers(2, VBO);

    for (int i = 0; i < 2; i++){
        glBindVertexArray(VAO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_DYNAMIC_DRAW);
    }

    Shader interpolShader   ("../shaders/interpolationVertex.vert"    , "../shaders/interpolationFragment.frag");
    Shader uniformTestShader("../shaders/testUniformVertexShader.vert", "../shaders/testUniformFragmentShader.frag");
    Shader upsideDownShader ("../shaders/upsideDownVertexShader.vert" , "../shaders/testUniformFragmentShader.frag");
    Shader IOVertexShader   ("../shaders/outPosVertexShader.vert"     , "../shaders/inPosFragmentShader.frag"); // Send the vertex pos to the fragmenColor
    Shader textureShader    ("../shaders/squareVertexShader.vert"     , "../shaders/squareFragmentShader.frag");
    //Shader myShader("../shaders/vertexTemp.vs", "../shaders/fragmentTemp.fs");

    float r = 0; float g = 0; float b = 0;
    float alphaBlendVal = 0;

    textureShader.use();
    textureShader.setInt("texture1", 0);
    textureShader.setInt("texture2", 1);

    //glm::mat4 trans = glm::mat4(1.0f);
    
    //trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //trans = glm::scale (trans, glm::vec3(0.5f, 0.5f, 0.5f));

    while (!glfwWindowShouldClose(window)){
        processInput(window);
        processColorScreen(window, &r, &g, &b);
        processAlphaBlend(window, &alphaBlendVal);
       
        glClearColor(r, g, b, 1.0f);  // This functions is a state-setting func for "glClear()"
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // State-using function 
        glClear(GL_COLOR_BUFFER_BIT); 
        glClear(GL_DEPTH_BUFFER_BIT); 

        float timeValue = glfwGetTime();
        
        textureShader.use();

        /*
        float xOffset = (sin(timeValue) / 2) + 0.5f;
        int offsetVertexLocation = glGetUniformLocation(textureShader.getShaderID(), "xOffset");
        glUniform1f(offsetVertexLocation, xOffset);


        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, timeValue, glm::vec3(0.0f, 0.0f, 1.0f));

        unsigned int transVertexLocation = glGetUniformLocation(textureShader.getShaderID(), "transform");
        glUniformMatrix4fv(transVertexLocation, 1, GL_FALSE, glm::value_ptr(trans));
        */
        int alphaBlendFragLocation = glGetUniformLocation(textureShader.getShaderID(), "alphaBlend");
        glUniform1f(alphaBlendFragLocation, alphaBlendVal);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, boxTexture); 
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, faceTexture);
      
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, timeValue, glm::vec3(0.5f, 1.0f, 0.0f));
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
        unsigned int modelLoc = glGetUniformLocation(textureShader.getShaderID(), "model");
        unsigned int viewLoc  = glGetUniformLocation(textureShader.getShaderID(), "view");
        unsigned int projLoc  = glGetUniformLocation(textureShader.getShaderID(), "projection");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc , 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc , 1, GL_FALSE, glm::value_ptr(projection));
      
        glBindVertexArray(VAO[0]);

        for (unsigned int i = 0; i < 10; i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePosition[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle + timeValue * 10), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
//glDrawArrays(GL_TRIANGLES, 0, 3);
