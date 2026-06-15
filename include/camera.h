#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
#include <stdio.h>

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =   0.0f;
const float SPEED       =   8.5f;
const float SENSITIVITY =   0.1f;
const float FOV         =  45.0f;

class Camera{
public:
    // internal constants
    const float MAX_DISTANCE = 30.0f;
    const float MIN_DISTANCE =  3.0f;

    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // camera to entity Attributes
    float camXoffset;
    float camZoffset;
    float horizontalDistanceFromEntity;
    float verticalDistanceFromEntity;

    // euler Angles
    float Yaw;
    float Pitch;
    float distance;

    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Fov;

    // This angle enables to rotate the camera around the player at any position in the Y axis
    float angleAroundPlayer = 0;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();
    glm::mat4 GetTopViewMatrix();
    
    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);
    
    void processController(const float *axes, float deltaTime, float xOffset, float yOffset);
    void triggerAimViewFov(const float *controllerAxes);

    // Update camera position to a relative entity
    void updateCameraPos(const glm::vec3 &pos);

    // Using the position and the Yaw of a entity I can make the camera follow it
    void orbitationAroundEntity(const glm::vec3 &pos, const float eYaw, float xCursorOffset, float yCursorOffset);

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};
#endif
