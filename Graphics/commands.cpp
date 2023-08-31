//
// Created by comppoter on 8/30/23.
//

#include "commands.hpp"

using namespace glm;


void _genBuffers(
        wrappedObject & vbo,
        std::vector<glm::vec3> & vertices,
        std::vector<glm::vec2> & uvs,
        std::vector<glm::vec3> & normals
        )
{
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
}

void _genBuffers(
        wrappedObject & vbo
        )
{
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
}

bool createObject(
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
    const char * cachePath = "cache/";
    const char * cacheExtension = ".cch";

    std::string objPath = objectPath;
    std::string objName = objPath.substr(0, objPath.find('.'));

    unsigned int index = planetMap.size() + 1;
    StellarBody body = StellarBody(name, mass, position, direction, index);

    std::cout<< "Load Started at:" << glfwGetTime()<<std::endl;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    bool loaded = loadOBJ(objectPath, vertices, uvs, normals);

    if (vertices.size() >= 32000) {
        wrappedObject vbo = *new wrappedObject();
        vbo.modelName = objName.c_str();
        GLuint texture = loadBMP_custom(texturePath);
        vbo.Texture = texture;



        for (auto & it : vboMap) { // Load from existing VBO
            if (it.second.modelName == objName) {
                vbo.indices = it.second.indices;
                vbo.vertices = it.second.vertices;
                vbo.uvs = it.second.uvs;
                vbo.normals = it.second.normals;

                _genBuffers(vbo);
                planetMap.insert(pair<unsigned short,StellarBody>(index, body));
                vboMap.insert(pair<unsigned short,wrappedObject>(index, vbo));

                goto END;
            }
        }


        for (auto f : filesystem::directory_iterator(cachePath)) { // Load From Cache
            if (f.path().filename().string() == (objName+cacheExtension)) {
                FILE * file = fopen(f.path().c_str(), "r");
                char line[256];

                fgets(line, sizeof(line), file);
                std::regex firstLine("\\|\\|\\+ [a-zA-Z0-9 ]+ \\+\\|\\|\n");
                std::cmatch result;
                if (std::regex_match(line, result, firstLine)) {

                    while (fgets(line, sizeof(line), file)) {
                        std::string stLine = line;
                        stLine.erase(std::remove(stLine.begin(), stLine.end(), '\n'), stLine.end());
                        if (line[0] == 'i') {
                            vbo.indices.push_back(std::stoi(stLine.substr(1, stLine.size())));
                        }
                        else if(line[0] == 'v') {
                            unsigned short pos1 = stLine.find_first_of(',');
                            unsigned short pos2 = stLine.find_last_of(',');
                            std::string ftOne = stLine.substr(1,pos1-1);
                            std::string ftTwo = stLine.substr(pos1+1, pos2-pos1);
                            vbo.vertices.emplace_back(
                                    std::stof(ftOne),
                                    std::stof(ftTwo),
                                    std::stof(stLine.substr(pos2+1, stLine.size())));
                        }
                        else if(line[0] == 'u') {
                            unsigned short pos1 = stLine.find_first_of(',');
                            std::string ftOne = stLine.substr(1,pos1-1);
                            vbo.uvs.emplace_back(
                                    std::stof(ftOne),
                                    std::stof(stLine.substr(pos1+1, stLine.size())));
                        }
                        else if(line[0] == 'n') {
                            unsigned short pos1 = stLine.find_first_of(',');
                            unsigned short pos2 = stLine.find_last_of(',');
                            std::string ftOne = stLine.substr(1,pos1-1);
                            std::string ftTwo = stLine.substr(pos1+1, pos2-pos1);
                            vbo.normals.emplace_back(
                                    std::stof(ftOne),
                                    std::stof(ftTwo),
                                    std::stof(stLine.substr(pos2+1, stLine.size())));
                        }
                    }

                    fclose(file);
                    std::cout << vbo.indices.size() << std::endl;

                    _genBuffers(vbo);
                    planetMap.insert(pair<unsigned short,StellarBody>(index, body));
                    vboMap.insert(pair<unsigned short,wrappedObject>(index, vbo));

                    goto END;
                }
                else {
                    std::cout<<"Loaded File Is Not a Cache File!"<<std::endl;
                    std::cout<<line<<" : "<<result[0]<<std::endl;
                    fclose(file);
                    filesystem::remove((cachePath + objName + cacheExtension).c_str());
                    goto FALLBACK;
                }
            }
        }

        std::cout<<"File not found"<<std::endl;
        FILE * file = fopen((cachePath + objName + cacheExtension).c_str() , "w*");

        _genBuffers(vbo, vertices, uvs, normals);
        planetMap.insert(pair<unsigned short,StellarBody>(index, body));
        vboMap.insert(pair<unsigned short,wrappedObject>(index, vbo));

        fprintf(file, "||+ %s +||\n", objName.c_str());
        fprintf(file, "VT\n");
        for (auto & ind : vbo.indices) {
            fprintf(file, "i%d\n", ind);
        }
        for (auto & vert : vbo.vertices) {
            fprintf(file, "v%f,%f,%f\n", vert.x, vert.y, vert.z);
        }
        for (auto & uv : vbo.uvs) {
            fprintf(file, "u%f,%f\n", uv.x, uv.y);
        }
        for (auto & norm : vbo.normals) {
            fprintf(file, "n%f,%f,%f\n", norm.x, norm.y, norm.z);
        }
        fclose(file);

    }
    else {
        if (loaded) {
            FALLBACK:
            wrappedObject vbo = *new wrappedObject();
            vbo.modelName = objName.c_str();
            GLuint texture = loadBMP_custom(texturePath);
            vbo.Texture = texture;

            _genBuffers(vbo, vertices, uvs, normals);

            planetMap.insert(pair<unsigned short, StellarBody>(index, body));
            vboMap.insert(pair<unsigned short, wrappedObject>(index, vbo));

        } else {
            return false;
        }
    }

    END:
    std::cout << "File Loaded at: " << glfwGetTime() << std::endl;
    return true;
}


void moveObject(
        wrappedObject & vbo,
        IDList & idList,

        glm::vec4 moveCommand)
{
    glUseProgram(idList.ProgramID);

    if (moveCommand.w == 0) {
        vbo.Model = glm::translate(vbo.Model,
                                   glm::vec3(moveCommand.x,moveCommand.y,moveCommand.z));
    }
    if (moveCommand.w == 1) {
        vbo.Model = glm::translate(glm::mat4(1.0f),
                                   glm::vec3(moveCommand.x,moveCommand.y,moveCommand.z));
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


    glBindVertexArray(vbo.VAO);
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