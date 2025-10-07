#include <cmath>
#include <iostream>
#include <math.h>
#include <stb_image.h>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "mesh.h"
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
    ShaderProgram shaderProgram(vertPath, fragPath);
    shaderProgram.use();

    const std::vector<float> vertices = {
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    const std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 3, 2,
    };

    const GLuint prog = shaderProgram.getID();
    const GLint posLoc = glGetAttribLocation(prog, "aPos");
    const GLint colLoc = glGetAttribLocation(prog, "aColor");
    const GLint uvLoc = glGetAttribLocation(prog, "aTexCoord");

    const GLsizei stride = 8 * sizeof(float);
    std::vector<VertexAttribute> attributes = {
        VertexAttribute{static_cast<GLuint>(posLoc),3, GL_FLOAT, GL_FALSE, stride, 0},
        VertexAttribute{static_cast<GLuint>(colLoc),3, GL_FLOAT, GL_FALSE, stride, 3*sizeof(float)},
        VertexAttribute{static_cast<GLuint>(uvLoc),2, GL_FLOAT, GL_FALSE, stride, 6*sizeof(float)}
    };

    std::vector<TextureSpec> textures = {
        TextureSpec{"texture1",0, prog, 0, std::string(ASSET_DIR) + "wall.jpg", false },
        TextureSpec{"texture2", 0, prog, 1, std::string(ASSET_DIR) + "awesomeface.png", true}
    };
    Transformation trans = {"transform", prog, 0, glm::mat4(1.0f)};
    Mesh quad(vertices, indices, attributes, textures, trans);

    // glm::mat4 transformation = glm::mat4(1.0f);
    // transformation = glm::rotate(transformation, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // transformation = glm::scale(transformation, glm::vec3(0.5f, 0.5f, 0.5f));

    // unsigned int tranformLoc = glGetUniformLocation(shaderProgram.getID(), "transform");
    // glUniformMatrix4fv(tranformLoc, 1, GL_FALSE, glm::value_ptr(transformation));

    // glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    // glm::mat4 translation = glm::mat4(1.0f);
    // translation = glm::translate(translation, glm::vec3(1.0f, 1.0f, 0.0f));
    // vec = translation * vec;
    // std::cout << vec.x << " " << vec.y << " " <<  vec.z << " " << vec.w << std::endl;

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 m = glm::mat4(1.0f);
        m = glm::translate(m, glm::vec3(0.5f, -0.5f, 0.0f));
        m = glm::rotate(m, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        quad.Draw(m);

        glfwSwapBuffers(window);
        glfwPollEvents();
        glfwSwapInterval(1);
    }
    quad.cleanup();

    shaderProgram.destroy();
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    std::cout << width << " " << height << std::endl;
}
