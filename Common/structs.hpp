//
// Created by comppoter on 8/30/23.
//

#ifndef INC_3DASTRO_STRUCTS_HPP
#define INC_3DASTRO_STRUCTS_HPP


#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

struct IDList{
    GLuint MatrixID;
    GLuint ModelMatrixID;
    GLuint ViewMatrixID;
    GLuint TextureID;
    GLuint LightID;
    GLuint ProgramID;
};
struct wrappedObject{
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    GLuint VAO{};
    GLuint Texture{};
    glm::mat4 Model = glm::mat4(1.0f);

    GLuint vertexBuffer{};
    GLuint uvBuffer{};
    GLuint normalBuffer{};
    GLuint elementBuffer{};

    const char * modelName;
};

#endif //INC_3DASTRO_STRUCTS_HPP
