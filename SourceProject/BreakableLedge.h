#pragma once

class BreakableLedge : public VisibleObject
{
private:
	bool isSeparated = false;
	std::chrono::steady_clock::time_point seperatedTime;

public:
	BreakableLedge(Vector2 pos);
	void Update(float dt, const std::vector<GameObject*>& coObjects = {}) override;
	RectF GetBBox() const override;
	void OnCollideWithCap();
};

