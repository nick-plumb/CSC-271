#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shaderprogram.h"
#include "stb_image.h"
#include "mesh.h"
#include "camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

int SCR_WIDTH = 800;
int SCR_LENGTH = 600;
Camera camera;

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

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_LENGTH, "Hello CG", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    //create a ShaderProgram object shaderProgram
//    std::string vertPath = std::string(SHADER_DIR) + "vertex.vert";
//    std::string fragPath = std::string(SHADER_DIR) + "fragment.frag";
//    ShaderProgram shaderProgram(vertPath, fragPath);

    std::string vertPath = std::string(SHADER_DIR) + "cube_vertex.vert";
    std::string fragPath = std::string(SHADER_DIR) + "container_fragment.frag";
    ShaderProgram containerShaderProgram(vertPath, fragPath);

    vertPath = std::string(SHADER_DIR) + "cube_vertex.vert";
    fragPath = std::string(SHADER_DIR) + "light_fragment.frag";
    ShaderProgram lightShaderProgram(vertPath, fragPath);

    Mesh container(std::string(ASSET_DIR) + "box.obj", containerShaderProgram.getID());
    Mesh light(std::string(ASSET_DIR) + "box.obj", lightShaderProgram.getID());

    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        camera.ProcessKeyboard(window, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable((GL_STENCIL_TEST));
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        containerShaderProgram.use();
        glm::vec3 lightPos(1.5f, 0.0f, 0.0f);
        glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        //Replace this line ^
        glm::mat4 projection = camera.GetProjection(SCR_WIDTH/SCR_LENGTH);


        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);

        // containerShaderProgram.setUniform("lightPos", lightPos);
        //containerShaderProgram.setUniform("objectColor", objectColor);
        // containerShaderProgram.setUniform("lightColor", lightColor);
        containerShaderProgram.setUniform("viewPos", camera.Position);

        containerShaderProgram.setUniform("model", model);
        containerShaderProgram.setUniform("view", view);
        containerShaderProgram.setUniform("projection", projection);

        // containerShaderProgram.setUniform("material.ambient",glm::vec3(1.0f, 0.5f, 0.31f));
        containerShaderProgram.setUniform("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        containerShaderProgram.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        containerShaderProgram.setUniform("material.shininess", 32.0f);

        containerShaderProgram.setUniform("pointLight.position", lightPos);
        containerShaderProgram.setUniform("pointLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
        containerShaderProgram.setUniform("pointLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        containerShaderProgram.setUniform("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        containerShaderProgram.setUniform("pointLight.constant", 1.0f);
        containerShaderProgram.setUniform("pointLight.linear", 0.09f);
        containerShaderProgram.setUniform("pointLight.quadratic", 0.032f);

        containerShaderProgram.setUniform("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        containerShaderProgram.setUniform("dirLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
        containerShaderProgram.setUniform("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        containerShaderProgram.setUniform("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        containerShaderProgram.bindTexture2D("material.diffuse", std::string(ASSET_DIR)+"container2.png", 0, false);
        containerShaderProgram.bindTexture2D("material.specular", std::string(ASSET_DIR)+"container2_specular.png", 1, false);

        for (unsigned int i = 0; i < 10; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            containerShaderProgram.setUniform("model", model);
            container.draw();
        }
        glDisable(GL_DEPTH_TEST);
        glStencilMask(0x00);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        lightShaderProgram.use();
        lightShaderProgram.setUniform("view", view);
        lightShaderProgram.setUniform("projection", projection);
        float scale = 1.1f;
        for (unsigned int i = 0; i < 10; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            model = glm::scale(model, glm::vec3(scale, scale, scale));
            lightShaderProgram.setUniform("model", model);
            container.draw();
        }
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);

        lightShaderProgram.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));

        lightShaderProgram.setUniform("model", model);
        lightShaderProgram.setUniform("view", view);
        lightShaderProgram.setUniform("projection", projection);


        light.draw();

        glfwSwapInterval(1);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    std::cout<< width << " " << height << std::endl;
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (camera.firstMouse)
    {
        camera.lastX = xpos;
        camera.lastY = ypos;
        camera.firstMouse = false;
    }

    float xoffset = xpos - camera.lastX;
    float yoffset = camera.lastY - ypos; // reversed since y-coordinates go from bottom to top

    camera.lastX = xpos;
    camera.lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}