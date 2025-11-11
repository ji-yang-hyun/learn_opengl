#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0); // 반드시 위치 먼저, 그다음 시점이동, 그다음 projection
    ourColor = aColor;
}