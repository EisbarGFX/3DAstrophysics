cmake_minimum_required(VERSION 3.25)
project(3Dastro)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(OpenGlLinkers -lglut -lGLU -lGL)


add_executable(3Dastro
        Graphics/loadShader.cpp
        Graphics/loadShader.hpp
        Common/control.hpp
        Common/control.cpp
        Common/loadOBJ.cpp
        Common/loadOBJ.hpp
        Common/loadDDS.cpp
        Common/loadDDS.hpp
        Physics/Particle.cpp
        Physics/Particle.hpp
        Common/main.cpp
        Common/commands.cpp
        Common/commands.hpp
        Common/structs.hpp
        Physics/Particle.cpp
        Physics/Particle.hpp
        Physics/Cell.cpp
        Physics/Cell.hpp
)

target_link_libraries(3Dastro glfw ${OpenGlLinkers} GLEW)
