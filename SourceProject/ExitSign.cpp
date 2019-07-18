#include "pch.h"
#include "ExitSign.h"

ExitSign::ExitSign() : VisibleObject(State::Destroyed, {}) // state is not used
{
	ResetNewStage();
}

void ExitSign::ResetNewStage()
{
	isKrystalCollected = false;
	nFramesAfterKrystalCollected = 0;
}

void ExitSign::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	if (!isKrystalCollected) return;

	nFramesAfterKrystalCollected++;
}

void ExitSign::Render() const
{
	if (!isKrystalCollected) return;

	static auto& sprite = Sprites::Get(SpriteId::ExitSign);

	if (nFramesAfterKrystalCollected <= 45)
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
	nFramesAfterKrystalCollected = 0;
	this->krystalPos = krystalPos;
}

ExitSign& ExitSign::Instance()
{
	static ExitSign instance;
	return instance;
}

