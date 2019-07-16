#pragma once

class Sprites 
{
private:
	static std::unordered_map<SpriteId, Sprite> spriteDictionary;
	static const auto GetFramesFromSpriteInfo (const Json::Value& spriteInfo);

public:
	static void LoadResources(const Json::Value& root);
	static const Sprite& Get(SpriteId id);
};
