#include "IndicesComponent.hpp"
#include "src/components/IndicesComponent.hpp"

const GLvoid* IndicesComponent::getIndices() const
{
	return &m_indices[0];
}

size_t IndicesComponent::getIndexCount() const
{
	return m_indices.size();
}

void IndicesComponent::reset()
{
	m_indices.clear();
}

void IndicesComponent::addIndex(GLuint index)
{
	m_indices.push_back(index);
}
