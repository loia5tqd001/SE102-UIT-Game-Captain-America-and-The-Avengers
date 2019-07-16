#pragma once

class Textures 
{
private:
	static std::unordered_map<TextureId, LPDIRECT3DTEXTURE9> textureDictionary;
	static void AddTexture(TextureId id, LPCSTR filePath, D3DCOLOR transparentColor);

public:
	static void LoadResources(const Json::Value& root);
	static const LPDIRECT3DTEXTURE9 Get(TextureId id);
};


