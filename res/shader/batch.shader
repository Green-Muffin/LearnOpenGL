@shader: vertex

#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in float slot_idx;
out vec2 v_tex_coord;
flat out int v_slot_idx;
void main()
{
    gl_Position = vec4(position, 1.0f);
    v_slot_idx = int(slot_idx);
    v_tex_coord = tex_coord;
}

@shader: fragment

#version 330 core

layout(location = 0) out vec4 frag_color;
in vec2 v_tex_coord;
flat in int v_slot_idx;
uniform sampler2D u_texture[2];

void main()
{
	int idx = clamp(v_slot_idx, 0, 1);
	vec4 tex_color = texture(u_texture[idx], v_tex_coord);
	frag_color = tex_color;
}