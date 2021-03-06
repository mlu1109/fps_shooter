#pragma once

#include <GL/glew.h>
#include <array>
#include <vector>
#include "Error.hpp"

class Cubemap
{
    GLuint m_id;
    const GLenum m_target = GL_TEXTURE_CUBE_MAP;

  public:
    Cubemap();

    void loadSide(GLenum target, const std::vector<GLubyte> &imageData, GLint internalFormat, GLsizei width, GLsizei height);

    void bind() const { glBindTexture(m_target, m_id); printError("Cubemap::bind"); }
    void destroy() { glDeleteTextures(1, &m_id); }
    void setRepeat() const;
    void setClampToEdge() const;
};