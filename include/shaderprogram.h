//
// Created by nhplu on 9/9/2025.
//

#ifndef CSC271_SHADERPROGRAM_H
#define CSC271_SHADERPROGRAM_H
#include <string>
#include <glad/glad.h>

class ShaderProgram {
public:
    ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
    ~ShaderProgram();

    void use() const;
    GLuint getID() const;

    void destroy();

private:
    GLuint ID;
    bool isDestroyed = false;

    std::string loadShaderSource(const std::string& filePath);
    static GLuint compileShader(const std::string& source, GLenum shaderType);
    void linkProgram(GLuint vertexShader, GLuint fragmentShader);
};

#endif //CSC271_SHADERPROGRAM_H
