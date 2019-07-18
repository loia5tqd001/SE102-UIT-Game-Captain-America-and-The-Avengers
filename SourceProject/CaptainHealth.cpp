#include "pch.h"
#include "CaptainHealth.h"

int CaptainHealth::Subtract(int dh)
{
	assert(dh > 0);
	Set( max(0, health - dh) );
	return health;
}

int CaptainHealth::Add(int dh)
{
	assert(dh > 0);
	Set( min(12, health + dh) );
	return health;
}

void CaptainHealth::Set(int h)
{
	assert(h >= 0 && h <= 12);
	health = h;
	// TODO: If health <= 0. Captain.SetState(Die)
}

void CaptainHealth::Draw() const
{
	if (health < 0 || health > 12) ThrowMyException("Captain health is exceed");

	static auto& sprite = Sprites::Get( SpriteId::CaptainHealth );

	if (health > 0)
	{
		sprite.Draw( { 15.0f, 20.0f } );
	}
	if (health > 4)
	{
		sprite.Draw( { 15.0f, 28.0f } );
	}
	if (health > 8)
	{
		sprite.Draw( { 15.0f, 36.0f } );
	}
}

CaptainHealth& CaptainHealth::Instance()
{
	static CaptainHealth instance;
	return instance;
}
