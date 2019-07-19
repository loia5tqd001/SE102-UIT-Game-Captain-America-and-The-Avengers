#include "pch.h"
#include "Captain.h"
#include"Shield.h"

static auto& setting = Settings::Instance();

Captain::Captain(const Vector2 & spawnPos) :
	VisibleObject(State::Captain_Standing, spawnPos),
	isInTheAir(false),
	shieldOn(true),
	prevPressedControlKey(KeyControls::Attack)
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
	animations.emplace(State::Captain_SitPunch, Animation(SpriteId::Captain_SitPunch, 0.15f));
	animations.emplace(State::Captain_Smash, Animation(SpriteId::Captain_Smash, 0.25f));

	animations.at(State::Captain_Smash).SetCusFrameHoldTime(0, 0.05f);

	shield = std::make_unique<Shield>(this);
	//bboxColor = Colors::MyPoisonGreen;
}

void Captain::OnKeyDown(BYTE keyCode)
{
	switch (keyCode)
	{
	case VK_TAB:
		OnFlashing(false);
		break;
	case VK_LEFT:
		if (prevPressedControlKey!=KeyControls::Left && !isInTheAir)
		{
			prevPressedControlKey = KeyControls::Left;
			timePressed = std::chrono::steady_clock::now();
			OutputDebugString("Last key is not Left\n");
		}
		else
		{
			std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
			std::chrono::duration<float> timepassed = now - timePressed;

			if (timepassed.count()>DOUBLE_KEY_DOWN_TIME_OUT)
			{
				prevPressedControlKey = KeyControls::Left;
				timePressed = now;
				OutputDebugString("KeyControl timeout!\n");
			}
			else
			{
				OutputDebugString("Event hit\n");
				//Todo:Set state
				SetState(State::Captain_Smash);
			}
		}
		break;
	case VK_RIGHT:
		if (prevPressedControlKey != KeyControls::Right && !isInTheAir)
		{
			prevPressedControlKey = KeyControls::Right;
			timePressed = std::chrono::steady_clock::now();
			OutputDebugString("Last key is not Right\n");
		}
		else
		{
			std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
			std::chrono::duration<float> timepassed = now - timePressed;

			if (timepassed.count() > DOUBLE_KEY_DOWN_TIME_OUT)
			{
				prevPressedControlKey = KeyControls::Right;
				timePressed = now;
				OutputDebugString("KeyControl timeout!\n");
			}
			else
			{
				OutputDebugString("Event hit\n");
				//Todo:Set state
				SetState(State::Captain_Smash);
			}
		}
		break;
	}

	if (keyCode == 0x4D) { //x
		if (isInTheAir == false) {
			SetState(State::Captain_Jump);
			isInTheAir = true;
		}
	}


	if (keyCode == 0x4E) { //z
		if (isInTheAir == false)
			if (curState == State::Captain_Sitting)
				SetState(State::Captain_SitPunch);
			else
			{
				if (shieldOn)
				{
					SetState(State::Captain_Throw);
					shieldOn = false;
					shield->ThrowAway();
				}
				else
					SetState(State::Captain_Punching);
			}
		else
			SetState(State::Captain_JumpKick);
	}
}

void Captain::ProcessInput()
{
	static const Window& wnd = Window::Instance();

	if (wnd.IsKeyPressed(setting.Get(KeyControls::Left)))
	{
		if (curState ==State::Captain_Smash)
		{
			return;
		}
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
		if (curState ==State::Captain_Smash)
		{
			return;
		}
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
		if (!isInTheAir && !(curState == State::Captain_SitPunch))
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
		break;
	case State::Captain_LookUp:
		vel.x = 0.0f;
		vel.y = 0.0f;
		break;
	case State::Captain_Sitting:
		vel.x = 0.0f;
		vel.y = 0.0f;
		break;
	case State::Captain_JumpKick:
		vel.x = 0.0f;
		break;
	case State::Captain_Smash:
		vel.x = SMASH_SPEED * nx;
		vel.y = 0.0f;
		break;
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
	case State::Captain_Smash:
		if (animations.at(curState).IsDoneCycle())
			SetState(State::Captain_Standing);
		break;
	case State::Captain_SitPunch:
		if (animations.at(curState).IsDoneCycle())
			SetState(State::Captain_Sitting);
		break;
	case State::Captain_Moving:
		SetState(State::Captain_Standing);
		break;
	default:
		break;
	}

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
	std::vector<GameObject*> co;
	shield->Update(dt, co);

}

void Captain::Render() const
{
	VisibleObject::Render();
	shield->Render();
}

