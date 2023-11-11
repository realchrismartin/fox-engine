#ifndef INDICESCOMPONENT_HPP
#define INDICESCOMPONENT_HPP

class IndicesComponent
{
public:
	const GLvoid* getIndices() const;
	size_t getIndexCount() const;
	void reset();
	void addIndex(GLuint index);
private:
	std::vector<GLuint> m_indices;
};
#endif