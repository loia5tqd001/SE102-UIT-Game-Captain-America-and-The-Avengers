#include "pch.h"

std::unordered_map<SpriteId, Sprite> Sprites::spriteDictionary;

const auto Sprites::GetFramesFromSpriteInfo(const Json::Value & spriteInfo)
{
	const LONG left         = spriteInfo[2].asInt ();
	const LONG top          = spriteInfo[3].asInt ();
	const UINT spriteWidth  = spriteInfo[4].asUInt();
	const UINT spriteHeight = spriteInfo[5].asUInt();
	const UINT nRects       = spriteInfo[6].asUInt();
	const auto margin       = spriteInfo[7].asInt();

	std::vector<Rect> frames; frames.reserve(nRects);

	for (UINT i = 0; i < nRects; i++)
	{
		static Rect frame;
		if (margin > 0) 
			frame.left = left + (spriteWidth + margin) * i;
		else
			frame.left = left - (spriteWidth + margin) * i;
		frame.top    = top;
		frame.right  = frame.left + spriteWidth;
		frame.bottom = frame.top + spriteHeight;

		frames.emplace_back(frame);
	}

	return frames;
}

void Sprites::LoadResources(const Json::Value& root)
{
	const auto& sprites = root["sprites"];
	for (auto& sprite : sprites)
	{
		const auto spriteId  = (SpriteId)sprite[0].asInt();
		const auto textureId = (TextureId)sprite[1].asInt();

		const auto frames = GetFramesFromSpriteInfo(sprite);

		Sprite sprite(textureId, std::move(frames));
		spriteDictionary.emplace(spriteId, std::move(sprite));
	}
}

const Sprite & Sprites::Get(SpriteId id)
{
	assert(spriteDictionary.count(id) == 1);
	return spriteDictionary.at(id);
}

