#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout(location = 0)in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 " gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0f);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
                                   "}\0";

int main()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Triangle", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create the Window!\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialise GLAD!\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    unsigned int vertexShader;
    // create the vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // compiling the shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // shader compile info
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << "\n";
    }
    // shader compile info - END

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Fragment Shader Compile Info
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << "\n";
    }

    // Fragment Shader Compile Info - END

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //  Shader Program Compile Info
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n"
                  << infoLog << "\n";
    }

    //  Shader Program Compile Info - END
    // deleting shaders since we don't need them anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // range between -1 and 1
    float vertices[] = {
        // first triangle
        -0.9f, -0.5f, 0.0f, // left
        -0.0f, -0.5f, 0.0f, // right
        -0.45f, 0.5f, 0.0f, // top
                            // second triangle
        0.0f, -0.5f, 0.0f,  // left
        0.9f, -0.5f, 0.0f,  // right
        0.45f, 0.5f, 0.0f   // top
    };

    // vertex buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // vertex Array Object (Easier binding VBO and pointers to it)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // copy data into the GPU Memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 1.The first parameter specifies which vertex attribute we want to configure.
    // Remember that we specified the location of the position vertex attribute in
    // the vertex shader with layout (location = 0). This sets the location of the
    // vertex attribute to 0 and since we want to pass data to this vertex attribute, we pass in 0.
    // 2.The next argument specifies the size of the vertex attribute. The vertex
    // attribute is a vec3 so it is composed of 3 values.
    // 3.The third argument specifies the type of the data which
    // is GL_FLOAT (a vec* in GLSL consists of floating point values).
    // 4.The next argument specifies if we want the data to be normalized. If we're inputting
    // integer data types (int, byte) and we've set this to GL_TRUE, the integer data is
    // normalized to 0 (or -1 for signed data) and 1 when converted to float. This is not
    // relevant for us so we'll leave this at GL_FALSE.
    // 5.The fifth argument is known as the stride and tells us the space between consecutive
    // vertex attributes. Since the next set of position data is located exactly 3 times
    // the size of a float away we specify that value as the stride. Note that since we know
    // that the array is tightly packed (there is no space between the next vertex attribute value)
    // we could've also specified the stride as 0 to let OpenGL determine the stride
    // (this only works when values are tightly packed). Whenever we have more vertex attributes
    // we have to carefully define the spacing between each vertex attribute but we'll get to
    // see more examples of that later on.
    // 6.The last parameter is of type void* and thus requires that weird cast. This is the
    // offset of where the position data begins in the buffer. Since the position data is at
    // the start of the data array this value is just 0.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind the VAO
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // @param3 : #vertices
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // release the memory
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return EXIT_SUCCESS;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}