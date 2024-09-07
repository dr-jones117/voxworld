#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 myColor;
out vec4 outPos;

uniform float offset;

void main()
{
    gl_Position = vec4(aPos.x + offset, aPos.y + offset, aPos.z + offset, 1.0f);
    outPos = gl_Position;
    myColor = aColor;
};

#shader fragment
#version 330 core

in vec3 myColor;
in vec4 outPos;

layout(location = 0) out vec4 FragColor;

void main()
{
    FragColor = vec4(outPos.xyz, 1.0f);
};