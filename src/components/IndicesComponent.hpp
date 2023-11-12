#ifndef INDICESCOMPONENT_HPP
#define INDICESCOMPONENT_HPP

class IndicesComponent
{
public:
	GLvoid* getIndices() const;
	void addOffsetIndices(size_t offset, const std::vector<GLuint>& localIndices);
	static const int MAX_INDICES = 3000;
private:
	GLuint m_indices[MAX_INDICES];

};
#endif