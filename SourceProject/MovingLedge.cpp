#include "pch.h"
#include "MovingLedge.h"

MovingLedge::MovingLedge(Vector2 pos, Behaviors behavior) :
	VisibleObject(State::MovingLedge_Stall, pos),
	behavior(behavior)
{
	animations.emplace(State::MovingLedge_Stop, Animation(SpriteId::MovingLedge, 0.15f));
	animations.emplace(State::MovingLedge_Moving, Animation(SpriteId::MovingLedge, 0.05f));
	animations.emplace(State::MovingLedge_Stall, Animation(SpriteId::MovingLedge));

	if (behavior == Behaviors::MovingLedge_Horizontal) {
		firetails.emplace_back(SpriteId::FireTail_Hor, 0.15f);
		firetails.emplace_back(SpriteId::Invisible);
	} else {
		firetails.emplace_back(SpriteId::FireTail_Hor, 0.1f);
		firetails.emplace_back(SpriteId::FireTail_Ver, 0.1f);
	}
}

void MovingLedge::OnCircleMoving(float dt)
{
	if (curState == State::MovingLedge_Stall)
	{
		SetState(State::MovingLedge_Moving);
	}
	else if (curState == State::MovingLedge_Moving)
	{
		static constexpr auto PI = 3.14159265f;
		static constexpr auto omega = 1.5f;
		static auto holdTime = 0.0f;
		static auto lastPos = pos;

		holdTime += dt;
		pos.x = 580.0f + 52.0f * cos( omega * holdTime);
		pos.y = 690.0f + 35.0f * cos( omega * holdTime - PI / 2.0f);

		// calculate for handling collision:
		vel.x = (pos.x - lastPos.x) / dt;
		vel.y = (pos.y - lastPos.y) / dt;
		lastPos = pos;
	}
}

void MovingLedge::OnDiagonalMoving(float dt)
{
	static const auto TOP_MOST = Vector2{ 680.0f, 495.0f }; // top most position
	static const auto BOTTOM_MOST = Vector2{ 630.0f, 615.0f };
	static const auto ORIGINAL_SPEED = Vector2{ -25.0f, 60.0f } * 1.64f;

	if (curState == State::MovingLedge_Stall)
	{
		vel = ORIGINAL_SPEED;
		SetState(State::MovingLedge_Stop);
	}
	else if (curState == State::MovingLedge_Moving)
	{
		pos.x += vel.x * dt;
		pos.y += vel.y * dt;
		if (pos.y <= TOP_MOST.y)
		{
			pos = TOP_MOST;
			SetState(State::MovingLedge_Stop);
		}
		else if (pos.y >= BOTTOM_MOST.y)
		{
			pos = BOTTOM_MOST;
			SetState(State::MovingLedge_Stop);
		}
	}
	else if (curState == State::MovingLedge_Stop)
	{
		static auto timeStopped = 0.0f;
		timeStopped += dt;
		if (timeStopped >= 0.4f)
		{
			timeStopped = 0.0f;
			SetState(State::MovingLedge_Moving);
			vel = -vel;
		}
	}
}

void MovingLedge::OnHorizontalMoving(float dt)
{
	static constexpr auto LEFT_MOST = 616.0f;
	static constexpr auto RIGHT_MOST = 714.0f;
	static const auto ORIGINAL_SPEED = Vector2{ -80.0f, 0.0f };

	if (curState == State::MovingLedge_Stall)
	{
		vel = ORIGINAL_SPEED;
		SetState(State::MovingLedge_Stop);
	}
	else if (curState == State::MovingLedge_Moving)
	{
		pos.x += vel.x * dt;
		if (pos.x <= LEFT_MOST)
		{
			pos.x = LEFT_MOST;
			SetState(State::MovingLedge_Stop);
		}
		else if (pos.x >= RIGHT_MOST)
		{
			pos.x = RIGHT_MOST;
			SetState(State::MovingLedge_Stop);
		}
	}
	else if (curState == State::MovingLedge_Stop)
	{
		static auto timeStopped = 0.0f;
		timeStopped += dt;
		if (timeStopped >= 0.4f)
		{
			timeStopped = 0.0f;
			SetState(State::MovingLedge_Moving);
			vel = -vel;
		}
	}
}

void MovingLedge::UpdateByUpdater(float dt)
{
	animations.at(curState).Update(dt);
	firetails.at(FireTail::Hor).Update(dt);
	firetails.at(FireTail::Ver).Update(dt);

	if (behavior == Behaviors::MovingLedge_Circle)
	{
		OnCircleMoving(dt);
	}
	else if (behavior == Behaviors::MovingLedge_Diagonal)
	{
		OnDiagonalMoving(dt);
	}
	else if (behavior == Behaviors::MovingLedge_Horizontal)
	{
		OnHorizontalMoving(dt);
	}
}

Vector2 MovingLedge::GetVelocity() const
{
	if (curState == State::MovingLedge_Moving) return vel;
	else return {};
}

RectF MovingLedge::GetBBox() const
{
	return VisibleObject::GetBBox().Trim(0.0f, 5.0f, 0.0f, 0.0f);
}

void MovingLedge::Render() const
{
	VisibleObject::Render();

	// render firetail:
	if (!DebugDraw::IsInDeepDebug()) 
	{
		const auto getVel = GetVelocity();
		const auto movingLedgeDrawPos = Camera::Instance().GetPositionInViewPort( pos );

		// draw horizontal firetail:
		if (getVel.x < 0)
		{
			const auto drawPos = movingLedgeDrawPos + Vector2{ 31.0f, 4.0f };
			firetails.at(FireTail::Hor).Render(drawPos, { -1.0f, 1.0f });
		}
		else if (getVel.x > 0)
		{
			const auto drawPos = movingLedgeDrawPos + Vector2{ -7.0f, 4.0f };
			firetails.at(FireTail::Hor).Render(drawPos, { 1.0f, 1.0f });
		}

		// draw vertical firetail:
		if (getVel.y != 0)
		{
			const auto drawPos1 = movingLedgeDrawPos + Vector2{ 5.0f, 16.0f };
			const auto drawPos2 = movingLedgeDrawPos + Vector2{ 21.0f, 16.0f };
			firetails.at(FireTail::Ver).Render(drawPos1);
			firetails.at(FireTail::Ver).Render(drawPos2);
		}
	}
}
