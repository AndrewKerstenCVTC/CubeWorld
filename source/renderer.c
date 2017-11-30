#include "renderer.h"

#include <stdio.h>

#include <GL/glew.h>
#include <GLKMath.h>

#include "camera.h"
#include "chunk.h"
#include "mesher.h"
#include "shader.h"
#include "sprite.h"
#include "texture.h"
#include "utility.h"
#include "window.h"
#include "world.h"

#include "simplex.h"

#define DRAW_COMMANDS_CHUNKS_MAX (64 * 64 * 8)

struct DrawArraysCommand
{
	GLuint count;
	GLuint instance_count;
	GLuint first;
	GLuint base_instance;
};

struct renderer
{
	GLuint framebuffer;
	GLuint framebuffer_color;
	GLuint framebuffer_depth;

	SHADER shader_fullquad;
	SHADER shader_sprite;
	SHADER shader_terrain;

	GLuint vao_chunks;
	GLuint vbo_chunks_matrices;
	GLuint vbo_chunks_commands;
	GLKMatrix4 vbo_chunk_matrices_buffer[DRAW_COMMANDS_CHUNKS_MAX];
	GLuint vao_fullquad;
	GLuint vbo_fullquad;
	GLuint vao_sprite;
	GLuint vbo_sprite;

	GLKMatrix4 matrix_projection2D;
	GLKMatrix4 matrix_projection3D;
	GLKMatrix4 matrix_view;

	struct DrawArraysCommand draw_commands_chunks[DRAW_COMMANDS_CHUNKS_MAX];
	size_t draw_commands_chunks_count;

	TEXTURE texture_noise;
	struct sprite sprite_noise;
};

static struct renderer renderer = { 0 };

static void render_sprite(struct sprite sprite);

bool renderer_initialize(void)
{
	// Initialize OpenGL
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		return false;
	}

	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("RENDERER: %s\n", glGetString(GL_RENDERER));

	if (!GLEW_VERSION_4_5)
	{
		printf("OpenGL 4.5 API is not available.\n");
	}

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClearColor(0.49f, 0.75f, 0.93f, 1.0f);
	glClearColor(0.0f, 0.75f, 1.00f, 1.0f);

	// Initialize framebuffer
	glGenFramebuffers(1, &renderer.framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, renderer.framebuffer);

	glGenTextures(1, &renderer.framebuffer_color);
	glBindTexture(GL_TEXTURE_2D, renderer.framebuffer_color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderer.framebuffer_color, 0);

	glGenRenderbuffers(1, &renderer.framebuffer_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, renderer.framebuffer_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1920, 1080);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderer.framebuffer_depth);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Initialize shaders
	renderer.shader_fullquad = shader_create(SHADER_ASSET_DIRECTORY "fullquad.vert.glsl", SHADER_ASSET_DIRECTORY "fullquad.frag.glsl");
	renderer.shader_sprite = shader_create(SHADER_ASSET_DIRECTORY "sprite.vert.glsl", SHADER_ASSET_DIRECTORY "sprite.frag.glsl");
	renderer.shader_terrain = shader_create(SHADER_ASSET_DIRECTORY "terrain.vert.glsl", SHADER_ASSET_DIRECTORY "terrain.frag.glsl");

	// Initialize VAO and VBOs for rendering chunks.
	glGenVertexArrays(1, &renderer.vao_chunks);
	glBindVertexArray(renderer.vao_chunks);

	mesher_setup_opengl_buffer();

	glGenBuffers(1, &renderer.vbo_chunks_matrices);
	glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo_chunks_matrices);

	glBufferData(GL_ARRAY_BUFFER, DRAW_COMMANDS_CHUNKS_MAX * sizeof(GLKMatrix4), NULL, GL_STREAM_DRAW);

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(GLKMatrix4), (GLvoid*)(sizeof(GLKVector4) * 0));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(GLKMatrix4), (GLvoid*)(sizeof(GLKVector4) * 1));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(GLKMatrix4), (GLvoid*)(sizeof(GLKVector4) * 2));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(GLKMatrix4), (GLvoid*)(sizeof(GLKVector4) * 3));

	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	glGenBuffers(1, &renderer.vbo_chunks_commands);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, renderer.vbo_chunks_commands);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(struct DrawArraysCommand) * DRAW_COMMANDS_CHUNKS_MAX, NULL, GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Initialize fullquad geometry.  Two float for position and two floats for
	// texture coordinates packed int a single vec4.
	GLfloat fullquad_vertices[] = {
		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
	};

	glGenVertexArrays(1, &renderer.vao_fullquad);

	glGenBuffers(1, &renderer.vbo_fullquad);
	glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo_fullquad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fullquad_vertices), fullquad_vertices, GL_STATIC_DRAW);

	glBindVertexArray(renderer.vao_fullquad);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Initialize sprite geometry.  Two floats for position and two floats for
	// texture coordinates packed into a single vec4.
	GLfloat sprite_vertices[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
	};

	glGenVertexArrays(1, &renderer.vao_sprite);

	glGenBuffers(1, &renderer.vbo_sprite);
	glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo_sprite);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sprite_vertices), sprite_vertices, GL_STATIC_DRAW);

	glBindVertexArray(renderer.vao_sprite);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Initialize projection matrix
	renderer.matrix_projection2D = GLKMatrix4MakeOrtho(0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f, -1.0f);
	renderer.matrix_projection3D = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(70.0f), window_aspect(), 0.1f, 1024.0f);

	// Camera
	camera_init();

	// Draw command stuff
	renderer.draw_commands_chunks_count = 0;

	// Noise experiments
	GLubyte noise_buffer[512 * 512];

	struct osn_context* noise;
	simplex(0, &noise);

	for (int y = 0; y < 512; y++)
	{
		for (int x = 0; x < 512; x++)
		{
			double dx = (double)x;
			double dy = (double)y;

			dx *= 0.007;
			dy *= 0.007;

			double v = simplex2(noise, dx, dy);

			//v = fabs(v);

			v = (v + 1.0) / 2.0;

			//v = pow(v, 2.0);

			noise_buffer[y * 512 + x] = (GLubyte)(v * 255.0);
		}
	}

	glGenTextures(1, &renderer.texture_noise);
	glBindTexture(GL_TEXTURE_2D, renderer.texture_noise);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, noise_buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);

	renderer.sprite_noise = sprite_create(0, 0, 512, 512, renderer.texture_noise);

	return true;
}

