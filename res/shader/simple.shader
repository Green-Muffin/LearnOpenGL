@shader: vertex

#version 330 core

layout(location = 0) in vec4 position;

void main()
{
    gl_Position = vec4(position.x, position.y, position.z, 1.0f);
}

@shader: fragment

#version 330 core

layout(location = 0) out vec4 color;

void main()
{
    color = texColor;
}