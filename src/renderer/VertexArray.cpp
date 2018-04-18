#include <cstddef>       // offsetof
#include "VertexArray.hpp"
#include "constants.hpp" // ATTRIB_...

VertexArray::VertexArray(const std::vector<Vertex> &vertices)
{
    glCreateVertexArrays(1, &m_id);
    glBindVertexArray(m_id);

    // Members
    // (not initalized in mem-list to keep consistency with the delegating constructor)
    m_vertexCount = vertices.size(); //
    m_indexCount = 0;

    // VertexBuffer
    glGenBuffers(1, &m_vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
    size_t bufSize = vertices.size() * sizeof(Vertex);
    glBufferData(GL_ARRAY_BUFFER, bufSize, vertices.data(), GL_STATIC_DRAW);

    // VertexBuffer's Attributes
    size_t stride = sizeof(Vertex);
    GLvoid *offsetPosition = reinterpret_cast<GLvoid *>(offsetof(Vertex, position));
    GLvoid *offsetNormal = reinterpret_cast<GLvoid *>(offsetof(Vertex, normal));
    GLvoid *offsetColor = reinterpret_cast<GLvoid *>(offsetof(Vertex, color));
    GLvoid *offsetTexCoord = reinterpret_cast<GLvoid *>(offsetof(Vertex, texCoord));

    glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, stride, offsetPosition);
    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, stride, offsetNormal);
    glVertexAttribPointer(ATTRIB_COLOR, 4, GL_BYTE, GL_TRUE, stride, offsetColor);
    glVertexAttribPointer(ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, stride, offsetTexCoord);

    glEnableVertexAttribArray(ATTRIB_POSITION);
    glEnableVertexAttribArray(ATTRIB_NORMAL);
    glEnableVertexAttribArray(ATTRIB_COLOR);
    glEnableVertexAttribArray(ATTRIB_TEX_COORD);
}

VertexArray::VertexArray(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices)
    : VertexArray{vertices}
{
    /* NOTE: Assumes that the delegated constructor has created and bound the VertexArray at this point */

    // Members
    // (not initialized in mem-list due to delegating constructor)
    m_indexCount = indices.size();

    // IndexBuffer
    glGenBuffers(1, &m_indexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
    size_t bufSize = indices.size() * sizeof(GLuint);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufSize, indices.data(), GL_STATIC_DRAW);
}

VertexArray::~VertexArray()
{
    glDeleteBuffers(1, &m_vertexBufferId);
    glDeleteBuffers(1, &m_indexBufferId);
    glDeleteVertexArrays(1, &m_id);
}