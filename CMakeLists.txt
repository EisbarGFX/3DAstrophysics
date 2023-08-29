cmake_minimum_required(VERSION 3.25)
project(3Dastro)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(OpenGlLinkers -lglut -lGLU -lGL)


add_executable(3Dastro loadShader.cpp loadShader.hpp main.cpp control.hpp control.cpp loadOBJ.cpp loadOBJ.hpp loadDDS.cpp loadDDS.hpp Physics/StellarBody.cpp Physics/StellarBody.hpp Graphics/main.cpp)

target_link_libraries(3Dastro glfw ${OpenGlLinkers} GLEW)
