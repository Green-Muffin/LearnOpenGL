@shader: vertex

#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_mvp;

void main()
{
    gl_Position = u_mvp * vec4(aPos, 1.0);
}

@shader: fragment

#version 330 core
out vec4 FragColor;

uniform vec3 obj_color;
uniform vec3 light_color;

void main()
{
    FragColor = vec4(light_color * obj_color, 1.0);
}