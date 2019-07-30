#pragma once

class MovingLedge : public VisibleObject
{
private:
	Behaviors behavior;

	void OnCircleMoving(float dt);
	void OnDiagonalMoving(float dt);
	void OnHorizontalMoving(float dt);

public:
	MovingLedge(Vector2 pos, Behaviors behavior);
	void UpdateByUpdater(float dt); // we ignore update by grid, only updatebyupdater to update 3 ledges synchronously
	void OnOutOfViewPort() override {} // override to not set to destroyed
	Vector2 GetVelocity() const override;
	RectF GetBBox() const override;
};

