#version 410 core
out vec4 FragColor2;
in vec4 vertex2Color;
void main() {
    FragColor2 = vertex2Color;
}