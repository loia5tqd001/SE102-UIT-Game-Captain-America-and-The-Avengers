#include "pch.h"

Animation::Animation(SpriteId spriteId, float holdTime) :
	sprite(Sprites::Get(spriteId)),
	holdTime(holdTime)
{
	if (holdTime == 0.0f)
	{
		assert(nFrames == 1);
		doneCycle = true;
	}
}

void Animation::Update(float dt)
{
	holdingTime += dt;

	while (holdingTime >= GetCurFrameHoldTime(curFrame))
	{
		holdingTime -= GetCurFrameHoldTime(curFrame);
		curFrame++;
		if (curFrame >= nFrames)
		{
			doneCycle = true;
			curFrame = 0;
		}
	}
}

void Animation::SetCusFrameHoldTime(UINT frameNumber, float holdTime)
{
	assert(float(holdTime) > 0.0f);
	cusHoldTimeMap[frameNumber] = holdTime;
}

void Animation::Render(const Vector2& pos, const Vector2& vtScale, int alpha) const
{
	sprite.Draw(pos, curFrame, vtScale, alpha);
}

bool Animation::IsDoneCycle()
{
	if (!doneCycle) return false;

	else doneCycle = false, curFrame = 0u, holdingTime = 0.0f;
	return true;	
}

Rect Animation::GetFrameSize(const Vector2 & vtScale) const
{
	return sprite.GetFrameSize(curFrame, vtScale);
}


