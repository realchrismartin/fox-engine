#ifndef TEXTUREMAPPER_HPP
#define TEXTUREMAPPER_HPP

enum class TextureEnum
{
	SPRITE_SHEET_1
};

namespace TextureMapper
{
	inline glm::vec2 getTextureDimensionsInPixels(TextureEnum texture)
	{
		switch (texture)
		{
			case(TextureEnum::SPRITE_SHEET_1):
			default:
			{
				return { 2500.f,2500.f }; //tODO: not floats... pls
			}
		}
	}
}

#endif