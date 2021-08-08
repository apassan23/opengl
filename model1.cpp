#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shader.hpp>
#include <Window.hpp>
#include <Texture.hpp>
#include <Camera.hpp>
#include <Model.hpp>
#include <string>

int HEIGHT = 1080;
int WIDTH = 1920;

Camera camera;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_callback(GLFWwindow *window, int width, int height);
void processInputs(Window &window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Error : No Model Path Specified\n";
        return EXIT_FAILURE;
    }

    Window window(WIDTH, HEIGHT);
    window.init();
    window.create("Models");

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialise GLAD !\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    camera.setResolution(WIDTH, HEIGHT);
    camera.setFOV(45.0f);
    window.setFrameBufferCallback(framebuffer_callback);
    window.setScrollCallback(scroll_callback);
    window.setMouseCallback(mouse_callback);

    Shader objectShader("/usr/OpenGL/models/shaders/vShader.vs", "/usr/OpenGL/models/shaders/fShader.fs");

    Model model(argv[1]);

    glm::mat4 objModel(1.0f);
    glm::mat4 projection(1.0f);

    Texture::flipVertically(true);

    glEnable(GL_DEPTH_TEST);

    // camera.setPosition(glm::vec3(0.0f, 15.0f, 40.0f));

    while (window.isOpen())
    {
        processInputs(window);

        camera.set();
        projection = glm::perspective(glm::radians(camera.zoom()), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        objectShader.use();
        objectShader.setVec3("light.ambient", 0.05f, 0.05f, 0.05f);
        objectShader.setVec3("light.diffuse", 0.4f, 0.4f, 0.4f);
        objectShader.setVec3("light.specular", 0.5f, 0.5f, 0.5f);
        objectShader.setVec3("light.position", 5.0f, 1.0f, 5.0f);
        objectShader.setFloat("material.shininess", 32.0f);
        objectShader.setVec3("viewPos", camera.getPosition());
        objectShader.setMatPtr("view", camera.getPtr());
        objectShader.setMat4("projection", projection);

        objModel = glm::translate(objModel, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        objModel = glm::scale(objModel, glm::vec3(1.0f));

        objectShader.setMat4("model", objModel);

        model.Draw(objectShader);
        window.enableEvents();
    }

    return EXIT_SUCCESS;
}

void framebuffer_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, WIDTH, HEIGHT);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    camera.setMouseControl(xpos, ypos, 0.1f);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.setScroll(xoffset, yoffset);
}

void processInputs(Window &window)
{
    if (glfwGetKey(window.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        window.close();
    }

    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    float cameraSpeed = 2.5 * deltaTime;

    if (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.setPosition(camera.getPosition() + (camera.getDirection() * cameraSpeed));
    }
    if (glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.setPosition(camera.getPosition() - (camera.getDirection() * cameraSpeed));
    }
    if (glfwGetKey(window.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.setPosition(camera.getPosition() - (glm::normalize(glm::cross(camera.getDirection(), camera.getDirectionUp())) * cameraSpeed));
    }
    if (glfwGetKey(window.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.setPosition(camera.getPosition() + (glm::normalize(glm::cross(camera.getDirection(), camera.getDirectionUp())) * cameraSpeed));
    }
}