#ifndef VERTICESCOMPONENT_HPP
#define VERTICESCOMPONENT_HPP

#include "src/graphics/Vertex.hpp"

class VerticesComponent
{
public:
	void addVertex(size_t index, const Vertex& vertex);
	GLvoid* getVertices() const;
	static const int MAX_VERTICES = 3000;

private:
	Vertex m_vertices[MAX_VERTICES];
};

#endif