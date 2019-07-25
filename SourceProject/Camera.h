#pragma once

class Camera
{
private:
	Vector2 pos; 
	const UINT width  = Window::Instance().GetWidth (); 
	const UINT height = Window::Instance().GetHeight();
	float leftAppend, rightAppend;
	class Captain* cap;
	State state = State::Camera_Normal;

	void CenterTo(const Vector2& center);
	void CenterAround(const Vector2& center);
	void OnRecover();
public:
	void SetMainCharacter(Captain* cap);
	void FollowMainCharacter();
	void MoveTo(const Vector2& newPos);
	void MoveBy(const Vector2& dist);
	void ClampWithin(const RectF& theBox);
	void SetState(State state);
	const RectF GetBBox() const;

	Vector2 GetPositionInViewPort(const Vector2& objPos) const;

private:
	Camera() = default;
	Camera(const Camera&) = delete;

public:
	static Camera& Instance();
};
