#include "pch.h"

// append area from center of captain. if center of captain goes out of that append, the camera will move
static constexpr auto LEFT_APPEND   =  14.0f;
static constexpr auto TOP_APPEND    =  55.0f; 
static constexpr auto RIGHT_APPEND  =  14.0f;
static constexpr auto BOTTOM_APPEND =  63.0f;
// 50-68: top ok, bottom sucks

void Camera::SetMainCharacter(Captain* cap)
{
	this->cap = cap;
	CenterTo(cap->GetCenter());
	SetState(State::Camera_Normal);
}

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

void Camera::CenterAround(const Vector2& center)
{	
	ClampWithin( RectF(center.x - (rightAppend + width/2.0f) ,
					   center.y - (BOTTOM_APPEND + height/2.0f) ,
					   center.x + (leftAppend + width/2.0f),
					   center.y + (TOP_APPEND + height/2.0f)));
}

void Camera::ClampWithin(const RectF& theBox)
{
	Utils::Clamp(pos.x, theBox.left, theBox.right - width);
	Utils::Clamp(pos.y, theBox.top, theBox.bottom - height);
	pos.x = floor(pos.x), pos.y = floor(pos.y);
}

void Camera::SetState(State state)
{
	this->state = state;
	switch (state)
	{
		case State::Camera_Normal:
			leftAppend = LEFT_APPEND;
			rightAppend = RIGHT_APPEND;
			break;
		case State::Camera_InAmbush:
			break;
		case State::Camera_Recover:
		{
			const auto capCenter = cap->GetCenter();
			const auto camCenter = this->GetBBox().GetCenter();
			if (capCenter.x + LEFT_APPEND < camCenter.x) // go over to left
			{
				leftAppend = floor(camCenter.x - capCenter.x);
			}
			else if (capCenter.x - RIGHT_APPEND > camCenter.x)
			{
				rightAppend = floor(capCenter.x - camCenter.x);
			}
			else {
				SetState(State::Camera_Normal);
			}
		}			
			break;
	}
}

const RectF Camera::GetBBox() const
{
	return { pos, width, height };
}

void Camera::OnRecover()
{
	const auto capCenter = cap->GetCenter();
	const auto camCenter = this->GetBBox().GetCenter();
	if (leftAppend > LEFT_APPEND) // case move over left
	{
		if (camCenter.x - capCenter.x < leftAppend)
			leftAppend = camCenter.x - capCenter.x;
	}
	else if (rightAppend > RIGHT_APPEND) // case move over right
	{
		if (capCenter.x - camCenter.x < rightAppend)
			rightAppend = capCenter.x - camCenter.x;
	}
	else
	{
		SetState(State::Camera_Normal);
	}
}

void Camera::FollowMainCharacter()
{
	switch (state)
	{
		case State::Camera_Normal:
			CenterAround(cap->GetCenter());
			break;
		case State::Camera_InAmbush:
			cap->ClampWithin(AmbushTrigger::Instance()->GetLockCaptain());
			ClampWithin(AmbushTrigger::Instance()->GetLockCamera());
			break;
		case State::Camera_Recover:
			OnRecover();
			CenterAround(cap->GetCenter());
			break;
	}
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