void renderer_update(void)
{
	camera_update();
	camera_look_through(&renderer.matrix_view);
}

void renderer_render(void)
{
	// Phase 1: Render the game into the 1920 x 1080 framebuffer
	glViewport(0, 0, 1920, 1080);
	glBindFramebuffer(GL_FRAMEBUFFER, renderer.framebuffer);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// Render the chunks.
	shader_use(renderer.shader_terrain);
	glBindVertexArray(renderer.vao_chunks);

	glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo_chunks_matrices);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLKMatrix4) * renderer.draw_commands_chunks_count, renderer.vbo_chunk_matrices_buffer);

	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, renderer.vbo_chunks_commands);
	glBufferSubData(GL_DRAW_INDIRECT_BUFFER, 0, sizeof(struct DrawArraysCommand) * renderer.draw_commands_chunks_count, renderer.draw_commands_chunks);

	glMultiDrawArraysIndirect(GL_TRIANGLES, NULL, renderer.draw_commands_chunks_count, 0);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);

	renderer.draw_commands_chunks_count = 0;

	glBindVertexArray(0);
	shader_use(SHADER_NULL);

	render_sprite(renderer.sprite_noise);

	// Phase 2: Render the 1920 x 1080 framebuffer scaled to the screen size
	glViewport(0, 0, window_width(), window_height());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);

	shader_use(renderer.shader_fullquad);
	glBindVertexArray(renderer.vao_fullquad);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderer.framebuffer_color);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	shader_use(SHADER_NULL);

	glEnable(GL_DEPTH_TEST);
}

