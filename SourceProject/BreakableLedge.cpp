#include "pch.h"
#include "BreakableLedge.h"

BreakableLedge::BreakableLedge(Vector2 pos) :
	VisibleObject(State::BreakableLedge_Intact, pos)
{
	animations.emplace(State::BreakableLedge_Intact, Animation(SpriteId::BreakableLedge_Intact));
	animations.emplace(State::BreakableLedge_Separated, Animation(SpriteId::BreakableLedge_Separated));
	timeFlashing = 4.0f;
}

void BreakableLedge::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	if (!isSeparated) return;

	if (curState != State::BreakableLedge_Separated)
	{
		std::chrono::duration<float> duration = std::chrono::steady_clock::now() - seperatedTime;
		if (duration.count() > 0.16f)
		{
			SetState(State::BreakableLedge_Separated);
		}
	}
	else 
	{
		if (!VisibleObject::GetBBox().IsIntersect(Camera::Instance().GetBBox()))
		{
			SetState((State::BreakableLedge_Intact));
			OnFlashing(false);
			isSeparated = false;
			return;
		}
	}

	OnFlashing();
}

RectF BreakableLedge::GetBBox() const
{
	if (curState == State::BreakableLedge_Intact) 
		return VisibleObject::GetBBox().Trim(0.0f, 4.0f, 0.0f, 0.0f);
	else return {};
}

void BreakableLedge::OnCollideWithCap()
{
	if (isSeparated) return;

	isSeparated = true;
	seperatedTime = std::chrono::steady_clock::now();
	OnFlashing(true);
}