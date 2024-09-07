#shader vertex
#version 330 core

layout(location = 0) in vec3 uPos;

void main()
{
    gl_Position = (uPos.x, uPos.y, uPos.z, 1.0f);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

void main()
{
    FragColor = (0.5f, 0.5f, 0.7f, 1.0f);
};