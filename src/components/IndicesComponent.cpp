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

void IndicesComponent::addOffsetIndices(size_t offset, const std::vector<GLuint>& localIndices)
{
	m_indices.clear();

	for (auto& index : localIndices)
	{
		m_indices.push_back(index + (GLuint)offset);
	}
}
