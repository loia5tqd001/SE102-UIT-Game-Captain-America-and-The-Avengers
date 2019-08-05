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

	void Render(const Vector2& pos, const Vector2& vtScale = { 1.0f, 1.0f }, D3DCOLOR color = Colors::White) const;

	void Reset() { holdingTime = 0.0f;curFrame = 0u; }

	float GetCurFrameHoldTime()
	{
		return GetFrameHoldTime(curFrame);
	}

	bool OnAnimation() { return ((holdingTime > 0) && (holdingTime < holdTime)); }

	float GetDefaultHoldTime() { return holdTime; }
	UINT GetCurFrameIndex() { return curFrame; }
	bool IsDoneCycle(bool resetCurFrame = true); // for some animation aimed to animate one cycle each call
	Rect GetFrameSize(const Vector2& vtScale = { 1.0f, 1.0f }) const;
	float GetHoldingTime() { return holdingTime; }
};

