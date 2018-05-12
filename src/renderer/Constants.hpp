#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

const GLuint ATTRIB_POSITION = 0;
const GLuint ATTRIB_NORMAL = 1;
const GLuint ATTRIB_TEX_COORD = 2;

const GLuint UNIFORM_MODEL_WORLD = 0;
const GLuint UNIFORM_WORLD_VIEW = 1;
const GLuint UNIFORM_VIEW_SCREEN = 2;
const GLuint UNIFORM_MODEL_VIEW_NORMAL = 3;

const GLuint UNIFORM_LIGHT_POSITION = 4;
const GLuint UNIFORM_LIGHT_COLOR = 5;
const GLuint UNIFORM_CAMERA_POSITION = 6;

const GLuint UNIFORM_MATERIAL_AMBIENT = 7;
const GLuint UNIFORM_MATERIAL_DIFFUSE = 8;
const GLuint UNIFORM_MATERIAL_SPECULAR = 9;
const GLuint UNIFORM_MATERIAL_SHINE = 10;

const GLuint UNIFORM_TIME = 11;

const GLuint UNIFORM_TEXTURE_UNIT = 12;



const double M_HALF_PI = M_PI/2;