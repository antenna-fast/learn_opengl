// Created by liuyaohua on 2021/10/22.
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC  // must ahead of GLFW!
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// 3d math lib
#include <glm/glm.hpp>  


using namespace std;
using namespace glm;


int main(){
    cout << "OpenGL Test ... " << endl;
    // Initialize GLFW
    if(!glfwInit()){
        cout << "Failed to initialize GLFW!" << endl;
        return -1;
    }
    // Using glfwWindowHint to config GLFW
    // first: option name
    // second: value (int type)
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // We don't want the old OpenGL
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);  // if we don't want to enable resize window
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed

    // Open a window and create its OpenGL context
    GLFWwindow* window;
    window = glfwCreateWindow(1024, 768, "Totorial 01", NULL, NULL);
    if (window == NULL)
    {
        fprintf( stderr, "Failed to open GLFW window\n" );
        glfwTerminate();
        return -1;
    }

    // Initialize GLEW
    glfwMakeContextCurrent(window);
    glewExperimental=true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    do{
        // Draw nothing, see you in tutorial 2 !

        // Swap buffers
        glfwSwapBuffers(window);

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
    glfwWindowShouldClose(window) == 0);

    return 0;
}