#pragma once

class Camera
{
private:
	Vector2 pos; 
	const UINT width  = Window::Instance().GetWidth (); 
	const UINT height = Window::Instance().GetHeight();
	RectF lockRegion = {}; // always lock camera if lock region != {}

public:
	void  MoveTo(const Vector2& newPos);
	void  MoveBy(const Vector2& dist);
	void  CenterTo(const Vector2& center);
	void  CenterAround(const Vector2& center, float radius = 14.0f);
	void  ClampWithin(const RectF& theBox);
	const RectF GetBBox() const;
	void SetLockRegion(const RectF& lock);

	Vector2 GetPositionInViewPort(const Vector2& objPos) const;

private:
	Camera() = default;
	Camera(const Camera&) = delete;

public:
	static Camera& Instance();
};
