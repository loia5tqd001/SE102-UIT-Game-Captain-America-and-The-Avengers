#include "pch.h"

void Camera::MoveTo(const Vector2 & newPos)
{
	pos.x = floor(newPos.x);
	pos.y = floor(newPos.y);
}

void Camera::MoveBy(const Vector2 & dist)
{
	MoveTo(pos + dist);
}

void Camera::CenterTo(const Vector2 & center)
{
	MoveTo(center);
	MoveBy({ -float(width / 2), -float(height / 2) });
}

void Camera::CenterAround(const Vector2& center, float radius)
{
	ClampWithin( RectF{
		center - Vector2{ radius + width / 2.0f, radius + height / 2.0f}, 
		width + UINT(2 * radius), 
		height + UINT(2 * radius)}
	);		
}

void Camera::ClampWithin(const RectF& theBox)
{
	Utils::Clamp(pos.x, theBox.left, theBox.right - width);
	Utils::Clamp(pos.y, theBox.top, theBox.bottom - height);
	pos.x = floor(pos.x), pos.y = floor(pos.y);
}

const RectF Camera::GetBBox() const
{
	return { pos, width, height };
}

Vector2 Camera::GetPositionInViewPort(const Vector2 & objPos) const
{
	return objPos - pos;
}

Camera& Camera::Instance()
{
	static Camera instance;
	return instance;
}

