#include <Window.hpp>
#include <iostream>

Window::Window(int width, int height)
{
    this->width = width;
    this->height = height;
}

Window::Window()
{
    width = 800;
    height = 600;
}

Window::~Window()
{
    glfwTerminate();
}

void Window::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Window::create(const char *title)
{
    window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (window == NULL)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create Window !\n");
    }

    glfwMakeContextCurrent(window);
}

void Window::close()
{
    glfwSetWindowShouldClose(this->window, true);
}

bool Window::isOpen()
{
    return (!glfwWindowShouldClose(this->window));
}

void Window::setFrameBufferCallback(GLFWframebuffersizefun fun)
{
    glfwSetFramebufferSizeCallback(this->window, fun);
}

void Window::setScrollCallback(GLFWscrollfun fun)
{
    glfwSetScrollCallback(this->window, fun);
}

void Window::setMouseCallback(GLFWcursorposfun fun)
{
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(this->window, fun);
}

void Window::enableEvents()
{
    glfwSwapBuffers(this->window);
    glfwPollEvents();
}

GLFWwindow *Window::getWindow()
{
    return this->window;
}