#version 410 core
layout (location = 0) in vec3 aPos;
//out vec4 vertexColor;
in vec3 aColor;
in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 TexCoord;

uniform mat4 transform;

void main(){
    gl_Position = transform * vec4(aPos, 1.0);
    //vertexColor = vec4(0.5, 0.0, 0.0, 1.0);
    vertexColor = vec4(aColor, 1.0);
    TexCoord = aTexCoord;

}