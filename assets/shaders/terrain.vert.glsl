#version 330

#extension GL_ARB_explicit_uniform_location : enable

const vec3 normal_table[6] = vec3[6](
	vec3(0.0, -1.0, 0.0),
	vec3(0.0, 1.0, 0.0),
	vec3(0.0, 0.0, -1.0),
	vec3(0.0, 0.0, 1.0),
	vec3(-1.0, 0.0, 0.0),
	vec3(1.0, 0.0, 0.0)
);

const float ambient_strength = 0.8;
const vec3 light_color = vec3(1.0, 1.0, 1.0);
const vec3 light_direction = -vec3(0.0, -1.0, 0.0);

//layout (location = 0) uniform mat4 projection;
//layout (location = 4) uniform mat4 modelview;
//layout (location = 8) uniform mat3 normalmat;

layout (location = 0) in ivec4 vertex_position;
layout (location = 1) in vec4 vertex_color;
layout (location = 2) in mat4 matrix_mvp;

out vec4 color;

void main()
{
	vec3 ambient = ambient_strength * light_color;

	// This works for our directional light and the chunks not being rotated, but the normal will need to be transformed with the normal matrix for other lights.
	vec3 diffuse = max(dot(normal_table[vertex_position.w], light_direction), 0.0) * light_color;

	color = vec4((ambient + diffuse) * vertex_color.rgb, 1.0);

	//gl_Position = projection * modelview * vec4(vertex_position.xyz, 1.0);
	//gl_Position = projection * matrix_modelview * vec4(vertex_position.xyz, 1.0);
	gl_Position = matrix_mvp * vec4(vertex_position.xyz, 1.0);
}
