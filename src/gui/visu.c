#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <complex.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "../core/fourier.h"
#include "../utils/utils.h"


void error_callback(int error, const char* description) {
	fprintf(stderr, "Error %d: %s\n", error, description);
}

uint8_t horizontal_arrow = 0;
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	UNUSED(mods);
	UNUSED(scancode);

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		horizontal_arrow += 1;
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		horizontal_arrow -= 1;
	}
}

void run_window(
	GLFWwindow *window,
	const double *data,
	const size_t n,
	const uint8_t mode
) {
	UNUSED(mode);

	GLint width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float ratio = 1.0/height*width;

	double x_coord[n];
	float vertices[3*n];
	double min_y = (double)INT64_MAX;
	double max_y = 0.0;
	for (size_t i = 0; i < n; i++) {
		x_coord[i] = width*(i+1)*1.0/(n+1);
		if (data[i] < min_y) min_y = data[i];
		if (data[i] > max_y) max_y = data[i];
	}

	for (size_t i = 0; i < n; i++) {
		// const GLdouble y = data[i];
		// const GLdouble mapped_y = (y-min_y)/(max_y-min_y)*height;
		// glVertex2d(x_coord[i], mapped_y);
		UNUSED(x_coord[i]);
		double angle = i*2*M_PI/(n-1);
		double radius = data[i]/100+0.6;
		// printf("%lf, %lf, %lf, %lf\n", radius, data[i], max_y, min_y);
		// radius = 0.8;
		vertices[3*i] = radius*cos(angle);
		vertices[3*i+1] = radius*sin(angle);
		vertices[3*i+2] = 0.0;

		if (ratio > 1.0) vertices[3*i] /= ratio;
		else if (ratio < 1.0) vertices[3*i+1] *= ratio;
	}

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

	glViewport(0, 0, width, height);

	glClearColor(0.5, 0.5, 0.5, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	// printf("max: %lf\n", max_y);
	// glPointSize(20.0);
	glDrawArrays(GL_LINE_STRIP, 0, n);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

int main() {
	glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		perror("NIOOOOOON FUCK INIT");
		return 1;
	}

	int window_width = 800; int window_height = 400;
	GLFWwindow *window = glfwCreateWindow(window_width, window_height, "Hello there", NULL, NULL);
	if (!window) {
		perror("fuck window");
		return 1;
	}
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
		
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fprintf(stderr, "Failed to initialize GLAD\n");
		return -1;
	}
	glfwSwapInterval(2);

	// float vertices[] = {
	// 	0.5, -0.5, 0.0,
	// 	-0.5, -0.5, 0.0,
	// 	0.0, 0.5, 0.0,
	// };

	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}";

	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}";


	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	
	size_t n;
	int sample_rate;
	float *sine = read_giodio_file("../song.giodio", (int*)&n, &sample_rate);
	// size_t R = 100;
	// float rand_freq[R];
	// float rand_weight[R];
	// for (size_t j = 0; j < R; j++) {
	// 	float freq = rand()*20000.0/RAND_MAX;
	// 	rand_freq[j] = freq;
	// 	rand_weight[j] = rand()*1.0/RAND_MAX;
	// }
	// for (size_t i = 0; i < n; i++) {
	// 	float x = i*1.0/sample_rate;
	// 	sine[i] = 0;
	// 	for (size_t j = 0; j < R; j++) {
	// 		float freq = rand_freq[j];
	// 		sine[i] += rand_weight[j]*sin(2*M_PI*freq*x);
	// 	}
	// }
	n = 1000;
	complex double *four = (complex double*)malloc(sizeof(complex double)*n);
	// dftf(sine, four, n);
	// dftf_para(sine, four, n, 16);
	// printf("\n");
	// for (size_t i = 0; i < n; i++) {
	// 	printf("%lf ", cabs(four[i]));
	// }
	// printf("\n");
		
	glBufferData(GL_ARRAY_BUFFER, 3*n*sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	uint8_t mode = 0;
	horizontal_arrow = 4;
	int time_index = 0;
	double start_time = glfwGetTime();
	double *real_data = (double *)malloc(sizeof(double)*n);
	while (!glfwWindowShouldClose(window)) {
		double time = glfwGetTime();
		dftf_para(sine+time_index, four, n, 16);
		mode = horizontal_arrow%6;
		for (size_t i = 0; i < n; i++) {
			if (mode == 0) {
				real_data[i] = creal((sine+time_index)[i]);
			} else if (mode == 1) {
				real_data[i] = cimag((sine+time_index)[i]);
			} else if (mode == 2) {
				real_data[i] = creal(four[i]);
			} else if (mode == 3) {
				real_data[i] = cimag(four[i]);
			} else if (mode == 4) {
				real_data[i] = cabs(four[i]);
			} else if (mode == 5) {
				real_data[i] = carg(four[i]);
			}
		}
		
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glUseProgram(shaderProgram);
		run_window(window, real_data, n, mode);
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		time_index = (glfwGetTime()-start_time)*sample_rate;
		double time_diff = glfwGetTime()-time;
		// printf("%d\n", time_index);
		// printf("%lf\n", (glfwGetTime()-start_time));
		UNUSED(time_diff);
		// printf("WTF %d\n", time_index);
		// printf("%lf %lf\n", 1000.0/sample_rate, time_diff);
		// usleep((5000.0/sample_rate-time_diff)*1e6);
		// printf("%lf\n", time_index*1.0/sample_rate);
	}

	glfwTerminate();
	return 0;
}