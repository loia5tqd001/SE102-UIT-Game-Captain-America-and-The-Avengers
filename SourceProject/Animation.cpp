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
	auto size= sprite.GetFrameSize(curFrame);
	Debug::Out("UpdatecurFrame: ", size.left, size.top, size.right, size.bottom);
}

void Animation::SetCusFrameHoldTime(UINT frameNumber, float holdTime)
{
	assert(float(holdTime) > 0.0f);
	cusHoldTimeMap[frameNumber] = holdTime;
}

void Animation::Render(const Vector2& pos, const Vector2& vtScale, int alpha) const
{
	auto size= sprite.GetFrameSize(curFrame);
	Debug::Out("RendercurFrame: ", size.left, size.top, size.right, size.bottom);
	sprite.Draw(pos, curFrame, vtScale, alpha);
}

bool Animation::IsDoneCycle()
{
	if (!doneCycle) return false;
	auto size= sprite.GetFrameSize(curFrame);
	Debug::Out("When done: ", size.left, size.top, size.right, size.bottom);

	doneCycle = false, holdingTime = 0.0f, curFrame = nFrames - 1;
	size= sprite.GetFrameSize(curFrame);
	Debug::Out("When after done: ", size.left, size.top, size.right, size.bottom);
	return true;	
}

Rect Animation::GetFrameSize(const Vector2 & vtScale) const
{
	return sprite.GetFrameSize(curFrame, vtScale);
}


