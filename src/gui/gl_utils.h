#ifndef GL_UTILS_H
#define GL_UTILS_H

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#include <stdlib.h>

typedef struct {
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLuint shaderProgram;
} gl_state;

GLuint create_shader_program(const char *vertex, const char *fragment);
void create_gl_buffers(gl_state *state, size_t vertex_count);
void prepare_to_draw(gl_state *state, const float *vertices, size_t size);
void solid_color_background(float r, float g, float b, float a);

#endif