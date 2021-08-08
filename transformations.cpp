#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>

void printVector(glm::vec3);

int main()
{
    // initialised to identity matrix
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f), vec1(vec);
    glm::mat4 trans = glm::mat4(1.0f);
    // translation
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    printVector(vec);

    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
    vec1 = trans * vec1;
    printVector(vec);
    return EXIT_SUCCESS;
}

void printVector(glm::vec3 vec)
{
    std::cout << "vec (" << vec.x << "," << vec.y << "," << vec.z << ")\n";
}