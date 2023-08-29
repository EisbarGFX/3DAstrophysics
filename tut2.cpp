//// Include standard headers
//#include <stdio.h>
//#include <stdlib.h>
//#include <iostream>
//#include <filesystem>
//
//#define nullptr 0
//
//// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
//#include <GL/glew.h>
//
//// Include GLFW
//#include <GLFW/glfw3.h>
//GLFWwindow* window;
//
//// Include GLM
//#include <glm/glm.hpp>
//#include <sqltypes.h>
//
//using namespace glm;
//
//#include "loadShader.hpp"
//
//
//int main() {
//    if( !glfwInit() ) {
//        fprintf(stderr, "Failed to initialize GLFW\n");
//        return -1;
//    }
//
//    glfwWindowHint(GLFW_SAMPLES, 4); // 4x AA
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2); // OpenGL 2.1
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
////    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
////    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    //open a window
//    window = glfwCreateWindow(1024, 768, "Test", nullptr, nullptr);if (window == nullptr) {
//        fprintf( stderr, "Failed to open GLFW window\n");
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window); // Init GLEW
//
////    glewExperimental=true;
//    glewInit();
//    if (glewInit() != GLEW_OK) {
//        fprintf(stderr, "Failed to initialize GLEW\n");
//        return -1;
//    }
//
//    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
//
//    glClearColor(0.0f,0.0f,0.4f,0.0f);
//
//    //Create shaders
//    GLuint programID = LoadShaders("VertexShader.glsl", "FragmentShader.glsl");
//
//    GLuint VertexArrayID;
//    glGenVertexArrays(1, &VertexArrayID);
//    glBindVertexArray(VertexArrayID);
//
//
//    // An array of 3 vectors which represents 3 vertices
//    static const GLfloat g_vertex_buffer_data[] = {
//            -1.0f, -1.0f, 0.0f,
//            1.0f, -1.0f, 0.0f,
//            0.0f,  1.0f, 0.0f,
//    };
//
//    // This will identify our vertex buffer
//    GLuint vertexbuffer;
//// Generate 1 buffer, put the resulting identifier in vertexbuffer
//    glGenBuffers(1, &vertexbuffer);
//// The following commands will talk about our 'vertexbuffer' buffer
//    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//// Give our vertices to OpenGL.
//    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
//
//
//
//    // MAIN LOOP
//    do{
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        // 1st attribute buffer : vertices
//        glEnableVertexAttribArray(0);
//        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//        glVertexAttribPointer(
//                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//                3,                  // size
//                GL_FLOAT,           // type
//                GL_FALSE,           // normalized?
//                0,                  // stride
//                (void*)nullptr            // array buffer offset
//        );
//
//        glUseProgram(programID);
//
//        // Draw the triangle !
//        glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
//        glDisableVertexAttribArray(0);
//
//
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//    while (
//        glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
//        glfwWindowShouldClose(window) == 0);
//}