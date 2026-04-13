#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <math.h>

#include "camera.h"
#include "shader.h"

#include "load2DTexture.h"
#include "gameTextures.cpp"

const int SCR_SOURCE_WIDTH  = 1920;
const int SCR_SOURCE_HEIGHT = 1080;

const int SCR_WIDTH  = 16 * 80;
const int SCR_HEIGHT =  9 * 90;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); 

float fov = 45.0f;
float zoom = 45.0f;

double yaw = -90.0f;
double pitch = 0.0f;
float lastX = (float)SCR_WIDTH  / 2;
float lastY = (float)SCR_HEIGHT / 2;
bool firstMouse = false;

void frameBufferSizeCallback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
    printf("RESIZE: %4d | %4d\n", width, height);
}

void cursorCallBack(GLFWwindow *window, double xPos, double yPos){
    if (firstMouse){
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    const float sensitivity = 0.1f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;
   
    yaw   += xOffset;
    pitch += yOffset;

    if (pitch >  89.0f) pitch =  89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
};

void scrollCallback(GLFWwindow *window, double xOffset, double yOffset){
    fov -= (float)yOffset;
    if (fov <=  1.0f) fov =  1.0f;
    if (fov >= 45.0f) fov = 45.0f;
}

void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void processCameraInput(GLFWwindow *window, glm::vec3 &cPos, glm::vec3 &cUp, glm::vec3 &cFront, float deltaTime){
    const float cameraSpeed = deltaTime * 5.0f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { cPos += cameraSpeed * cFront; }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { cPos -= cameraSpeed * cFront; }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { cPos -= glm::normalize(glm::cross(cFront, cUp)) * cameraSpeed; }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { cPos += glm::normalize(glm::cross(cFront, cUp)) * cameraSpeed; }
}

// TODO: ADD CONTROLLER HEADER FILE || INPUT HEADER FILE
void processCameraInputController(GLFWwindow *window, glm::vec3 &cPos, glm::vec3 &cUp, glm::vec3 &cFront, float deltaTime){

    int axesCount;
    const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
    
    printf("LEFT STICK X AXIS %f\n", axes[0]);
    printf("LEFT STICK Y AXIS %f\n", axes[1]);
    printf("RIGHT STICK X AXIS %f\n", axes[2]);
    printf("RIGHT STICK Y AXIS %f\n", axes[4]);

    const float cameraSpeed = deltaTime * 5.0f;
    
    if (axes[1] > 0.5f) cPos -= (cameraSpeed * axes[1]) * cFront; 
    if (axes[1] < 0.5f) cPos += (cameraSpeed * abs(axes[1])) * cFront; 
    if (axes[0] > 0.5f) cPos += glm::normalize(glm::cross(cFront, cUp)) * (cameraSpeed * axes[0]); 
    if (axes[0] < 0.5f) cPos -= glm::normalize(glm::cross(cFront, cUp)) * (cameraSpeed * abs(axes[0])); 


    float xOffset = -axes[3] - lastX;
    float yOffset = lastY - (-axes[4]);
    lastX = -axes[3] * 10;
    lastY = -axes[4] * 10;

    const float sensitivity = 0.1f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;
   
    yaw   += xOffset;
    pitch += yOffset;

    if (pitch >  89.0f) pitch =  89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
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
 
    int presentController = glfwJoystickPresent(GLFW_JOYSTICK_1); // CHECK FOR GAME_CONTROLLER
    printf("JOYSTICK/GAMEPAD 1 STATUS: %s\n", (presentController == GL_TRUE)? "CONNECTED" : "DISCONECTED");
    if ( presentController == GL_TRUE){
        int axesCount;
        const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
        printf("NUMBER OF AXES AVAILABLE: %d\n", axesCount);
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetCursorPosCallback(window, cursorCallBack);
    glfwSetScrollCallback(window, scrollCallback);

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
        glm::vec3( 0.0f,  0.0f,   0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f,  -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f,  -3.5f),
        glm::vec3(-1.7f,  3.0f,  -7.5f),
        glm::vec3( 1.3f, -2.0f,  -2.5f),
        glm::vec3( 1.5f,  2.0f,  -2.5f),
        glm::vec3( 1.5f,  0.2f,  -1.5f),
        glm::vec3(-1.3f,  1.0f,  -1.5f)
    };

    float floorVertices[] = {
        -20.0f, 0.0f, -20.0f, 0.0f, 4.0f,
         20.0f, 0.0f, -20.0f, 4.0f, 4.0f,
         20.0f, 0.0f,  20.0f, 4.0f, 0.0f,
        -20.0f, 0.0f,  20.0f, 0.0f, 0.0f
    };

    float wallVertices[] = {
        -20.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -20.0f, 5.0f, 0.0f, 0.0f, 10.0f,
         20.0f, 5.0f, 0.0f, 40.0f, 10.0f,
         20.0f, 0.0f, 0.0f, 40.0f, 0.0f
    };

    unsigned int floorIndices[] = {
        0, 1, 2,
        0, 2, 3
    };

    unsigned int wallIndices[] = {
        0, 1, 2,
        0, 2, 3
    };

    float texCoords[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.5f, 1.0f
    };

    stbi_set_flip_vertically_on_load(true); // FLIP Y VALUE
    LoadTextures textures; 
    textures.loadAll();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

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

    unsigned int floorVAO, floorVBO, floorEBO;
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glGenBuffers(1, &floorEBO);
    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorIndices), floorIndices, GL_STATIC_DRAW);

    unsigned int wallVAO, wallVBO, wallEBO;
    glGenVertexArrays(1, &wallVAO);
    glBindVertexArray(wallVAO);
    glGenBuffers(1, &wallVBO);
    glGenBuffers(1, &wallEBO);
    glBindBuffer(GL_ARRAY_BUFFER, wallVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertices), wallVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wallEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wallIndices), wallIndices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    

    Shader floorShader  ("../shaders/floorVertexShader.vert" , "../shaders/floorFragmentShader.frag");
    Shader textureShader("../shaders/squareVertexShader.vert", "../shaders/squareFragmentShader.frag");
    Shader wallShader   ("../shaders/wallVertexShader.vert"  , "../shaders/wallFragmentShader.frag");

    //float r = 0.5f; float g = 0.4f; float b = 0.7f;
    float r = 0.0f; float g = 0.0f; float b = 0.0f;
    float alphaBlendVal = 0;

    textureShader.use();
    textureShader.setInt("texture1", 0);
    textureShader.setInt("texture2", 1);

    floorShader.use();
    floorShader.setInt("floorTexture", 2);

    wallShader.use();
    wallShader.setInt("wallTexture", 3);

    float deltaTime = 0.0f; // Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    while (!glfwWindowShouldClose(window)){
        processInput(window);
        processColorScreen(window, &r, &g, &b);
        processCameraInput(window, cameraPos, cameraUp, cameraFront, deltaTime);
        processCameraInputController(window, cameraPos, cameraUp, cameraFront, deltaTime);
        processAlphaBlend(window, &alphaBlendVal);

        glClearColor(r, g, b, 1.0f);  // This functions is a state-setting func for "glClear()"
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // State-using function 

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        textures.woodBoxContainer.bindTexture(0);
        textures.pixelRedEye.bindTexture(1);
        textures.floor.bindTexture(2);
        textures.metal.bindTexture(3);


        textureShader.use();
        int alphaBlendFragLocation = glGetUniformLocation(textureShader.getShaderID(), "alphaBlend");
        glUniform1f(alphaBlendFragLocation, alphaBlendVal);

        glm::mat4 model      = glm::mat4(1.0f);
        glm::mat4 view       = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        model = glm::rotate(model, deltaTime, glm::vec3(0.5f, 1.0f, 0.0f));
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); 

        projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
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
            model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle + currentFrame * 15), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
 
        floorShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -2.8f, 0.0f));
       
        unsigned int floorModelLoc = glGetUniformLocation(floorShader.getShaderID(), "model");
        unsigned int floorViewLoc  = glGetUniformLocation(floorShader.getShaderID(), "view");
        unsigned int floorProjLoc  = glGetUniformLocation(floorShader.getShaderID(), "projection");
        glUniformMatrix4fv(floorModelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(floorViewLoc , 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(floorProjLoc , 1, GL_FALSE, glm::value_ptr(projection));
        
        glBindVertexArray(floorVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        wallShader.use();
        for (int i = 0; i < 4; i++){
            model = glm::mat4(1.0f);
            model = glm::rotate(model, glm::radians(i * 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, glm::vec3(0.0f, -2.8f, -20.0f));
            unsigned int wallModelLoc = glGetUniformLocation(wallShader.getShaderID(), "model");
            unsigned int wallViewLoc  = glGetUniformLocation(wallShader.getShaderID(), "view");
            unsigned int wallProjLoc  = glGetUniformLocation(wallShader.getShaderID(), "projection");
            glUniformMatrix4fv(wallModelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(wallViewLoc , 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(wallProjLoc , 1, GL_FALSE, glm::value_ptr(projection));

            glBindVertexArray(wallVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
