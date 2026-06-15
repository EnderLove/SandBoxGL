#include "camera.h"
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV){
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    distance = position.z;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV){
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()    { return glm::lookAt(Position, Position + Front, Up); }
glm::mat4 Camera::GetTopViewMatrix() { return glm::lookAt(Position, Position + Front, glm::vec3(0.0f, 0.0f, 1.0f)); }

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset){
    distance -= (float)yoffset;
    if (distance < MIN_DISTANCE) distance = MIN_DISTANCE;
    if (distance > MAX_DISTANCE) distance = MAX_DISTANCE;
}

void Camera::orbitationAroundEntity(const glm::vec3 &ePos, const float eYaw, float xCursorOffset, float yCursorOffset){
    Pitch -= yCursorOffset * 0.01f;

    if (Pitch >  89.0f) Pitch =  89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;

    horizontalDistanceFromEntity = distance * cos(glm::radians(Pitch));
    verticalDistanceFromEntity   = distance * sin(glm::radians(Pitch));

    float theta = -eYaw + 180.0f;
    camXoffset = horizontalDistanceFromEntity * sin(glm::radians(theta));
    camZoffset = horizontalDistanceFromEntity * cos(glm::radians(theta));
    
    Yaw = theta;
    
    Position.x = ePos.x - camXoffset;
    Position.y = ePos.y + verticalDistanceFromEntity;
    Position.z = ePos.z - camZoffset;

    updateCameraPos(ePos);
    updateCameraVectors();
}

// TODO SET A ENTITY POS TO THE CONSTRUCTOR OR ANY OTHER METHOD
void Camera::updateCameraPos(const glm::vec3 &ePos){
    Position.x = ePos.x - camXoffset;
    Position.y = ePos.y + verticalDistanceFromEntity;
    Position.z = ePos.z - camZoffset;
}

void Camera::updateCameraVectors(){
    glm::vec3 front;
    front.x =  sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = -sin(glm::radians(Pitch));
    front.z =  cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = glm::normalize(glm::cross(Right, Front));
}

// =============================== CONTROLLER METHODS ========================================

// AIM MODE WITH CONTROLLER
void Camera::triggerAimViewFov(const float *controllerAxes){
    if(controllerAxes[2] >=  -1.0f){
        Fov -= ((float)controllerAxes[2]) * 3;
        if (Fov < 20.0f) Fov = 20.0f;
        if (Fov > 45.0f) Fov = 45.0f;
    }
}

void Camera::processController(const float *axes, float deltaTime, float xOffset, float yOffset){
    //TODO: ADD ANSI SCAPE CODES TO SHOW CONSTANT OUTPUT IN TERMINAL 
    //printf("LEFT  STICK X AXIS %f\n", axes[0]);
    //printf("LEFT  STICK Y AXIS %f\n", axes[1]);
    //printf("RIGHT STICK X AXIS %f\n", axes[2]);
    //printf("RIGHT STICK Y AXIS %f\n", axes[4]);

    const float cameraSpeed = deltaTime * 10.0f;

    // CAMERA MOVEMENT 
    //
    if (!(axes[1] < 0.5f && axes[1] > -0.5f)){
        if (axes[1] > 0.5f) Position -= (cameraSpeed * axes[1]) * Front; 
        if (axes[1] < 0.5f) Position += (cameraSpeed * abs(axes[1])) * Front; 
    }
    if (!(axes[0] < 0.5f && axes[0] > -0.5f)){
        if (axes[0] > 0.5f) Position += glm::normalize(glm::cross(Front, Up)) * (cameraSpeed * axes[0]); 
        if (axes[0] < 0.5f) Position -= glm::normalize(glm::cross(Front, Up)) * (cameraSpeed * abs(axes[0])); 
    } 

    // CAMERA VIEW MOVEMENT
    xOffset *= SENSITIVITY * 2;
    yOffset *= SENSITIVITY * 2;
    Yaw   += xOffset;
    Pitch += yOffset;

    if (Pitch >  89.0f) Pitch =  89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;
    
    //Position.y = 0.0f; // STAY AT XZ LEVEL
    updateCameraVectors();
}
