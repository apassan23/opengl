#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out vec3 norm;
out vec2 texCoord;
out vec3 fragPos;
out vec3 LightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * vec4(aPos,1.0);
    norm = mat3(transpose(inverse(model))) * aNormal;
    fragPos = vec3(model * vec4(aPos,1.0));
    texCoord = aTex;
}