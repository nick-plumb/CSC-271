#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shaderprogram.h"
#include "stb_image.h"
#include "mesh.h"
#include "camera.h"
//decided to make another material struct to make my idea work
struct Material{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

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
            glm::vec3( 2.0f,  0.0f,  0.0f),
            glm::vec3(4.0f,  0.0f,  0.0f),
            glm::vec3(6.0f,  0.0f,  0.0f),
        glm::vec3(0.0f,  2.0f,  0.0f),
        glm::vec3(2.0f,  2.0f,  0.0f),
        glm::vec3(4.0f,  2.0f,  0.0f),
        glm::vec3(6.0f,  2.0f,  0.0f),


    };
//TODO
    //going to try and make a list of material objects to iterate through to make the loop work
    // cant do material structs so ill use vectors and loop through a setuniform function\
    // trying this with my own material struct defined in this main program
    Material emerald = {glm::vec3(0.0215f, 0.1745f, 0.0215f),
        glm::vec3(0.07568f, 0.61424f, 0.07568f),
        glm::vec3(0.633f, 0.727811f, 0.633f),
        0.6f
    };
    Material pearl = {glm::vec3(0.25f, 0.20725f, 0.20725f),
        glm::vec3(1.0f, 0.829f, 0.829f),
        glm::vec3( 0.296648f,  0.296648f,  0.296648f),
        0.088f
    };
    Material bronze = {
        glm::vec3(0.2125f, 0.1275f, 0.054f),
        glm::vec3(0.714f, 0.4284f, 0.18144f),
        glm::vec3(0.393548f, 0.271906f, 0.166721f),
        0.2f
    };
    Material gold = {
        glm::vec3(0.24725f, 0.1995f, 0.0745f),
        glm::vec3(0.75164f, 0.60648f, 0.22648f),
        glm::vec3(0.628281f, 0.555802f, 0.366065f),
        0.4f
    };
    Material cyanPlastic = {
        glm::vec3(0.0f, 0.1f, 0.06f),
        glm::vec3(0.0f, 0.50980392f, 0.50980392f),
        glm::vec3(0.50196078f, 0.50196078f, 0.50196078f),
        0.25f
    };
    Material redPlastic = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.5f, 0.0f, 0.0f),
        glm::vec3(0.7f, 0.6f, 0.6f),
        0.25f
    };
    Material greenRubber = {
        glm::vec3(0.0f, 0.05f, 0.0f),
        glm::vec3(0.4f, 0.5f, 0.4f),
        glm::vec3(0.04f, 0.7f, 0.04f),
        0.078125f
    };
    Material yellowRubber = {
        glm::vec3(0.05f, 0.05f, 0.0f),
        glm::vec3(0.5f, 0.5f, 0.4f),
        glm::vec3(0.7f, 0.7f, 0.04f),
        0.078125f
    };
    Material ruby = {
        glm::vec3(0.1745f, 0.01175f, 0.01175f),
        glm::vec3(0.61424f, 0.04136f, 0.04136f),
        glm::vec3(0.727811f, 0.626959f, 0.626959f),
        0.6f
    };
    Material jade = {
        glm::vec3(0.135f, 0.2225f, 0.1575f),
        glm::vec3(0.54f, 0.89f, 0.63f),
        glm::vec3(0.316228f, 0.316228f, 0.316228f),
        0.1f
    };
    Material obsidian = {
        glm::vec3(0.05375f, 0.05f, 0.06625f),
        glm::vec3(0.18275f, 0.17f, 0.22525f),
        glm::vec3(0.332741f, 0.328634f, 0.346435f),
        0.3f
    };
    Material chrome = {
        glm::vec3(0.25f, 0.25f, 0.25f),
        glm::vec3(0.4f, 0.4f, 0.4f),
        glm::vec3(0.774597f, 0.774597f, 0.774597f),
        0.6f
    };
    Material brass = {
        glm::vec3(0.329412f, 0.223529f, 0.027451f),
        glm::vec3(0.780392f, 0.568627f, 0.113725f),
        glm::vec3(0.992157f, 0.941176f, 0.807843f),
        0.217949f
    };
    Material silver = {
        glm::vec3(0.19225f, 0.19225f, 0.19225f),
        glm::vec3(0.50754f, 0.50754f, 0.50754f),
        glm::vec3(0.508273f, 0.508273f, 0.508273f),
        0.4f
    };
    Material turquoise = {
        glm::vec3(0.1f, 0.18725f, 0.1745f),
        glm::vec3(0.396f, 0.74151f, 0.69102f),
        glm::vec3(0.297254f, 0.30829f, 0.306678f),
        0.1f
    };
    Material copper = {
        glm::vec3(0.19125f, 0.0735f, 0.0225f),
        glm::vec3(0.7038f, 0.27048f, 0.0828f),
        glm::vec3(0.256777f, 0.137622f, 0.086014f),
        0.1f
    };




    Material materials[] = {
        cyanPlastic, redPlastic, greenRubber, yellowRubber,
        emerald, pearl, bronze, gold
    };
    Material materials2[] = {
    silver, copper, chrome, brass,
    turquoise, obsidian, jade, ruby
    };

    //TODO
    //make only 8 positions
    //edit the for loop

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        camera.ProcessKeyboard(window, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        containerShaderProgram.use();
        //moved light source back to light all boxes better
        glm::vec3 lightPos(3.0f, 1.0f, 8.0f);
        glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjection(SCR_WIDTH/SCR_LENGTH);

         containerShaderProgram.setUniform("lightPos", lightPos);
        // containerShaderProgram.setUniform("objectColor", objectColor);
         containerShaderProgram.setUniform("lightColor", lightColor);
        containerShaderProgram.setUniform("viewPos", camera.Position);

        containerShaderProgram.setUniform("model", model);
        containerShaderProgram.setUniform("view", view);
        containerShaderProgram.setUniform("projection", projection);

        // containerShaderProgram.setUniform("material.ambient",glm::vec3(1.0f, 0.5f, 0.31f));
        // containerShaderProgram.setUniform("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        // containerShaderProgram.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        // containerShaderProgram.setUniform("material.shininess", 32.0f);

        // containerShaderProgram.setUniform("pointLight.position", lightPos);
        // containerShaderProgram.setUniform("pointLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
        // containerShaderProgram.setUniform("pointLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        // containerShaderProgram.setUniform("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        //
        // containerShaderProgram.setUniform("pointLight.constant", 1.0f);
        // containerShaderProgram.setUniform("pointLight.linear", 0.09f);
        // containerShaderProgram.setUniform("pointLight.quadratic", 0.032f);

        // containerShaderProgram.setUniform("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        // containerShaderProgram.setUniform("dirLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
        // containerShaderProgram.setUniform("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        // containerShaderProgram.setUniform("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        // containerShaderProgram.bindTexture2D("material.diffuse", std::string(ASSET_DIR)+"container2.png", 0, false);
        // containerShaderProgram.bindTexture2D("material.specular", std::string(ASSET_DIR)+"container2_specular.png", 1, false);

        //removed the textures
//TODO
        //edit this loop to make 8 cubes and change the material each time, if possible
        //make it stop rotating as well - done
        for (unsigned int i = 0; i < 8 ;i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            // float angle = 20.0f * i;
            // model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            containerShaderProgram.setUniform("model", model);
            containerShaderProgram.setUniform("material.diffuse", materials2[i].diffuse);
            containerShaderProgram.setUniform("material.ambient", materials2[i].ambient);
            containerShaderProgram.setUniform("material.specular", materials2[i].specular);
            containerShaderProgram.setUniform("material.shininess", materials2[i].shininess * 128.0f);
            container.draw();
        }


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