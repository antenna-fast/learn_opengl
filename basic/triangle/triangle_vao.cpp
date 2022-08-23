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
                            "void main() {"

                            "gl_Position = vec4(pos, 1);"
                            "}";

std::string fragmentShader = "#version 430\n"
							"out vec4 FragColor;"
                            "void main() {"
							"FragColor = vec4(1, 1, 0, 1);"
							"}";

std::string fragmentShaderLight = "#version 430\n"
							"out vec4 FragColor;"
                            "void main() {"
							"FragColor = vec4(1, 1, 1, 1);"
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
GLuint loadDataInBuffersCube()
{

    float verticesCube[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
	GLuint vboId;
	// allocate buffer space and pass data to it
	glGenBuffers(1, &vboId);  // 1. create a VBO (vertex buffer object)	
	glBindBuffer(GL_ARRAY_BUFFER, vboId);  // 2. bind GL_ARRAY_BUFFER
	// 3. copy the vertex data into buffer memory: 
	// <objectBufferType, sizeof(in byte), realData, GPU memory type>
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);

	// unbind the active buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vboId;
}

GLuint loadDataInBuffersTriangle()
{
	float verticesTriangle[] = { 
					-1, -1, 0,  // left_down
					0, 0, 0,	// right_down
					0, -1, 0,	// right_up
					
					// 0.5, 0.5, 0,	// right_up (rep)
					// -0.5, 0.5, 0,	// left_up
					// -0.5, -0.5, 0,	// left_down
	};
    
	GLuint vboId;
	// allocate buffer space and pass data to it
	glGenBuffers(1, &vboId);  // 1. create a VBO (vertex buffer object)	
	glBindBuffer(GL_ARRAY_BUFFER, vboId);  // 2. bind GL_ARRAY_BUFFER
	// 3. copy the vertex data into buffer memory: 
	// <objectBufferType, sizeof(in byte), realData, GPU memory type>
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTriangle), verticesTriangle, GL_STATIC_DRAW);

	// unbind the active buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vboId;
}

// Initialize and put everything together
GLuint vaoId, lightVAO;
GLuint programId, programIdLight;

void init()
{
	// clear the framebuffer each frame with black color
	glClearColor(0, 0, 0, 0);
	// 1. Data
	GLuint vboIdCube = loadDataInBuffersCube();
    GLuint vboIdTriangle = loadDataInBuffersTriangle();

    // 2. Shader Program
	// 	compile shader code
	GLuint vShaderId = compileShaders(vertexShader, GL_VERTEX_SHADER);  	// Vertex Shader
	GLuint fShaderId = compileShaders(fragmentShader, GL_FRAGMENT_SHADER);  // Fragment Shader
	//	create shader program and link
	programId = linkProgram(vShaderId, fShaderId);

	// 3. Set Vertex attribute: VAO
	glGenVertexArrays(1, &vaoId); 	// (1)Generate VAO
	glBindVertexArray(vaoId);  		// (2)Bind it so that rest of vao operations affect this vao
	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vboIdCube);
	// Set vertex Attribute
	GLuint posAttributePosition = glGetAttribLocation(programId, "pos");
	glVertexAttribPointer(posAttributePosition, 3, GL_FLOAT, false, 5*sizeof(float), (void*)0);
	// Enable this attribute array linked to 'pos'
	glEnableVertexAttribArray(posAttributePosition);

	// second VAO
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vboIdTriangle);
	// Set vertex attribute 
	glVertexAttribPointer(posAttributePosition, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void *)0);
	glEnableVertexAttribArray(posAttributePosition);

	// glBindVertexArray(vaoId);
	glBindVertexArray(lightVAO);  // triangle

}

// Function that does the drawing
// glut calls this function whenever it needs to redraw
void display()
{
	// clear the color buffer before each drawing
	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(programId);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // line mode

	// draw triangles starting from index 0 and using 3 indices
	glBindVertexArray(vaoId);  // cube
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(lightVAO);  // triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

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
