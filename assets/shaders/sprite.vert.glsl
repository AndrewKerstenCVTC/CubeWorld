#version 430

#extension GL_ARB_explicit_uniform_location : enable

layout (location = 0) uniform mat4 projection;
layout (location = 4) uniform mat4 model;

layout (location = 0) in vec4 vertex;

out vec2 texcoord;

void main()
{
	texcoord = vertex.zw;
	
	gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}
