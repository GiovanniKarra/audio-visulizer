#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include "gl_utils.h"
#include <string.h>


int read_file(const char *filename, char *buffer, size_t max_size) {
	FILE *file = fopen(filename, "rb");

	fseek(file, 0, SEEK_END);
	size_t length = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	if (length >= max_size) {
		fclose(file);
		return -1;
	}
	
	fread(buffer, sizeof(char), length, file);
	buffer[length] = '\0';
	fclose(file);

	return length;
}


GLuint create_shader_program(const char *vertex, const char *fragment) {
	int success;
	char infoLog[512];
	
	const size_t MAX_FILE_SIZE = 4096;
	char buffer[MAX_FILE_SIZE];
	int err = read_file(vertex, buffer, MAX_FILE_SIZE);
	if (err == -1) {
		perror("Shader file too large");
		exit(1);
	}

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar*[]){buffer}, NULL);
	glCompileShader(vertexShader);
	
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("VERTEX SHADER COMPILATION ERROR\n");
		printf("%s\n", infoLog);
	};

	err = read_file(fragment, buffer, MAX_FILE_SIZE);
	if (err == -1) {
		perror("Shader file too large");
		exit(1);
	}
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar*[]){buffer}, NULL);
	glCompileShader(fragmentShader);
	
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("FRAGMENT SHADER COMPILATION ERROR\n");
		printf("%s\n", infoLog);
	};

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("SHADER PROGRAM LINK ERROR\n");
		printf("%s\n", infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}


void create_gl_buffers(gl_state *state, size_t vertex_count) {
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBufferData(GL_ARRAY_BUFFER, 3*vertex_count*sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	state->VAO = VAO;
	state->VBO = VBO;
}


void prepare_to_draw(gl_state *state, const float *vertices, size_t size) {
	glBindVertexArray(state->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, state->VBO);
	glUseProgram(state->shaderProgram);

	glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertices);
}

void solid_color_background(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}