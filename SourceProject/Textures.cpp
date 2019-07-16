#include "pch.h"

std::unordered_map<TextureId, LPDIRECT3DTEXTURE9> Textures::textureDictionary;

void Textures::AddTexture(TextureId id, LPCSTR texturePath, D3DCOLOR transparentColor)
{
	assert(textureDictionary.count(id) == 0);

	D3DXIMAGE_INFO info;
	if (D3DXGetImageInfoFromFile(texturePath, &info) != D3D_OK)
	{
		ThrowMyException("Get image info from file failed:", texturePath);
	}

	LPDIRECT3DTEXTURE9 texture;
	HRESULT result = D3DXCreateTextureFromFileEx(
					 Game::Instance().GetDirect3DDevice(),
					 texturePath,
					 info.Width,
					 info.Height,
					 D3DX_DEFAULT,
					 D3DUSAGE_DYNAMIC,
					 D3DFMT_UNKNOWN,
					 D3DPOOL_DEFAULT,
					 D3DX_DEFAULT,
					 D3DX_DEFAULT,
					 transparentColor,
					 &info,
					 NULL,
					 &texture);

	if (result != D3D_OK) ThrowMyException("Create texture from file failed");
	textureDictionary.emplace(id, texture);
}

// Learn more about jsoncpp: https://github.com/open-source-parsers/jsoncpp
void Textures::LoadResources(const Json::Value& root)
{
	const auto& textures = root["textures"];
	for (auto& texture : textures)
	{
		const auto textureId              = (TextureId)texture[0].asInt();
		const auto texturePath            = texture[1].asCString();
		const auto& transparentColorJson  = texture[2];

		D3DCOLOR    transparentColor;
		if (transparentColorJson.isNumeric()) {
			transparentColor = 0;
		} else {
			const UINT r = transparentColorJson[0].asUInt();
			const UINT g = transparentColorJson[1].asUInt();
			const UINT b = transparentColorJson[2].asUInt();
			transparentColor = D3DCOLOR_XRGB(r, g, b);
		}

		AddTexture(textureId, texturePath, transparentColor);
	}
}

const LPDIRECT3DTEXTURE9 Textures::Get(TextureId id)
{
	assert(textureDictionary.count(id) == 1);
	return textureDictionary.at(id);
}


