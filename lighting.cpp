#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shader.hpp>
#include <Window.hpp>
#include <Texture.hpp>
#include <Camera.hpp>
#include <string>

int HEIGHT = 600;
int WIDTH = 800;

Camera camera;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_callback(GLFWwindow *window, int width, int height);
void processInputs(Window &window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

glm::vec3 lightPos(1.4f, 1.0f, 2.0f);
int main()
{
    Window window(WIDTH, HEIGHT);
    window.init();
    window.create("lighting");

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

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3(0.0f, 0.0f, -3.0f)};

    GLuint objVAO, lightVAO;
    GLuint VBO;

    glGenVertexArrays(1, &objVAO);
    glGenVertexArrays(1, &lightVAO);

    glBindVertexArray(objVAO);

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    Shader objectShader("phongShaders/vShader.vs", "light/objShaderV2.fs");
    Shader lightShader("phongShaders/vShader.vs", "phongShaders/lightFragShader.fs");

    Texture diffuseMap;
    Texture specularMap;

    diffuseMap.load("container2.png");
    specularMap.load("container2_specular.png");

    glm::mat4 objModel(1.0f);
    glm::mat4 lightModel(1.0f);
    lightModel = glm::translate(lightModel, lightPos);
    lightModel = glm::scale(lightModel, glm::vec3(0.2f));

    glm::mat4 projection(1.0f);

    glEnable(GL_DEPTH_TEST);

    objectShader.use();
    objectShader.setInt("material.diffuse", 0);
    objectShader.setInt("material.specular", 1);
    // objectShader.setInt("material.emission", 2);

    while (window.isOpen())
    {
        processInputs(window);

        camera.set();
        projection = glm::perspective(glm::radians(camera.zoom()), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);

        glClearColor(0.7f, 0.7f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        objectShader.use();
        objectShader.setVec3("viewPos", camera.getPosition());
        objectShader.setFloat("material.shininess", 32.0f);
        // directional light
        objectShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        objectShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        objectShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        objectShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

        for (int i = 0; i < 4; i++)
        {
            std::string index = std::to_string(i);
            objectShader.setVec3(("pointLights[" + index + "].position").c_str(), pointLightPositions[i]);
            objectShader.setVec3(("pointLights[" + index + "].ambient").c_str(), 0.05f, 0.05f, 0.05f);
            objectShader.setVec3(("pointLights[" + index + "].diffuse").c_str(), 0.8f, 0.8f, 0.8f);
            objectShader.setVec3(("pointLights[" + index + "].specular").c_str(), 1.0f, 1.0f, 1.0f);
            objectShader.setFloat(("pointLights[" + index + "].constant").c_str(), 1.0f);
            objectShader.setFloat(("pointLights[" + index + "].linear").c_str(), 0.09);
            objectShader.setFloat(("pointLights[" + index + "].quadratic").c_str(), 0.032);
        }

        objectShader.setVec3("spotLight.position", camera.getPosition());
        objectShader.setVec3("spotLight.direction", camera.getDirection());
        objectShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        objectShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        objectShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        objectShader.setFloat("spotLight.constant", 1.0f);
        objectShader.setFloat("spotLight.linear", 0.09);
        objectShader.setFloat("spotLight.quadratic", 0.032);
        objectShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        objectShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        objectShader.setMatPtr("view", camera.getPtr());
        objectShader.setMat4("projection", projection);

        glActiveTexture(GL_TEXTURE0);
        diffuseMap.bind();
        glActiveTexture(GL_TEXTURE1);
        specularMap.bind();
        glBindVertexArray(objVAO);

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            objectShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        lightShader.use();
        lightShader.setMatPtr("view", camera.getPtr());
        lightShader.setMat4("projection", projection);

        for (unsigned int i = 0; i < 4; i++)
        {

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        window.enableEvents();
    }

    diffuseMap.Delete();
    specularMap.Delete();
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &objVAO);
    glDeleteVertexArrays(1, &lightVAO);

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
