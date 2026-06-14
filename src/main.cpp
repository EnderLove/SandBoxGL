#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/trigonometric.hpp>
#include <stdio.h>
#include <math.h>

#include "camera.h"
#include "shader.h"

#include "load2DTexture.h"
#include "models.h"
#include "gameTextures.cpp"

const int SCR_SOURCE_WIDTH  = 1920;
const int SCR_SOURCE_HEIGHT = 1080;
const int SCR_WIDTH  = 16 * 80;
const int SCR_HEIGHT =  9 * 90;

float cubePosX = 0;
float cubePosZ = 0;
float cubeOrientation = 0;

glm::vec3 cubePos    (0.0f, 0.0f,  0.0f);
//glm::vec3 cubePos    (0.0f, -2.3f,  0.0f);
glm::vec3 cubeWorldUp(0.0f,  1.0f,  0.0f);
glm::vec3 cubeUp     (0.0f,  1.0f,  0.0f);
glm::vec3 cubeFront  (0.0f,  0.0f, -1.0f);
glm::vec3 cubeRight  (1.0f,  0.0f,  0.0f);
float cubeYaw = 0;
float cubePitch = 0;

float xCursorOffset = 0;
float yCursorOffset = 0;
double xCursorPos = 0;
double yCursorPos = 0;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f)); // Initialization of the camera with a pos

float lastX = (float)SCR_WIDTH  / 2;
float lastY = (float)SCR_HEIGHT / 2;
bool firstMouse = true;

float deltaTime = 0.0f; // Diference of time between frames
float lastFrame = 0.0f; // Time of the last frame

float r = 0.0f; float g = 0.0f; float b = 0.0f; // BACKGROUND COLOR

void frameBufferSizeCallback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
    printf("RESIZE: %4d | %4d\n", width, height);
}

void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
 
void cursorCallBack(GLFWwindow *window, double xPos, double yPos){
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){

        xCursorPos = xPos;
        yCursorPos = yPos;
        glm::vec3 cubeDir(0.0f, 0.0f, 0.0f);

        if (firstMouse){
            lastX = xCursorPos;
            lastY = yCursorPos;
            firstMouse = false;
        }

        xCursorOffset = xCursorPos - lastX;
        yCursorOffset = lastY - yCursorPos;

        lastX = xCursorPos;
        lastY = yCursorPos;

        // TODO :IMPLEMENT CAMERA MOVEMENT AROUND THE CUBE
        //camera.ProcessMouseMovement(xCursorOffset, yCursorOffset);

        cubeYaw   += xCursorOffset * 0.01f;
        cubePitch += yCursorOffset * 0.01f; // The pitch is useless by now

        cubeDir.z = -cos(cubeYaw); 
        cubeDir.x =  sin(cubeYaw);

        cubeFront = glm::normalize(cubeDir);
        cubeRight = glm::normalize(glm::cross(cubeFront, cubeWorldUp));
        cubeUp    = glm::normalize(glm::cross(cubeRight, cubeFront));

        cubeOrientation = -cubeYaw; // ANGLE OF RORATION FOR THE CUBE

        //camera.Pitch -= yCursorOffset * 0.01f;
        //camera.angleAroundPlayer -= xCursorOffset * 0.01f;

        camera.Pitch -= yCursorOffset;
        camera.angleAroundPlayer -= xCursorOffset;

        // TODO Investigate the use of radians with cos and sin
        float horizontalCameraDistanceFromCube = camera.distance * cos(glm::radians(camera.Pitch));
        float verticalCameraDistanceFromCube   = camera.distance * sin(glm::radians(camera.Pitch));

        float theta = cubeYaw + camera.angleAroundPlayer;
        float camXoffset = horizontalCameraDistanceFromCube * sin(glm::radians(theta));
        float camZoffset = horizontalCameraDistanceFromCube * cos(glm::radians(theta));
        
        camera.Position.x = cubePos.x - camXoffset;
        camera.Position.y = cubePos.y + verticalCameraDistanceFromCube;
        camera.Position.z = cubePos.z - camZoffset;

        camera.Yaw = 180 - (cubeYaw + camera.angleAroundPlayer);

        camera.updateCameraVectors();
        //camera.rotateAround(cubePos, cubeYaw, cubePitch);
    }
};

