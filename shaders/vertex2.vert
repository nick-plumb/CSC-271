#version 410 core
layout (location = 0) in vec3 bPos;
in vec3 bColor;

out vec4 vertex2Color;

void main() {
    gl_Position = vec4(bPos, 1.0);
    vertex2Color = vec4(bColor, 1.0);

}