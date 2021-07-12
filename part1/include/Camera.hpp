/** @file Camera.hpp
 *  @brief Sets up an OpenGL camera.
 *  
 *  Sets up an OpenGL Camera. The camera is what
 *  sets up our 'view' matrix.
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/glm.hpp"

class Camera{
public:
	// Singleton pattern for having one single camera.
	static Camera& Instance();
    // Return a 'view' matrix with our
    // camera transformation applied.
    glm::mat4 GetWorldToViewmatrix() const;
    // Move the camera around
    void MouseLook(int mouseX, int mouseY);
    void MoveForward(float speed);
    void MoveBackward(float speed);
    void MoveLeft(float speed);
    void MoveRight(float speed);
    void MoveUp(float speed);
    void MoveDown(float speed);
    // Returns the 'eye' position which
    // is where the camera is.
    float GetEyeXPosition();
    float GetEyeYPosition();
    float GetEyeZPosition();
	// Returns the 'view' position
    float GetViewXDirection();
    float GetViewYDirection();
    float GetViewZDirection();
private:
	// Constructor is private because we should
    // not be able to construct any cameras,
    // this how we ensure only one is ever created
    Camera();
    // Track the old mouse position
    glm::vec2 m_oldMousePosition;
    // Where is our camera positioned
    glm::vec3 m_eyePosition;
    // What direction is the camera looking
    glm::vec3 m_viewDirection;
    // Which direction is 'up' in our world
    // Generally this is constant, but if you wanted
    // to 'rock' or 'rattle' the camera you might play
    // with modifying this value.
    glm::vec3 m_upVector;
};




#endif
