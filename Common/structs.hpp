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

    string modelName;
};

struct x44Matrix {
    int n_col = 4;
    int n_row = 4;
    std::vector<float> positions{};
    x44Matrix() = default;
    explicit x44Matrix(float allVal) {
        positions = std::vector<float>(16, allVal);
    };
    x44Matrix(glm::vec4 c1, glm::vec4 c2, glm::vec4 c3, glm::vec4 c4) {
        positions = {
                c1.x, c2.x, c3.x, c4.x,
                c1.y, c2.y, c3.y, c4.y,
                c1.z, c2.z, c3.z, c4.z,
                c1.w, c2.w, c3.w, c4.w
        };
    }
    float get(int col, int row) {return positions.at(col*n_col + row);};
    void set(int col, int row, float val) {positions[col*n_col + row] = val;};

};
struct translationx44Matrix : x44Matrix{
    translationx44Matrix(float x, float y, float z) : x44Matrix() {
         this->positions = x44Matrix(
                glm::vec4(1,0,0,x),
                glm::vec4(0,1,0,y),
                glm::vec4(0,0,1,z),
                glm::vec4(0,0,0,1)).positions;
    };
};
struct scalex44Matrix : x44Matrix{
    scalex44Matrix(float x, float y, float z) : x44Matrix() {
        this->positions = x44Matrix(
                glm::vec4(x, 0, 0, 0),
                glm::vec4(0, y, 0, 0),
                glm::vec4(0, 0, z, 0),
                glm::vec4(0, 0, 0, 1)).positions;
    }
};

#endif //INC_3DASTRO_STRUCTS_HPP
