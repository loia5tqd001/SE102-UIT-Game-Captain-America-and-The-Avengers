#pragma once

class Animation
{
private:
	const Sprite& sprite;
	const float   holdTime;
	      float   holdingTime = 0.0f;
	const UINT    nFrames = sprite.GetNumberOfFrames();
	      UINT    curFrame = 0u;
	      bool    doneCycle = false;
	      std::unordered_map<UINT, float> cusHoldTimeMap;

	float GetFrameHoldTime(UINT frameNumber)
	{
		if (cusHoldTimeMap.count(frameNumber) == 0)
			return holdTime;
		else
			return cusHoldTimeMap[frameNumber];
	}
public:
	Animation(SpriteId spriteId, float holdTime = std::numeric_limits<float>::infinity());

	void Update(float dt);

	void SetCusFrameHoldTime(UINT frameNumber, float holdTime);

	void EraseCusFrameHoldTime(UINT frameNumber);

	void Render(const Vector2& pos, const Vector2& vtScale = { 1.0f, 1.0f }, int alpha = 255) const;

	float GetCurFrameHoldTime()
	{
		return GetFrameHoldTime(curFrame);
	}

	float GetDefaultHoldTime() { return holdTime; }

	bool IsDoneCycle(); // for some animation aimed to animate one cycle each call
	Rect GetFrameSize(const Vector2& vtScale = { 1.0f, 1.0f }) const;

};

