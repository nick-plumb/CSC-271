#version 410 core

out vec4 FragColor;
in vec4 vertexColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;

void main(){
    vec4 bricks = texture(texture1, TexCoord/2);
    vec4 face = texture(texture2, TexCoord);


    FragColor = mix(bricks, face, mixValue);
}
