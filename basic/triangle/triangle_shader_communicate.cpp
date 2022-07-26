// CPP program to render a triangle using Shaders
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC  // must ahead of GLFW!
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <glm/glm.hpp>  // 3d math lib


// Vertex Shader
std::string vertexShader =  "#version 430\n"
                            "in vec3 pos;"
							"out vec4 vertexColor;"  // set vertex color in vertex shader
                            "void main() {"
                            "gl_Position = vec4(pos.x, pos.y, pos.z, 1);"
                            // "gl_Position = vec4(pos, 1);"
							"vertexColor = vec4(0.5, 0.0, 0.0, 1.0);"  // set vertex color in vertex shader
                            "}";

// // (or specific vec location) Vertex Shader
// std::string vertexShader =  "#version 430\n"
//                             "layout (location=0) in vec3 pos;"  // location can be found using glGetAttribLocation(programId, "pos");
//                             "void main() {"
//                             "gl_Position = vec4(pos.x, pos.y, pos.z, 1);"
//                             // "gl_Position = vec4(pos, 1);"
//                             "}";

// Fragment Shader
// std::string fragmentShader = "#version 430\n"
// 							"void main() {"
// 							"gl_FragColor = vec4(1, 1, 0, 1);"
// 							"}";

std::string fragmentShader = "#version 430\n"
							"out vec4 FragColor;"
							"in vec4 vertexColor;"  // receive vertex color in fragment shader
                            "void main() {"
							// "FragColor = vec4(1, 1, 0, 1);"
							"FragColor = vertexColor;"
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


// Load data in VBO and return the vbo's id
GLuint loadDataInBuffers()
{
    // 0. define the vertex coordinates
	GLfloat vertices[] = { 
						-0.5, -0.5, 0,  // left_down
						0.5, -0.5, 0,	// right_down
						0.5, 0.5, 0,	// right_up                        
	};

	GLuint vboId;

	// allocate buffer space and pass data to it
	// 1. create a VBO (vertex buffer object)
	glGenBuffers(1, &vboId);
	// 2. bind GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	// 3. copy the vertex data into buffer memory
	// <object buffer type, sizeof(in byte), real data, GPU memory type>
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// unbind the active buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vboId;
}

// Initialize and put everything together
void init()
{
	// clear the framebuffer each frame with black color
	glClearColor(0, 0, 0, 0);
	// 1. Prepare data
	GLuint vboId = loadDataInBuffers();
    
    // 2. Create Shader Program
	// 	compile shader code
	GLuint vShaderId = compileShaders(vertexShader, GL_VERTEX_SHADER);  // Vertex Shader
	GLuint fShaderId = compileShaders(fragmentShader, GL_FRAGMENT_SHADER);  // Fragment Shader
	//	create shader program and link
	GLuint programId = linkProgram(vShaderId, fShaderId);

	// 3. Set Vertex attribute
	// Get the 'pos' variable location inside this program
	GLuint posAttributePosition = glGetAttribLocation(programId, "pos");

	// 3. VAO
	GLuint vaoId;
	glGenVertexArrays(1, &vaoId);  	// (1)Generate VAO
	glBindVertexArray(vaoId);  		// (2)Bind it so that rest of vao operations affect this vao

	// 4. set Vertex Attribute Pointer
	// buffer from which 'pos' will receive its data and the format of that data
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	// <location, vector dimension, data type, normaliize?, stride>
	// stride: 0 for automic setting by opengl
	glVertexAttribPointer(posAttributePosition, 3, GL_FLOAT, false, 0, 0);
	// glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);  // cooperate with: layout (location=0) in vertexShader

	// Enable this attribute array linked to 'pos'
	glEnableVertexAttribArray(posAttributePosition);

	// Use this program for rendering.
	glUseProgram(programId);
}

// Function that does the drawing
// glut calls this function whenever it needs to redraw
void display()
{
	// clear the color buffer before each drawing
	// glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // line mode

	// draw triangles starting from index 0 and using 3 indices
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// swap the buffers and hence show the buffers content to the screen
	glutSwapBuffers();
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
	init();
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
