//
// Created by comppoter on 8/26/23.
//

#ifndef INC_3DASTRO_LOADDDS_HPP
#define INC_3DASTRO_LOADDDS_HPP


#include <GL/glew.h>
#include <filesystem>
#include <cstring>

// Load a .BMP file using our custom loader
GLuint loadBMP_custom(const char * imagepath);

GLuint loadDDS(const char * imagepath);


#endif //INC_3DASTRO_LOADDDS_HPP
