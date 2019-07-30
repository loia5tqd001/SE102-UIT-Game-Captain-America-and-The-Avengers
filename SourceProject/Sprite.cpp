#include "pch.h"

Sprite::Sprite(Sprite&& other) noexcept :
	texture( std::move(other.texture) ),
	frames( std::move(other.frames) )
{
}

Sprite::Sprite(TextureId texture, const Rect& frame) :
	texture(Textures::Get(texture)),
	frames(1, frame)
{
}

Sprite::Sprite(TextureId texture, const std::vector<Rect>& frames) :
	texture(Textures::Get(texture)),
	frames(frames) 
{
}

void Sprite::Draw(const Vector2& pos, UINT frameIndex, const Vector2& vtScale, D3DCOLOR color) const
{
	assert(frameIndex <= frames.size());
	Game::Instance().Draw(pos, texture, frames[frameIndex], vtScale, color);
}

Rect Sprite::GetFrameSize(UINT frameIndex, const Vector2 & vtScale) const
{
	assert(frameIndex <= frames.size());
	return frames[frameIndex] * vtScale.GetAbs();
}
