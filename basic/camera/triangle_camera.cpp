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

// GLM 3d math lib
#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Vertex Shader(specific vec location)
std::string vertexShader =  "#version 430\n"
                            "layout (location=0) in vec3 pos;"  // location can be found using glGetAttribLocation(programId, "pos");
                            "layout (location=1) in vec2 tex;"  // location can be found using glGetAttribLocation(programId, "pos");

							"out vec2 TexCoord;"

							"uniform mat4 model;"
							"uniform mat4 view;"
							"uniform mat4 projection;"

                            "void main() {"
                            "gl_Position = projection * view * model * vec4(pos, 1);"
                            // "gl_Position = vec4(pos, 1);"
							"TexCoord = tex;"
                            "}";

// Fragment Shader
std::string fragmentShader = "#version 430\n"
							"in vec2 TexCoord;"
							"out vec4 FragColor;"

							"uniform vec4 CPUColor;"  // uniform var, can be set by CPU
							"uniform sampler2D ourTexture;"							
							
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

	const int screenWidth = 800;
	const int screenHeigh = 600;

	glutInitWindowSize(screenWidth, screenHeigh);
	glutInitWindowPosition(100, 100);
	// glutCreateWindow("Triangle Using OpenGL");
	GLuint window = glutCreateWindow("Triangle Using OpenGL");
	
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
	GLuint texture;  // 1. Create texture object	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);  // 2. Bind it

	// Load image using stb_image
	int width, height, nrChannels;
	std::string img_path = "/home/antenna/ssd/texture_data/2022.02.21.14.45.43/run_data/rgb/0_0.png";
	stbi_set_flip_vertically_on_load(true);  // flip y-axis
	unsigned char *data = stbi_load(img_path.c_str(), &width, &height, &nrChannels, 0);
	if(!data){
		std::cout << "Failed to load image" << std::endl;
		return -1;
	}

	// Generate texture using image loaded before
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);  // generate Mipmap
	stbi_image_free(data);  // Free image memory

	// Model Matrix: Transform model into world coordinate
	glm::mat4 model = glm::mat4(1.0f);
	// model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	// View Matrix: Transform world into Camera
	// We want to move camera along z+ 
	glm::mat4 view = glm::mat4(1.0f);
	// view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0));

	// Camera
	// camera position
	glm::vec3 cameraPosision = glm::vec3(0.0f, 0.0f, 3.0f);
	// direction
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);  // look at
	glm::vec3 cameraDirection = glm::normalize(cameraPosision - cameraTarget);  // camera - target
	// right-vector
	// at first, we can define a up vector, then cross-product the up vector and direction vector 
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	// up-axis
	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));

	// Build LookAt matrix as view
	// <cameraPosition, cameraTarget, up>
	view = glm::lookAt(cameraPosision, cameraTarget, up);
	// camera rotation around the model
	float radius = 3.0;

	// Free Move!
	// At first, we define camera system

	// Projection Matrix
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), float(screenWidth / screenHeigh), 0.1f, 1000.0f);

	int i = 0;
	while (1)
	{	i++;
		if(i > 1000) i=0;
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(programId);

		// set CPUColor: uniform
		float greenValue = (sin(i / 6) / 2.0f) + 0.5f;
		std::cout << "running green: " << greenValue << std::endl;
		int vertexColorLocation = glGetUniformLocation(programId, "CPUColor");
		glUniform4f(vertexColorLocation, greenValue, 1.0, 1.0f, 1.0f);  // rgba, uodate uniform must after glUseProgram
		
		// model = glm::rotate(model, glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		
		// Change camera view
		// only translation
		// cameraPosision = cameraPosision + glm::vec3(0.0f, 0.0f, 0.1f);
		
		// rotation around the model
		float camX = radius * cos(i/60.0);
		float camY = 0;
		float camZ = radius * sin(i/60.0);
		// cameraPosision = glm::vec3(camX, camY, camZ);
		// view = glm::lookAt(cameraPosision, cameraTarget, up);

		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraTarget = cameraPosision + cameraFront;

		float cameraSpeed = 0.01;
		// add some keys
	
		cameraPosision += cameraSpeed * cameraFront;
		view = glm::lookAt(cameraPosision, cameraTarget, cameraUp);

		// view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.01f));  // move the whole scene to imitate camera
		// view = glm::rotate(view, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		// set transform Matrix
		// model Matrix
		int modelLoc = glGetUniformLocation(programId, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));  
		// view Matrix
		int viewLoc = glGetUniformLocation(programId, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view)); 
		// projection Matrix
		int projLoc = glGetUniformLocation(programId, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection)); 

		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(vaoId);

		// draw triangles starting from index 0 and using 3 indices
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // line frame mode

		// glDrawArrays(GL_TRIANGLES, 0, 6);
		// glDrawArrays(GL_QUADS, 0, 4);

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
