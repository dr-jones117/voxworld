#shader vertex
#version 330 core

layout(location = 0) in vec3 uPos;

void main()
{
    gl_Position = vec4(uPos.x, uPos.y, uPos.z, 1.0f);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

void main()
{
    FragColor = vec4(0.9f, 0.9f, 0.1f, 1.0f);
};