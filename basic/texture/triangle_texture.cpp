#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// CPP program to render a triangle using Shaders
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

#define GLEW_STATIC  // must ahead of GLFW!
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <glm/glm.hpp>  // 3d math lib


// Vertex Shader(specific vec location)
std::string vertexShader =  "#version 430\n"
                            "layout (location=0) in vec3 pos;"  // location can be found using glGetAttribLocation(programId, "pos");
                            "layout (location=1) in vec2 tex;"  // location can be found using glGetAttribLocation(programId, "pos");
							"out vec2 TexCoord;"

                            "void main() {"
                            "gl_Position = vec4(pos, 1);"
							"TexCoord = tex;"
                            "}";

// Fragment Shader
// std::string fragmentShader = "#version 430\n"
// 							"void main() {"
// 							"gl_FragColor = vec4(1, 1, 0, 1);"
// 							"}";

std::string fragmentShader = "#version 430\n"
							"in vec2 TexCoord;"
							"uniform vec4 CPUColor;"  // uniform var, can be set by CPU
							"uniform sampler2D ourTexture;"							
							"out vec4 FragColor;"

                            "void main() {"
							// "FragColor = vec4(1, 1, 0, 1);"  // fixed color
							// "FragColor = CPUColor;"
							"FragColor = texture(ourTexture, TexCoord) * CPUColor;"  // texture: buid-in function<sampler, tex coordinate>
							"}";

// Compile and create shader object and returns its id
GLuint compileShaders(std::string shader, GLenum type)
{
	// 1. create a shader object
	GLuint shaderId = glCreateShader(type);  // type: GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
	if (shaderId == 0) { // Error: Cannot create shader object
		std::cout << "Error creating shaders";
		return 0;
	}

	// 2. Attach source code to this object(shaderId)
    const char* shaderCode = shader.c_str();
	glShaderSource(shaderId, 1, &shaderCode, NULL);  // <shaderObject, numberOfString, ShaderCode, NULL>
	
	// 3. compile the shader object
	glCompileShader(shaderId);

	// 4. check for compilation status
	GLint compileStatus;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);

	if (!compileStatus) { // If compilation was not successful
		int length;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
		char* cMessage = new char[length];

		// Get additional information
		glGetShaderInfoLog(shaderId, length, &length, cMessage);
		std::cout << "Cannot Compile Shader: " << cMessage;
		delete[] cMessage;
		glDeleteShader(shaderId);
		return 0;
	}

	return shaderId;
}

// Creates a shader program containing vertex and fragment shader
// links it and returns its ID
GLuint linkProgram(GLuint vertexShaderId, GLuint fragmentShaderId)
{
    // 1. create a program
	GLuint programId = glCreateProgram();
	if (programId == 0) {
		std::cout << "Error Creating Shader Program";
		return 0;
	}

	// 2. Attach both the shaders to it
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	// 3. Create executable of this program (Link)
	glLinkProgram(programId);

	// 4. Get the link status for this program
    GLint linkStatus;
	glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);

	if (!linkStatus) { // If the linking failed
		std::cout << "Error Linking program";
		glDetachShader(programId, vertexShaderId);
		glDetachShader(programId, fragmentShaderId);
		glDeleteProgram(programId);

		return 0;
	}

	return programId;
}


// Initialize and put everything together
// void init()
GLint init()
{
	// 1. compile shader code
	GLuint vShaderId = compileShaders(vertexShader, GL_VERTEX_SHADER);  // Vertex Shader
	GLuint fShaderId = compileShaders(fragmentShader, GL_FRAGMENT_SHADER);  // Fragment Shader
	// 2. create shader program and link
	GLuint programId = linkProgram(vShaderId, fShaderId);

	return programId;
}


// main function
// sets up window to which we'll draw
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Triangle Using OpenGL");
	glewInit();

	GLint programId = init();

	// Define the data
	GLfloat vertices[] = { 
					-0.5, -0.5, 0, 0.0, 0.0,  // left_down
					0.5, -0.5, 0, 1.0, 0,	// right_down
					0.5, 0.5, 0, 1.0, 1.0,	// right_up
					-0.5, 0.5, 0, 0, 1.0	// left_up
	};
	// EBO indices
	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	GLuint vboId, vaoId, eboId;
	// VAO
	glGenVertexArrays(1, &vaoId); // 1. Generate VAO	
	glBindVertexArray(vaoId);	  // 2. Bind it so that rest of vao operations affect this vao
	
	// VBO
	// allocate buffer space and pass data to it	
	glGenBuffers(1, &vboId);  				// 1. create a VBO (vertex buffer object)	
	glBindBuffer(GL_ARRAY_BUFFER, vboId);  	// 2. bind GL_ARRAY_BUFFER
	// <object buffer type, sizeof(in byte), real data, GPU memory type>
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // 3. copy the vertex data into buffer memory

	// EBO
	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// GLuint posAttributePosition = glGetAttribLocation(programId, "pos");
	// glVertexAttribPointer(posAttributePosition, 3, GL_FLOAT, false, 0, 0);
	// glEnableVertexAttribArray(posAttributePosition);
	// or manually set location:
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 5*sizeof(float), (void*)0);  // cooperate with: layout (location=0) in vertexShader
	glEnableVertexAttribArray(0);  // Enable this attribute array linked to var
	
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 5*sizeof(float), (void*)(3*sizeof(float)));  // cooperate with: layout (location=1) in vertexShader
	glEnableVertexAttribArray(1);  // Enable this attribute array linked to var
	
	// Texture
	GLuint texture;  // 1. create texture object	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);  // 2. bind it

	// Load image using stb_image
	int width, height, nrChannels;
	std::string img_path = "/home/antenna/ssd/texture_data/2022.02.21.14.45.43/run_data/rgb/0_0.png";
	stbi_set_flip_vertically_on_load(true);  // flip y-axis
	unsigned char *data = stbi_load(img_path.c_str(), &width, &height, &nrChannels, 0);
	if(!data){
		std::cout << "Failed to load image" << std::endl;
		return -1;
	}

	// 3. Generate texture using image loaded before
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);  // generate Mipmap
	stbi_image_free(data);  // free image memory

	int i = 0;
	while (1)
	{	i++;
		if(i > 1000) i=0;
		std::cout << "running" << std::endl;
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(programId);

		// set CPUColor: uniform
		float greenValue = (sin(i / 6) / 2.0f) + 0.5f;
		std::cout << "green: " << greenValue << std::endl;
		int vertexColorLocation = glGetUniformLocation(programId, "CPUColor");
		glUniform4f(vertexColorLocation, greenValue, 0.0, 0.0f, 1.0f);  // rgba, uodate uniform must after glUseProgram

		// draw triangles starting from index 0 and using 3 indices
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // line mode
		// glDrawArrays(GL_TRIANGLES, 0, 6);
		// glDrawArrays(GL_QUADS, 0, 4);
		
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(vaoId);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// swap the buffers and hence show the buffers content to the screen
		glutSwapBuffers();
		glfwPollEvents();
	}
	
	glDeleteVertexArrays(1, &vaoId);
	glDeleteVertexArrays(1, &vboId);
	glDeleteVertexArrays(1, &eboId);

	glfwTerminate();
	return 0;
}
