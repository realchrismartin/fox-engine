#ifndef INDICESCOMPONENT_HPP
#define INDICESCOMPONENT_HPP

class IndicesComponent
{
public:
	const GLvoid* getIndices() const;
	size_t getIndexCount() const;
	void addOffsetIndices(size_t offset, const std::vector<GLuint>& localIndices);
private:
	std::vector<GLuint> m_indices;
};
#endif