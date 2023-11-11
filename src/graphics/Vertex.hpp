#ifndef VERTEX_HPP
#define VERTEX_HPP

/// @brief Represents one (local) vertex in a model.
struct Vertex
{
	GLfloat x = 0.f;
	GLfloat y = 0.f;
	GLfloat z = 0.f;

	GLfloat s = 0.f;
	GLfloat t = 0.f;
};

#endif