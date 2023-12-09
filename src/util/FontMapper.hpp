#ifndef FONTMAPPER_HPP
#define FONTMAPPER_HPP

namespace FontMapper
{
	//TODO: assumes the use of one texture atlas.
	inline const glm::vec2 getCharacterTexCoords(const char character)
	{
		switch (character)
		{
		case 'a':
			return { 282, 142 };
		case 'b':
			return { 302, 136 };
		case 'c':
			return { 97, 183 };
		case 'd':
			return { 2,177 };
		case 'e':
			return { 22,183 };
		case 'f':
			return { 132, 177 };
		case 'g':
			return { 42, 183 };
		case 'h':
			return { 151,12 };
		case 'i':
			return { 59,218 };
		case 'j':
			return { 267,177 };
		case 'k':
			return { 167,177 };
		case 'l':
			return { 111,218 };
		case 'm':
			return { 81,18 };
		case 'n':
			return { 171,18 };
		case 'o':
			return { 251,18 };
		case 'p':
			return { 271,18 };
		case 'q':
			return { 291,18 };
		case 'r':
			return { 114,183 };
		case 's':
			return { 2,59 };
		case 't':
			return { 150,177 };
		case 'u':
			return { 42,59 };
		case 'v':
			return { 82,59 };
		case 'w':
			return { 55,18 };
		case 'x':
			return { 102,59 };
		case 'y':
			return { 242,59 };
		case 'z':
			return { 2,101 };
		case ' ':
			return { 62,183 };
		case '?':
		default:
			return { 42,94 };
		}
	}

	inline const glm::vec2 getCharacterSpriteSize(const char character)
	{
		switch (character)
		{
		case 'a':
			return { 17,17 };
		case 'b':
			return { 17,23 };
		case 'c':
			return { 14,17 };
		case 'd':
			return { 17,23 };
		case 'e':
			return { 17,17 };
		case 'f':
			return { 14,23 };
		case 'g':
			return { 17,23 };
		case 'h':
			return { 17,23 };
		case 'i':
			return { 7,23 };
		case 'j':
			return { 11,29 };
		case 'k':
			return { 14,23 };
		case 'l':
			return { 4,23 };
		case 'm':
			return { 23,17 };
		case 'n':
			return { 17,17 };
		case 'o':
			return { 17,17 };
		case 'p':
			return { 17,23 };
		case 'q':
			return { 17,23 };
		case 'r':
			return { 14,17 };
		case 's':
			return { 17,17 };
		case 't':
			return { 13,23 };
		case 'u':
			return { 17,17 };
		case 'v':
			return { 17,17 };
		case 'w':
			return { 23,17 };
		case 'x':
			return { 17,17 };
		case 'y':
			return { 17,23 };
		case 'z':
			return { 17,17 };
		case ' ':
			return { 15,17 };
		case '?':
		default:
			return { 17,24 };
		}
	}
}

#endif