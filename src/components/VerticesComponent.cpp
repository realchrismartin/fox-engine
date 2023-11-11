#include "VerticesComponent.hpp"
#include "VerticesComponent.hpp"

void VerticesComponent::addVertex(size_t index, const Vertex& vertex)
{
    if (index >= MAX_VERTICES)
    {
        std::cout << "Skipped adding vertex, there are too many already (index too high)" << std::endl;
    }

    m_vertices[index] = vertex;
}

GLvoid* VerticesComponent::getVertices() const
{
    return (GLvoid*)m_vertices;
}
