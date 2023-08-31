//
// Created by comppoter on 8/30/23.
//

#include "commands.hpp"

using namespace glm;


bool createBody(
        // out
        std::map<unsigned short,StellarBody> & planetMap,
        std::map<unsigned short,wrappedObject> & vboMap,

        // in
        const char * objectPath,
        const char * texturePath,

        IDList & idList,

        const std::string& name,
        double mass,
        glm::vec4 position,
        glm::vec4 direction)
{
    unsigned int index = planetMap.size() + 1;
    StellarBody body = StellarBody(name, mass, position, direction, index);

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    bool loaded = loadOBJ(objectPath, vertices, uvs, normals);
    if (loaded) {
        wrappedObject vbo = *new wrappedObject();

        GLuint texture = loadBMP_custom(texturePath);
        vbo.Texture = texture;

        indexVBO(vertices, uvs, normals, vbo.indices, vbo.vertices, vbo.uvs, vbo.normals);
        auto size = vbo.indices.size();

        glGenVertexArrays(1, &vbo.VAO);
        glBindVertexArray(vbo.VAO);

        glGenBuffers(1, &vbo.vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vbo.vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, vbo.vertices.size() * sizeof(glm::vec3), &vbo.vertices[0], GL_STATIC_DRAW);
        glGenBuffers(1, &vbo.uvBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vbo.uvBuffer);
        glBufferData(GL_ARRAY_BUFFER, vbo.uvs.size() * sizeof(glm::vec2), &vbo.uvs[0], GL_STATIC_DRAW);
        glGenBuffers(1, &vbo.normalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vbo.normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, vbo.normals.size() * sizeof(glm::vec3), &vbo.normals[0], GL_STATIC_DRAW);
        glGenBuffers(1, &vbo.elementBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo.elementBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, vbo.indices.size() * sizeof(unsigned int), &vbo.indices[0], GL_STATIC_DRAW);

        planetMap.insert(pair<unsigned short,StellarBody>(index, body));
        vboMap.insert(pair<unsigned short,wrappedObject>(index, vbo));
        return true;
    }
    else {
        return false;
    }
}

void drawObject(
        wrappedObject & vbo,
        IDList & idList,

        glm::mat4 & ProjectionMatrix,
        glm::mat4 & ViewMatrix)
{
    glUseProgram(idList.ProgramID);

    glm::mat4 mvp = ProjectionMatrix * ViewMatrix * vbo.Model;
    glUniformMatrix4fv(idList.MatrixID, 1, GL_FALSE, &mvp[0][0]);
    glUniformMatrix4fv(idList.ModelMatrixID, 1, GL_FALSE, &vbo.Model[0][0]);
    glUniformMatrix4fv(idList.ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);


    glActiveTexture(GL_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, vbo.Texture);
    glUniform1i(idList.TextureID, 0);

//    std::cout<<"Texture bound"<<std::endl;

    glBindVertexArray(vbo.VAO);
//    std::cout<<"VAO bound"<<std::endl;
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo.vertexBuffer);
    glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            nullptr
    );
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo.uvBuffer);
    glVertexAttribPointer(
            1,                                // attribute
            2,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            nullptr                          // array buffer offset
    );
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo.normalBuffer);
    glVertexAttribPointer(
            2,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            nullptr
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo.elementBuffer);
    glDrawElements(
            GL_TRIANGLES,
            vbo.indices.size(),
            GL_UNSIGNED_INT,
            nullptr
    );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}