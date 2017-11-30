#version 430

in vec2 texcoord;

uniform sampler2D texture0;

out vec4 fragColor;

void main()
{
	fragColor = texture(texture0, texcoord);
}
