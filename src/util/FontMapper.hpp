#ifndef FONTMAPPER_HPP
#define FONTMAPPER_HPP

namespace FontMapper
{

	//TODO: assumes the use of one texture atlas.
	inline const glm::vec2 getCharacterTexCoords(const char character)
	{
		switch (character)
		{
			//TODO: add cases
		case 'a':
		default:
			return { 82,84 };
		}
	}

	inline const glm::vec2 getCharacterSpriteSize(const char character)
	{
		switch (character)
		{
			//TODO: add cases
		default:
			return { 18,39 };
		}
	}
}

#endif