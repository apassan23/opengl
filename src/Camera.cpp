#include "Camera.hpp"

#include <cmath>

Camera::Camera()
{
    view = glm::mat4(1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    pos = glm::vec3(0.0f, 0.0f, 3.0f);
    yaw = -90.0f;
    pitch = 0.0f;
    firstMouse = true;
    lastX = 400.0f;
    lastY = 300.0f;
}

void Camera::setResolution(int width, int height)
{
    lastX = width / 2;
    lastY = height / 2;
}

void Camera::rotate(float angle, glm::vec3 vec)
{
    view = glm::rotate(view, glm::radians(angle), vec);
}

void Camera::setPosition(glm::vec3 vec)
{
    pos = vec;
}

glm::vec3 Camera::getPosition()
{
    return this->pos;
}

void Camera::setDirection(glm::vec3 vec)
{
    front = vec;
}

glm::vec3 Camera::getDirection()
{
    return this->front;
}

glm::vec3 Camera::getDirectionUp()
{
    return this->up;
}

void Camera::set()
{
    view = glm::lookAt(pos, pos + front, up);
}

void Camera::setMouseControl(double xpos, double ypos, float sensitivity)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // since y input is inverted
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    this->yaw += xoffset;
    this->pitch += yoffset;

    if (this->pitch > 89.0f)
        this->pitch = 89.0f;
    if (this->pitch < -89.0f)
        this->pitch = -89.0f;

    glm::vec3 direction;
    direction.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    direction.y = std::sin(glm::radians(pitch));
    direction.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    this->front = glm::normalize(direction);
}

const float *Camera::getPtr()
{
    return glm::value_ptr(this->view);
}

void Camera::setFOV(float fov)
{
    this->fov = fov;
}

void Camera::setScroll(double x, double y)
{
    fov -= static_cast<float>(y);

    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

float Camera::zoom()
{
    return fov;
}