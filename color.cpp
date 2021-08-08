#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Camera.hpp>
#include <Shader.hpp>
#include <iostream>

int WIDTH = 800;
int HEIGHT = 600;

Camera cam;
float fov = 45.0f;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

float deltaTime = 0.0f, lastFrame = 0.0f;

void framebuffer_callback(GLFWwindow *window, int width, int height);
void processInputs(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "color", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create Window!\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialise GLAD ! \n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    cam.setResolution(WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f};

    GLuint objectVAO, lightVAO;
    GLuint VBO;

    glGenVertexArrays(1, &objectVAO);
    glGenVertexArrays(1, &lightVAO);

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(objectVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    Shader objectShader("light/VShader.vs", "light/objFragShader.fs"), lightShader("light/VShader.vs", "light/lightFragShader.fs");

    glm::mat4 objectModel(1.0f);
    glm::mat4 lightModel(1.0f);
    lightModel = glm::translate(lightModel, lightPos);
    lightModel = glm::scale(lightModel, glm::vec3(0.2f));

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(fov), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        processInputs(window);

        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cam.set();

        objectShader.use();

        GLint location;
        location = glGetUniformLocation(objectShader.getID(), "lightColor");
        glUniform3fv(location, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

        location = glGetUniformLocation(objectShader.getID(), "objColor");
        glUniform3fv(location, 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));

        location = glGetUniformLocation(objectShader.getID(), "model");
        glUniformMatrix4fv(location, 1, false, glm::value_ptr(objectModel));

        location = glGetUniformLocation(objectShader.getID(), "view");
        glUniformMatrix4fv(location, 1, false, cam.getPtr());

        location = glGetUniformLocation(objectShader.getID(), "projection");
        glUniformMatrix4fv(location, 1, false, glm::value_ptr(projection));

        glBindVertexArray(objectVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lightShader.use();

        location = glGetUniformLocation(lightShader.getID(), "model");
        glUniformMatrix4fv(location, 1, false, glm::value_ptr(lightModel));

        location = glGetUniformLocation(lightShader.getID(), "view");
        glUniformMatrix4fv(location, 1, false, cam.getPtr());

        location = glGetUniformLocation(lightShader.getID(), "projection");
        glUniformMatrix4fv(location, 1, false, glm::value_ptr(projection));

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &objectVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    return EXIT_SUCCESS;
}

void framebuffer_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, WIDTH, HEIGHT);
}

void processInputs(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    float cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cam.setPosition(cam.getPosition() + (cam.getDirection() * cameraSpeed));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cam.setPosition(cam.getPosition() - (cam.getDirection() * cameraSpeed));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        glm::vec3 rightVec = glm::cross(cam.getDirection(), cam.getDirectionUp());
        cam.setPosition(cam.getPosition() - (glm::normalize(rightVec) * cameraSpeed));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        glm::vec3 rightVec = glm::cross(cam.getDirection(), cam.getDirectionUp());
        cam.setPosition(cam.getPosition() + (glm::normalize(rightVec) * cameraSpeed));
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    cam.setMouseControl(xpos, ypos, 0.1f);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    fov -= static_cast<float>(yoffset);
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}