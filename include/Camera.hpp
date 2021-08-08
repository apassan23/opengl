#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
private:
    glm::mat4 view;
    glm::vec3 up;
    glm::vec3 front;
    glm::vec3 pos;
    float yaw;
    float pitch;
    float lastX, lastY;
    bool firstMouse;
    float fov;

public:
    Camera();
    void setResolution(int width, int height);
    void rotate(float angle, glm::vec3 vec);
    void setPosition(glm::vec3 vec);
    glm::vec3 getPosition();
    void setDirection(glm::vec3 vec);
    glm::vec3 getDirection();
    glm::vec3 getDirectionUp();
    void set();
    void setMouseControl(double xpos, double ypos, float sensitivity);
    const float *getPtr();
    void setScroll(double x, double y);
    void setFOV(float fov);
    float zoom();
};

#endif