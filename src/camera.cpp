#include "camera.h"
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV){
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
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
glm::mat4 Camera::GetViewMatrix() { return glm::lookAt(Position, Position + Front, Up); }
glm::mat4 Camera::GetTopViewMatrix() { return glm::lookAt(Position, Position + Front, glm::vec3(0.0f, 0.0f, 1.0f)); }

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime){
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;

    Position.y = 0;
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

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch){
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch){
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }
    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset){
    Fov -= (float)yoffset;
    if (Fov < 1.0f)
        Fov = 1.0f;
    if (Fov > 45.0f)
        Fov = 45.0f;
}

// AIM MODE WITH CONTROLLER
void Camera::triggerAimViewFov(const float *controllerAxes){
    if(controllerAxes[2] >=  -1.0f){
        Fov -= ((float)controllerAxes[2]) * 3;
        if (Fov < 20.0f) Fov = 20.0f;
        if (Fov > 45.0f) Fov = 45.0f;
    }
}

void Camera::updateCameraVectors(){
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = glm::normalize(glm::cross(Right, Front));
}
