#include "VerticesComponent.hpp"
#include "VerticesComponent.hpp"

void VerticesComponent::addVertex(size_t index, const Vertex& vertex)
{
    if (index >= MAX_VERTICES)
    {
        std::cout << "Skipped adding vertex, there are too many already (index too high)" << std::endl;
        return;
    }

    m_vertices[index] = vertex;
}

GLvoid* VerticesComponent::getVertices() const
{
    return (GLvoid*)m_vertices;
}

void VerticesComponent::setTransformPoolIndex(size_t vertexCount, size_t transformPoolIndex)
{
    for (size_t i = 0; i < vertexCount; i++)
    {
        m_vertices[i].mvpIndex = (GLfloat)transformPoolIndex;
    }
}