//void cursorCallBack(GLFWwindow *window, double xPos, double yPos){
//    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
//
//        xCursorPos = xPos;
//        yCursorPos = yPos;
//        glm::vec3 cubeDir(0.0f, 0.0f, 0.0f);
//
//        if (firstMouse){
//            lastX = xCursorPos;
//            lastY = yCursorPos;
//            firstMouse = false;
//        }
//
//        xCursorOffset = xCursorPos - lastX;
//        yCursorOffset = lastY - yCursorPos;
//
//        lastX = xCursorPos;
//        lastY = yCursorPos;
//
//        // TODO :IMPLEMENT CAMERA MOVEMENT AROUND THE CUBE
//        //camera.ProcessMouseMovement(xCursorOffset, yCursorOffset);
//
//        cubeYaw   += xCursorOffset * 0.01f;
//        cubePitch += yCursorOffset * 0.01f; // The pitch is useless by now
//
//        cubeDir.z = -cos(cubeYaw); 
//        cubeDir.x =  sin(cubeYaw);
//
//        cubeFront = glm::normalize(cubeDir);
//        cubeRight = glm::normalize(glm::cross(cubeFront, cubeWorldUp));
//        cubeUp    = glm::normalize(glm::cross(cubeRight, cubeFront));
//
//        cubeOrientation = -cubeYaw; // ANGLE OF RORATION FOR THE CUBE
//        //camera.rotateAround(cubePos, cubeYaw, cubePitch);
//    }
//};

// TODO MOUSE_CALLBACK_FUNCTION IMPLEMENT

void scrollCallback(GLFWwindow *window, double xOffset, double yOffset){
    camera.ProcessMouseScroll(yOffset);
}

const float *controllerAxes;
const unsigned char *controllerButtons;
bool CONTROLLER_CONNECTED = false;

void loadControllerGamePad(){
    if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GLFW_TRUE){
        int axesCount;
        int buttonCount;
        controllerAxes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
        controllerButtons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
        CONTROLLER_CONNECTED = true;
        printf("CONTROLLER_CONNECTED==================\n");
    }
}

void processCameraInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { camera.ProcessKeyboard(FORWARD , deltaTime); }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { camera.ProcessKeyboard(BACKWARD, deltaTime); }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { camera.ProcessKeyboard(LEFT    , deltaTime); }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { camera.ProcessKeyboard(RIGHT   , deltaTime); }
}

void processCameraInputController(){
    float xOffset = 0;
    float yOffset = 0;

    if (!(controllerAxes[3] < 0.5f && controllerAxes[3] > -0.5f)) xOffset = -controllerAxes[3] - lastX;
    if (!(controllerAxes[4] < 0.5f && controllerAxes[4] > -0.5f)) yOffset = lastY - (-controllerAxes[4]);
    lastX = -controllerAxes[3] * 10;
    lastY = -controllerAxes[4] * 10;
    
    camera.processController(controllerAxes, deltaTime, xOffset, yOffset);
}

void processPlayerInput(GLFWwindow *window){
    float playerSpeed = 14.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { cubePos += playerSpeed * cubeFront; }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { cubePos -= playerSpeed * cubeFront; }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { cubePos -= playerSpeed * cubeRight; }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { cubePos += playerSpeed * cubeRight; }
}