void render_chunk(struct chunk* chunk)
{
	if (chunk->mesh == NULL)
	{
		return;
	}

	GLKMatrix4 matrix_mv = GLKMatrix4Multiply(renderer.matrix_view, chunk->transform.matrix);
	GLKMatrix4 matrix_mvp = GLKMatrix4Multiply(renderer.matrix_projection3D, matrix_mv);

	// Frustum culling.
	/*
	GLKVector4 frustum_planes[6];

	// Left plane.
	frustum_planes[0].x = matrix_mvp.m03 + matrix_mvp.m00;
	frustum_planes[0].y = matrix_mvp.m13 + matrix_mvp.m10;
	frustum_planes[0].z = matrix_mvp.m23 + matrix_mvp.m20;
	frustum_planes[0].w = matrix_mvp.m33 + matrix_mvp.m30;

	// Right plane.
	frustum_planes[1].x = matrix_mvp.m03 - matrix_mvp.m00;
	frustum_planes[1].y = matrix_mvp.m13 - matrix_mvp.m10;
	frustum_planes[1].z = matrix_mvp.m23 - matrix_mvp.m20;
	frustum_planes[1].w = matrix_mvp.m33 - matrix_mvp.m30;

	// Bottom plane.
	frustum_planes[2].x = matrix_mvp.m03 + matrix_mvp.m01;
	frustum_planes[2].y = matrix_mvp.m13 + matrix_mvp.m11;
	frustum_planes[2].z = matrix_mvp.m23 + matrix_mvp.m21;
	frustum_planes[2].w = matrix_mvp.m33 + matrix_mvp.m31;

	// Top plane.
	frustum_planes[3].x = matrix_mvp.m03 - matrix_mvp.m01;
	frustum_planes[3].y = matrix_mvp.m13 - matrix_mvp.m11;
	frustum_planes[3].z = matrix_mvp.m23 - matrix_mvp.m21;
	frustum_planes[3].w = matrix_mvp.m33 - matrix_mvp.m31;

	// Near plane.
	frustum_planes[4].x = matrix_mvp.m03 + matrix_mvp.m02;
	frustum_planes[4].y = matrix_mvp.m13 + matrix_mvp.m12;
	frustum_planes[4].z = matrix_mvp.m23 + matrix_mvp.m22;
	frustum_planes[4].w = matrix_mvp.m33 + matrix_mvp.m32;

	// Far plane.
	frustum_planes[5].x = matrix_mvp.m03 - matrix_mvp.m02;
	frustum_planes[5].y = matrix_mvp.m13 - matrix_mvp.m12;
	frustum_planes[5].z = matrix_mvp.m23 - matrix_mvp.m22;
	frustum_planes[5].w = matrix_mvp.m33 - matrix_mvp.m32;

	for (int i = 0; i < 6; i++)
	{
		int in = 0;
		int out = 0;

		for (int j = 0; j < 8; j++)
		{
			float d =
				frustum_planes[i].x * chunk->corners[j].x +
				frustum_planes[i].y * chunk->corners[j].y +
				frustum_planes[i].z * chunk->corners[j].z +
				frustum_planes[i].w;

			if (d < 0)
			{
				out++;
			}
			else
			{
				in++;
			}

			if (in && out)
			{
				break;
			}
		}

		if (in == 0)
		{
			// Cull the chunk.
			return;
		}
	}
	*/

	// Add the draw call.
	renderer.vbo_chunk_matrices_buffer[renderer.draw_commands_chunks_count] = matrix_mvp;

	struct DrawArraysCommand* command = &renderer.draw_commands_chunks[renderer.draw_commands_chunks_count];

	command->first = chunk->mesh->first;
	command->count = chunk->mesh->count;
	command->instance_count = 1;
	command->base_instance = renderer.draw_commands_chunks_count;

	renderer.draw_commands_chunks_count++;
}

void render_sprite(struct sprite sprite)
{
	// model matrix
	GLKMatrix4 matrix_model = GLKMatrix4Identity;
	matrix_model = GLKMatrix4TranslateWithVector3(matrix_model, sprite.position);
	matrix_model = GLKMatrix4ScaleWithVector3(matrix_model, sprite.size);

	// select shader and vao
	shader_use(renderer.shader_sprite);
	glBindVertexArray(renderer.vao_sprite);

	// matrices
	glUniformMatrix4fv(0, 1, GL_FALSE, (const GLfloat*)&renderer.matrix_projection2D);
	glUniformMatrix4fv(4, 1, GL_FALSE, (const GLfloat*)&matrix_model);

	// textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite.texture);

	// draw
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	// deselect shader and vao
	glBindVertexArray(0);
	shader_use(SHADER_NULL);
}