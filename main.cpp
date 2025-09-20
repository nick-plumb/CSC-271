#include <cmath>
#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderprogram.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);


int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(800, 600, "CSC271", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to open GLFW window.\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }
    std::string vertPath = std::string(SHADER_DIR) + "/vertex.vert";
    std::string fragPath = std::string(SHADER_DIR) + "/fragment.frag";
    std::string vertPath2 = std::string(SHADER_DIR) + "/vertex2.vert";
    std::string fragPath2 = std::string(SHADER_DIR) + "/fragment2.frag";
    ShaderProgram shaderProgram(vertPath, fragPath);
    ShaderProgram shaderProgram2(vertPath2, fragPath2);
    shaderProgram.use();
    shaderProgram2.use();

    float v1[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.1f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.1f, 0.1f, 0.0f, 1.0f, 0.0f, 0.0f,
        };
    float v2[] = {
        -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
    };

    // unsigned int indices[] = {
    //     0, 1, 2,
    //
    //     3, 4, 5
    // };

    // float timeValue = glfwGetTime();
    // float greenValue = (sin(timeValue / 2.0f)) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaderProgram.getID(), "ourColor");
    // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    // vertex buffer object -> reserves a space in gpu memory
    //vertex array buffer object
    unsigned int VBO, VAO, VBO2, VAO2;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Triangle 1
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v1),v1, GL_STATIC_DRAW);
    GLint posAttrib = glGetAttribLocation(shaderProgram.getID(), "aPos");
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(posAttrib);

    GLint colAttrib = glGetAttribLocation(shaderProgram.getID(), "aColor");
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(colAttrib);

    // Triangle 2
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v2),v2, GL_STATIC_DRAW);

    GLint posAttrib2 = glGetAttribLocation(shaderProgram2.getID(), "bPos");
    glVertexAttribPointer(posAttrib2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(posAttrib2);

    GLint colAttrib2 = glGetAttribLocation(shaderProgram2.getID(), "bColor");
    glVertexAttribPointer(colAttrib2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(colAttrib2);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);



    while (!glfwWindowShouldClose(window)) {
        /**
         rendering tasks and input data
         **/
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        glfwSwapInterval(1); //enables vsync so we don't render as many frames as possible


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // float timeValue = glfwGetTime();
        // float greenValue = (sin(timeValue / 2.0f)) + 0.5f;
        // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    // glDeleteBuffers(1, &EBO);

    shaderProgram.destroy();
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    std::cout << width << " " << height << std::endl;
}
