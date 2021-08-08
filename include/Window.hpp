#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <GLFW/glfw3.h>

class Window
{
private:
    GLFWwindow *window;
    int width;
    int height;

public:
    Window();
    Window(int width, int height);
    ~Window();
    void init();
    void create(const char *title);
    void close();
    bool isOpen();
    void setFrameBufferCallback(GLFWframebuffersizefun fun);
    void setScrollCallback(GLFWscrollfun fun);
    void setMouseCallback(GLFWcursorposfun fun);
    void setHeight();
    int getHeight();
    void setWidth();
    int getWidth();
    void enableEvents();
    GLFWwindow *getWindow();
};

#endif