void processColorScreen(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) { r += 0.01f; if (r > 1) r = 0; }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) { g += 0.01f; if (g > 1) g = 0; }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) { b += 0.01f; if (b > 1) b = 0; }

    if(glfwJoystickPresent(GLFW_JOYSTICK_1)){
        if (controllerButtons[0] == GLFW_PRESS) { g += 0.01f; if (g > 1) g = 0; } // A
        if (controllerButtons[1] == GLFW_PRESS) { b += 0.01f; if (b > 1) b = 0; } // B
        if (controllerButtons[2] == GLFW_PRESS) { r += 0.01f; if (r > 1) r = 0; } // X
    } 
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
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        printf("Failed to initialize GLAD");
        return -1;
    }
 
    printf("JOYSTICK/GAMEPAD 1 STATUS: %s\n", (CONTROLLER_CONNECTED == GL_TRUE)? "CONNECTED" : "DISCONECTED");
    if (CONTROLLER_CONNECTED == GL_TRUE){
        int axesCount;
        const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
        printf("NUMBER OF AXES AVAILABLE: %d\n", axesCount);
    }

    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorCallBack);
    glfwSetScrollCallback(window, scrollCallback);
    loadControllerGamePad();

    glEnable(GL_DEPTH_TEST);
    
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    printf("Maximun number of vertex attributes supported: %d\n", nrAttributes);
   
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    stbi_set_flip_vertically_on_load(true); // FLIP Y VALUE
    LoadTextures textures; 
    textures.loadAll();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // FIRST WE CREATE THE VERTEX ARRAY OBJECT THAT WILL STORE ALL THE SETTINGS
    unsigned int VAO[2], VBO[2], EBO[2]; 
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, EBO);
    glGenBuffers(2, VBO);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_DYNAMIC_DRAW);

    // LIGHT VERTEX DATA LOADING
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(1, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // FLOOR VERTEX DATA LOADING
    unsigned int floorVAO, floorVBO, floorEBO;
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glGenBuffers(1, &floorEBO);
    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorIndices), floorIndices, GL_STATIC_DRAW);

    // WALL VERTES DATA LOADING
    unsigned int wallVAO, wallVBO, wallEBO;
    glGenVertexArrays(1, &wallVAO);
    glBindVertexArray(wallVAO);
    glGenBuffers(1, &wallVBO);
    glGenBuffers(1, &wallEBO);
    glBindBuffer(GL_ARRAY_BUFFER, wallVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertices), wallVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wallEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wallIndices), wallIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // SHADER LOADING 
    Shader floorShader ("../shaders/floorVertexShader.vert" , "../shaders/floorFragmentShader.frag");
    Shader cubeShader  ("../shaders/squareVertexShader.vert", "../shaders/squareFragmentShader.frag");
    //Shader wallShader  ("../shaders/wallVertexShader.vert"  , "../shaders/wallFragmentShader.frag");
    Shader wallShader  ("../shaders/floorVertexShader.vert"  , "../shaders/floorFragmentShader.frag");
    Shader lightShader ("../shaders/lightVertexShader.vert" , "../shaders/lightFragmentShader.frag");
    Shader lightSrcShader("../shaders/lightSourceVertexShader.vert", "../shaders/lightSourceFragmentShader.frag");
    Shader materialShader("../shaders/materialVertexShader.vert", "../shaders/materialFragmentShader.frag");

    
    cubeShader.use();
    cubeShader.setInt("texture1", 0);
    cubeShader.setInt("texture2", 1);
    floorShader.use();
    floorShader.setInt("floorTexture", 2);
    wallShader.use();
    //wallShader.setInt("wallTexture", 3);
    wallShader.setInt("floorTexture", 3);

    glm::vec3 lightPos = glm::vec3(-35.0f, 5.0f, 0.0f);
    cubeShader.use();
    cubeShader.setVec3("light.position", lightPos);

    materialShader.use();
    materialShader.setInt("material.diffuse", 4);
    materialShader.setInt("material.specular", 5);
    materialShader.setInt("material.emission", 6);
    materialShader.setVec3("light.ambient" , glm::vec3(1.0f, 1.0f, 1.0f));
    materialShader.setVec3("light.diffuse" , glm::vec3(1.0f, 1.0f, 1.0f));
    materialShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    //materialShader.setVec3("material.ambient" , glm::vec3(0.5f, 0.2f, 0.11f));
    //materialShader.setVec3("material.diffuse" , glm::vec3(0.0f, 0.5f, 1.0f));
    //materialShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 1.0f));
    materialShader.setFloat("material.shininess", 256.0f); 

    glm::vec3 colorLight;
    float alphaBlendVal = 0;


    while (!glfwWindowShouldClose(window)){
        processInput(window);
        processColorScreen(window);
        //processCameraInput(window); 
        processAlphaBlend(window, &alphaBlendVal);
        processPlayerInput(window);
        
        if (CONTROLLER_CONNECTED) processCameraInputController();
        if (CONTROLLER_CONNECTED) camera.triggerAimViewFov(controllerAxes);

        glClearColor(r, g, b, 1.0f);  // This functions is a state-setting func for "glClear()"
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // State-using function 

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        
        // TEXTURE BINDING 
        textures.woodBoxContainer.bindTexture(1);
        textures.pixelRedEye.bindTexture(0);
        textures.floor.bindTexture(2);
        textures.metal.bindTexture(3);
        textures.container2.bindTexture(4);
        textures.container2Spec.bindTexture(5);
        textures.matrix.bindTexture(6);
        //textures.container2WoodEmission.bindTexture(6);

        // GLOBAL VIEW & PROJECTION 
        glm::mat4 globalView       = glm::mat4(1.0f);
        glm::mat4 globalProjection = glm::mat4(1.0f);
        globalView = camera.GetViewMatrix(); 
        globalProjection = glm::perspective(glm::radians(camera.Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
        // LIGHT POS ROTATION 
        materialShader.use();
        float lightAngle = 0.10f * deltaTime;
        float lPosx = lightPos.x;
        float lPosz = lightPos.z;
        lightPos.x = (cos(lightAngle) * lPosx) + (-sin(lightAngle) * lPosz);
        lightPos.z = (sin(lightAngle) * lPosx) + ( cos(lightAngle) * lPosz);
        if (lightPos.y <= -1.0f) lightPos.x = -35.0f;
        materialShader.setVec3("light.position", lightPos);
        materialShader.setVec3("viewPos", camera.Position);

        colorLight = glm::vec3(1.0f);

        glm::vec3 diffuseColor = colorLight * glm::vec3(1.0f);
        glm::vec3 ambientColor = diffuseColor;

        materialShader.setVec3("light.diffuse", diffuseColor);

        // CUBES ROTATING
        materialShader.use();
        float texMoveSpeed;
        materialShader.setFloat("time", glfwGetTime());
        materialShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 1.0f));
        materialShader.setVec3("lightColor" , glm::vec3(1.0f, 1.0f, 1.0f));

        glm::mat4 cubeModel = glm::mat4(1.0f);
        unsigned int modelLoc    = glGetUniformLocation(materialShader.getShaderID(), "model");
        unsigned int modelInvLoc = glGetUniformLocation(materialShader.getShaderID(), "modelInverse");
        unsigned int viewLoc     = glGetUniformLocation(materialShader.getShaderID(), "view");
        unsigned int projLoc     = glGetUniformLocation(materialShader.getShaderID(), "projection");
        glUniformMatrix4fv(viewLoc , 1, GL_FALSE, glm::value_ptr(globalView));
        glUniformMatrix4fv(projLoc , 1, GL_FALSE, glm::value_ptr(globalProjection));
     
        // TODO: Implement a way to unlock the xOffset by pressing the right click
        glBindVertexArray(VAO[0]);
        cubeModel = glm::translate(cubeModel, cubePos);
        cubeModel = glm::rotate(cubeModel, cubeOrientation, glm::vec3(0.0f, 1.0f, 0.0f)); // REMOVED THE RADIANS TRANSFORMATION
        cubeModel = glm::scale(cubeModel, glm::vec3(0.7f, 0.7f, 0.7f));

        glm::mat4 cubeModelInverse = glm::inverse(cubeModel); // NORMAL MATRIX
        glUniformMatrix4fv(modelInvLoc, 1, GL_FALSE, glm::value_ptr(cubeModelInverse));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cubeModel));
        glDrawArrays(GL_TRIANGLES, 0, 36);
      
        //camera.updateCameraPos(glm::vec3(0.0f, 0.0f, cubePos.z));    
        //if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
        //    camera.rotateAround(cubePos, cubeYaw, cubePitch);
        //}else {
        //    camera.rotateAround(cubePos, 0, 0);
        //}

        floorShader.use();
        // SCENARIO FLOOR
        floorShader.setVec3("lightPos", lightPos);
        floorShader.setVec3("viewPos", camera.Position);
        floorShader.setVec3("objectColor", glm::vec3(0.5f, 0.2f, 0.5f));
        floorShader.setVec3("lightColor" , glm::vec3(1.0f, 1.0f, 1.0f));
        
        glm::mat4 floorModel= glm::mat4(1.0f);
        floorModel= glm::translate(floorModel, glm::vec3(0.0f, -2.8f, 0.0f));
        glm::mat4 floorModelInv = glm::inverse(floorModel);

        unsigned int floorModelLoc = glGetUniformLocation(floorShader.getShaderID(), "model");
        unsigned int floorViewLoc  = glGetUniformLocation(floorShader.getShaderID(), "view");
        unsigned int floorProjLoc  = glGetUniformLocation(floorShader.getShaderID(), "projection");
        unsigned int floorInvModelLoc = glGetUniformLocation(floorShader.getShaderID(), "modelInverse"); 
        glUniformMatrix4fv(floorModelLoc, 1, GL_FALSE, glm::value_ptr(floorModel));
        glUniformMatrix4fv(floorViewLoc , 1, GL_FALSE, glm::value_ptr(globalView));
        glUniformMatrix4fv(floorProjLoc , 1, GL_FALSE, glm::value_ptr(globalProjection));
        glUniformMatrix4fv(floorInvModelLoc, 1, GL_FALSE, glm::value_ptr(floorModelInv));
        
        glBindVertexArray(floorVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // SCENARIO WALLS
        wallShader.use();
        wallShader.setVec3("LightPos", lightPos);
        wallShader.setVec3("viewPos", camera.Position);
        wallShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        wallShader.setVec3("objectColor", glm::vec3(0.5f, 0.1f, 0.5f));
        for (int i = 0; i < 4; i++){
            glm::mat4 wallModel= glm::mat4(1.0f);
            wallModel= glm::rotate(wallModel, glm::radians(i * 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            wallModel= glm::translate(wallModel, glm::vec3(0.0f, -2.8f, -20.0f));
            glm::mat4 wallInvModel = glm::inverse(wallModel);
            unsigned int wallInvModelLoc = glGetUniformLocation(wallShader.getShaderID(), "modelInverse");
            unsigned int wallModelLoc = glGetUniformLocation(wallShader.getShaderID(), "model");
            unsigned int wallViewLoc  = glGetUniformLocation(wallShader.getShaderID(), "view");
            unsigned int wallProjLoc  = glGetUniformLocation(wallShader.getShaderID(), "projection");
            glUniformMatrix4fv(wallInvModelLoc, 1, GL_FALSE, glm::value_ptr(wallInvModel));
            glUniformMatrix4fv(wallModelLoc, 1, GL_FALSE, glm::value_ptr(wallModel));
            glUniformMatrix4fv(wallViewLoc , 1, GL_FALSE, glm::value_ptr(globalView));
            glUniformMatrix4fv(wallProjLoc , 1, GL_FALSE, glm::value_ptr(globalProjection));

            glBindVertexArray(wallVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        // SCENARIO LIGHT SOURCE 
        lightSrcShader.use();
        lightSrcShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        unsigned int lightModelLoc = glGetUniformLocation(lightSrcShader.getShaderID(), "model");
        unsigned int lightViewLoc  = glGetUniformLocation(lightSrcShader.getShaderID(), "view");
        unsigned int lightProjLoc  = glGetUniformLocation(lightSrcShader.getShaderID(), "projection");
        glUniformMatrix4fv(lightModelLoc, 1, GL_FALSE, glm::value_ptr(lightModel));
        glUniformMatrix4fv(lightViewLoc , 1, GL_FALSE, glm::value_ptr(globalView));
        glUniformMatrix4fv(lightProjLoc , 1, GL_FALSE, glm::value_ptr(globalProjection));

        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
        lastFrame = currentFrame;
    }
    glfwTerminate();
    return 0;
}
