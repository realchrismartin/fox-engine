#include "IndicesComponent.hpp"
#include "src/components/IndicesComponent.hpp"

void IndicesComponent::addOffsetIndices(size_t offset, const std::vector<GLuint>& localIndices)
{
    if (localIndices.size() > (size_t)MAX_INDICES)
    {
        std::cout << "Skipped adding index, there are too many for the array size" << std::endl;
        return;
    }

	for (size_t i = 0 ; i < localIndices.size(); i++)
	{
        m_indices[i] = localIndices[i] + (GLuint)offset;
	}
}

GLvoid* IndicesComponent::getIndices() const
{
    return (GLvoid*)m_indices;
}

