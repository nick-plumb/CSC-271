//
// Created by dengq on 10/29/25.
//
#include "camera.h"
#include <cmath>

Camera::Camera() {
    updateVectors();

}

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float yawDeg, float pitchDeg)
        : Position(position), WorldUp(worldUp), Yaw(yawDeg), Pitch(pitchDeg) {
    updateVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
    // return Camera::myLookAt(Position, Position + Front, Up);
    return glm::lookAt(Position, Position + Front, Up);
}
// hw 5
glm::mat4 Camera::myLookAt(glm::vec3 cPos, glm::vec3 front, glm::vec3 worldUp) const {
    glm::vec3 forward = glm::normalize(cPos - front);
    glm::vec3 right = glm::normalize(glm::cross(worldUp, forward));
    glm::vec3 up = glm::normalize(glm::cross(forward, right));

    glm::mat4 view(1.0f);

    view[0][0] = right.x;
    view[1][0] = right.y;
    view[2][0] = right.z;

    view[0][1] = up.x;
    view[1][1] = up.y;
    view[2][1] = up.z;

    view[0][2] = forward.x;
    view[1][2] = forward.y;
    view[2][2] = forward.z;

    view[3][0] = -glm::dot(right, cPos);
    view[3][1] = -glm::dot(up, cPos);
    view[3][2] = -glm::dot(forward, cPos);
    return view;
}

glm::mat4 Camera::GetProjection(float aspect, float nearPlane, float farPlane) const {
    return glm::perspective(glm::radians(Zoom), aspect, nearPlane, farPlane);
}
// end hw 5
void Camera::ProcessKeyboard(GLFWwindow *window, float deltaTime) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    const float velocity = MovementSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Position += velocity * Front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Position -= velocity * Front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Position -= glm::normalize(glm::cross(Front, Up)) * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Position += glm::normalize(glm::cross(Front, Up)) * velocity;
    //added this to make movement easier - up and down movement without turning camera
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        Position += glm::vec3(0, velocity, 0);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        Position -= glm::vec3(0, velocity, 0);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    if (constrainPitch) {
        Pitch = std::clamp(Pitch, -89.9f, 89.9f);
    }

    updateVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    Zoom -= yoffset;
    Zoom = std::clamp(Zoom, 1.0f, 90.0f);
}

void Camera::updateVectors() {
    // Convert yaw/pitch to a forward vector (right-handed, OpenGL-style)
    const float yawRad   = glm::radians(Yaw);
    const float pitchRad = glm::radians(Pitch);

    glm::vec3 f;
    f.x = std::cos(yawRad) * std::cos(pitchRad);
    f.y = std::sin(pitchRad);
    f.z = std::sin(yawRad) * std::cos(pitchRad);
    Front = glm::normalize(f);

    // Derive Right and Up to form an orthonormal basis (Gram–Schmidt)
    Right = glm::normalize(glm::cross(Front, WorldUp));
    // If Front is nearly parallel to WorldUp, Right can become tiny; clamping Pitch above avoids this.
    Up    = glm::normalize(glm::cross(Right, Front));
}