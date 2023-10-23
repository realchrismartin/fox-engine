#ifndef UTILS_HPP
#define UTILS_HPP

namespace Utils
{
	void normalize(sf::Vector2f& vec2)
	{
		float length = sqrt((vec2.x * vec2.x) + (vec2.y * vec2.y));

		vec2.x = vec2.x / length;
		vec2.y = vec2.y / length;
	}

}

#endif