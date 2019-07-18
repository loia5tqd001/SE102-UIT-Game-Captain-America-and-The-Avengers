#include "pch.h"
#include "Captain.h"

static auto& setting = Settings::Instance();

Captain::Captain(const Vector2 & spawnPos) :
	VisibleObject(State::Captain_Standing, spawnPos),
	isInTheAir(false),
	doubleKeyDownTimeOut(0.2f)
	//Todo: Remove this when test is done
	,shieldOn(true)
{
	animations.emplace(State::Captain_Standing, Animation(SpriteId::Captain_Standing));
	animations.emplace(State::Captain_Moving, Animation(SpriteId::Captain_Walking, 0.1f));
	animations.emplace(State::Captain_Jump, Animation(SpriteId::Captain_Jump));
	animations.emplace(State::Captain_Falling, Animation(SpriteId::Captain_Jump));
	animations.emplace(State::Captain_LookUp, Animation(SpriteId::Captain_LookUp, 0.1f));
	animations.emplace(State::Captain_Sitting, Animation(SpriteId::Captain_Sitting, 0.1f));
	animations.emplace(State::Captain_Punching, Animation(SpriteId::Captain_Punching, 0.1f));
	animations.emplace(State::Captain_Throw, Animation(SpriteId::Captain_Throw, 0.2f));
	animations.emplace(State::Captain_JumpKick, Animation(SpriteId::Captain_JumpKick, 0.2f));

	//bboxColor = Colors::MyPoisonGreen;
}

void Captain::OnKeyDown(BYTE keyCode)
{
	switch (keyCode)
	{
	case VK_TAB:
		OnFlashing(false);
		break;
	}

	if (keyCode == setting.Get(KeyControls::Jump)) {
		if (isInTheAir == false) {
			SetState(State::Captain_Jump);
			isInTheAir = true;
		}
	}

	if (keyCode == setting.Get(KeyControls::Attack)) {
		if (isInTheAir == false)
			if (shieldOn)
				SetState(State::Captain_Throw);
			else
				SetState(State::Captain_Punching);
		else
			SetState(State::Captain_JumpKick);
	}
}

void Captain::ProcessInput()
{
	static const Window& wnd = Window::Instance();

	if (wnd.IsKeyPressed(setting.Get(KeyControls::Left)))
	{
		nx = -std::abs(nx);
		vel.x = nx * WALKING_SPEED;
		if (!isInTheAir)
		{
			SetState(State::Captain_Moving);
		}
		return;
	}

	if (wnd.IsKeyPressed(setting.Get(KeyControls::Right)))
	{
		nx = std::abs(nx);
		vel.x = nx * WALKING_SPEED;
		if (!isInTheAir)
		{
			SetState(State::Captain_Moving);
		}
		return;
	}

	if (wnd.IsKeyPressed(setting.Get(KeyControls::Up)))
	{
		if (!isInTheAir)
		{
			SetState(State::Captain_LookUp);
		}
		return;
	}

	if (wnd.IsKeyPressed(setting.Get(KeyControls::Down)))
	{
		if (!isInTheAir)
		{
			SetState(State::Captain_Sitting);
		}
		return;
	}
}

void Captain::HandleNoCollisions(float dt)
{
	//Todo: Remove when test is done
	if (isStandingOnTheGround() && isInTheAir)
	{
		pos.y = 200;
		vel.y = 0;
		isInTheAir = false;
		SetState(State::Captain_Standing);
	}
	pos.x += vel.x * dt;
	pos.y += vel.y * dt;

	//Todo: Delete this when test is done
}

void Captain::HandleCollisions(float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(*this, coObjects, dt);
	if (coEvents.size() == 0) { HandleNoCollisions(dt); return; }

	float min_tx, min_ty, nx, ny;
	CollisionDetector::FilterCollisionEvents(coEvents, min_tx, min_ty, nx, ny);

	// NOTE: HACK: not perfect handler but we're fine
	if (coEvents.size() == 0) return; // the case object's going toward the corner

	//pos.x += min_tx * vel.x * dt;
	//pos.y += min_ty * vel.y * dt;

	//if (nx != 0.0f) vel.x = 0.0f;
	//if (ny != 0.0f) { vel.y = 0.0f; _isJumping = false; }

	// Collision logic with Goombas
	/*for (UINT i = 0; i < coEvents.size(); i++)
	{
		const CollisionEvent& e = coEvents[i];

		if (auto goomba = dynamic_cast<Goomba*>(e.pCoObj))
		{
			if (e.ny < 0.0f && goomba->GetState() != State::GoombaDie)
			{
				goomba->SetState(State::GoombaDie);
				vel.y = -JUMP_DEFLECT_SPEED;
				OnFlashing(true);
			}
		}
	}*/
}

void Captain::SetState(State state)
{
	prevState = curState;
	VisibleObject::SetState(state);

	assert(animations.count(state) == 1);

	switch (state)
	{
	case State::Captain_Standing:
		vel.x = 0.0f;
		vel.y = 0.0f;
		break;
	case State::Captain_Moving:
		vel.x = nx * WALKING_SPEED;
		vel.y = 0.0f;
		break;
	case State::Captain_Jump:
		vel.y = -JUMPING_SPEED;
		break;
	case State::Captain_Punching:
		vel.x = 0;
		vel.y = 0;
	case State::Captain_LookUp:
		vel.x = 0.0f;
		vel.y = 0.0f;
		break;
	case State::Captain_Sitting:
		vel.x = 0.0f;
		vel.y = 0.0f;
	case State::Captain_JumpKick:
		vel.x = 0.0f;
	default:
		break;
	}
}

void Captain::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	//early checking
	if (curState == State::Destroyed)
		return;
	switch (curState)
	{
	case State::Captain_JumpKick:
		if (animations.at(curState).IsDoneCycle())
			SetState(State::Captain_Falling);
		break;
	case State::Captain_Throw:
	case State::Captain_Punching:
	case State::Captain_Sitting:
	case State::Captain_LookUp:
		if (animations.at(curState).IsDoneCycle())
			SetState(State::Captain_Standing);
		break;
	case State::Captain_Moving:
		SetState(State::Captain_Standing);
		break;
	default:
		break;
	}
#pragma region _EDIT_
	//if (curState==State::Captain_JumpKick)
	//{
	//	if (animations.at(curState).IsDoneCycle())
	//	{
	//		SetState(State::Captain_Falling);
	//	}
	//}

	//if (curState==State::Captain_Throw)
	//{
	//	if (animations.at(curState).IsDoneCycle())
	//	{
	//		SetState(State::Captain_Standing);
	//	}
	//}

	//if (curState==State::Captain_Punching)
	//{
	//	if (animations.at(curState).IsDoneCycle())
	//	{
	//		SetState(State::Captain_Standing);
	//	}
	//}
#pragma endregion
	//Gravity to make Cap fall
	if (isInTheAir)
	{
		vel.y += GRAVITY * dt;
	}

	// process input
	ProcessInput();

	// handle collisions
	HandleCollisions(dt, coObjects);

	// update animations
	animations.at(curState).Update(dt);

	//// recalculate if image should be rendered
	//OnFlashing();
}

