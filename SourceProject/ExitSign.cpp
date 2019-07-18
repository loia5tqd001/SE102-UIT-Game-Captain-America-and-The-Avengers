#include "pch.h"
#include "ExitSign.h"

ExitSign::ExitSign() 
{
	ResetNewStage();
}

void ExitSign::ResetNewStage()
{
	isKrystalCollected = false;
	timePassedAfterKrystalCollected = 0.0f;
}

void ExitSign::Draw()
{
	if (!isKrystalCollected) return;

	static auto& sprite = Sprites::Get(SpriteId::ExitSign);

	if ((timePassedAfterKrystalCollected += GameTimer::Dt()) <= 0.96f)
	{
		auto exitPos = krystalPos - Vector2{ 9.0f, 25.0f };
		auto drawablePos = Camera::Instance().GetPositionInViewPort(exitPos);
		sprite.Draw(drawablePos);
	}
	else {
		static int count = 0;
		static bool shouldDraw = false;
		if (count <= 10) {
			count++;
		}
		else {
			shouldDraw = !shouldDraw;
			count = 0;
		}
		static auto drawablePos = Vector2{ 30.0f, 20.0f };
		if (shouldDraw) sprite.Draw( drawablePos );
	}
}

void ExitSign::KrystalCollected(const Vector2& krystalPos)
{
	isKrystalCollected = true;
	timePassedAfterKrystalCollected = 0.0f;
	this->krystalPos = krystalPos;
}

ExitSign& ExitSign::Instance()
{
	static ExitSign instance;
	return instance;
}

