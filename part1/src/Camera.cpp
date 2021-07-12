#include "Camera.hpp"

#include "glm/gtx/transform.hpp"

#include <iostream>

Camera& Camera::Instance(){
    static Camera* instance = new Camera();
    return *instance;
}

void Camera::MouseLook(int mouseX, int mouseY){
    // Detect how much the mouse has moved since
    // the last time
    glm::vec2 mouseDelta = 0.01f*(glm::vec2(mouseX, mouseY)-m_oldMousePosition);
    glm::vec3 xVector = glm::vec3(1,0,0);
    glm::mat3 yRot = glm::mat3(glm::rotate(-mouseDelta.y, xVector));
    glm::mat3 xRot = glm::mat3(glm::rotate(-mouseDelta.x, m_upVector));

    m_viewDirection = yRot * xRot * m_viewDirection;
    
    // Update our old position after we have made changes 
    m_oldMousePosition = glm::vec2(mouseX, mouseY);
}

void Camera::MoveForward(float speed){
    m_eyePosition = glm::vec3(GetEyeXPosition(), GetEyeYPosition(), GetEyeZPosition() + speed);
}

void Camera::MoveBackward(float speed){
    m_eyePosition = glm::vec3(GetEyeXPosition(), GetEyeYPosition(), GetEyeZPosition() - speed);
}

void Camera::MoveLeft(float speed){
    m_eyePosition = glm::vec3(GetEyeXPosition() - speed, GetEyeYPosition(), GetEyeZPosition());
}

void Camera::MoveRight(float speed){
    m_eyePosition = glm::vec3(GetEyeXPosition() + speed, GetEyeYPosition(), GetEyeZPosition());
}

void Camera::MoveUp(float speed){
    m_eyePosition = glm::vec3(GetEyeXPosition(), GetEyeYPosition() + speed, GetEyeZPosition());
}

void Camera::MoveDown(float speed){
    m_eyePosition = glm::vec3(GetEyeXPosition(), GetEyeYPosition() - speed, GetEyeZPosition());
}

float Camera::GetEyeXPosition(){
    return m_eyePosition.x;
}

float Camera::GetEyeYPosition(){
    return m_eyePosition.y;
}

float Camera::GetEyeZPosition(){
    return m_eyePosition.z;
}

float Camera::GetViewXDirection(){
    return m_viewDirection.x;
}

float Camera::GetViewYDirection(){
    return m_viewDirection.y;
}

float Camera::GetViewZDirection(){
    return m_viewDirection.z;
}

Camera::Camera(){
    std::cout << "Created a Single Camera!\n";
	// Position us at the origin.
    m_eyePosition = glm::vec3(0.0f,0.0f, 0.0f);
	// Looking down along the z-axis initially.
    // Set the view direction to be positive, so that our DOF effect can work
    m_viewDirection = glm::vec3(0.0f,0.0f, 1.0f);
	// For now--our upVector always points up along the y-axis
    m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::GetWorldToViewmatrix() const{
    // Think about the second argument and why that is
    // setup as it is.
    return glm::lookAt( m_eyePosition,
                        // second arg is the center, (position camera is looking at)
                        // This essentially the direction vector
                        // Since eyePosition is (0,0,0), this doesn't really matter
                        // But if the camera were somewhere else, this will.
                        m_eyePosition + m_viewDirection,
                        m_upVector);
}
