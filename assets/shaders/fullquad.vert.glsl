#version 430

layout (location = 0) in vec4 vertex;

out vec2 texcoord;

void main()
{
	texcoord = vertex.zw;

	gl_Position = vec4(vertex.xy, 0.0, 1.0);
}
