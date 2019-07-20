#include "pch.h"
#include "Captain.h"
#include "Shield.h"
#include "Block.h"
#include "Spawner.h"
#include "AmbushTrigger.h"
#include "Item.h"
#include "Enemy.h"
#include "Capsule.h"

static auto& setting = Settings::Instance();
static auto& wnd = Window::Instance();

Captain::Captain(const Vector2 & spawnPos) :
	VisibleObject(State::Captain_Standing, spawnPos),
	shieldOn(true),
	prevPressedControlKey(KeyControls::Attack),
	canSpin(false),
	posWhenJump(spawnPos)
{
	animations.emplace(State::Captain_Standing, Animation(SpriteId::Captain_Standing));
	animations.emplace(State::Captain_Moving, Animation(SpriteId::Captain_Walking, 0.1f));
	animations.emplace(State::Captain_Jump, Animation(SpriteId::Captain_Jump, 0.2f));
	animations.emplace(State::Captain_Falling, Animation(SpriteId::Captain_Jump));
	animations.emplace(State::Captain_LookUp, Animation(SpriteId::Captain_LookUp, 0.1f));
	animations.emplace(State::Captain_Sitting, Animation(SpriteId::Captain_Sitting, 0.1f));
	animations.emplace(State::Captain_Punching, Animation(SpriteId::Captain_Punching, 0.2f));
	animations.emplace(State::Captain_Throw, Animation(SpriteId::Captain_Throw, 0.2f));
	animations.emplace(State::Captain_JumpKick, Animation(SpriteId::Captain_JumpKick, 0.2f));
	animations.emplace(State::Captain_SitPunch, Animation(SpriteId::Captain_SitPunch, 0.15f));
	animations.emplace(State::Captain_Smash, Animation(SpriteId::Captain_Smash, 0.25f));
	animations.at(State::Captain_Smash).SetCusFrameHoldTime(0, 0.05f);
	animations.emplace(State::Captain_Spin, Animation(SpriteId::Captain_Spin, 0.3f));

	shield = std::make_unique<Shield>(this);

	pos = { 73.0f, 391.0f };
	bboxColor = Colors::MyPoisonGreen;
}

void Captain::OnKeyDown(BYTE keyCode)
{
	switch (keyCode)
	{
	case VK_TAB:
		OnFlashing(false);
		break;
	case VK_LEFT:
		if (prevPressedControlKey != KeyControls::Left && !IsInTheAir())
		{
			prevPressedControlKey = KeyControls::Left;
			timePressed = std::chrono::steady_clock::now();
			OutputDebugString("Last key is not Left\n");
		}
		else
		{
			std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
			std::chrono::duration<float> timepassed = now - timePressed;

			if (timepassed.count() > DOUBLE_KEY_DOWN_TIME_OUT)
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
		if (prevPressedControlKey != KeyControls::Right && !IsInTheAir())
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

		if (IsInTheAir() == false) {
			SetState(State::Captain_Jump);
			posWhenJump = pos;
		}


	if(keyCode == setting.Get(KeyControls::Attack)) 
	{ 
		if (IsInTheAir() == false)
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

void Captain::OnKeyUp(BYTE keyCode)
{
	if (keyCode == 0x4D)
	{
		if (curState == State::Captain_Jump)
			SetState(State::Captain_Falling);
	}
}

void Captain::ProcessInput()
{
	static const Window& wnd = Window::Instance();

	if (wnd.IsKeyPressed(setting.Get(KeyControls::Left)))
	{
		if (curState == State::Captain_Smash)
		{
			return;
		}
		nx = -std::abs(nx);
		vel.x = nx * WALKING_SPEED;

		if (!IsInTheAir()) //on the ground
		{
			SetState(State::Captain_Moving);
		}
		return;
	}

	if (wnd.IsKeyPressed(setting.Get(KeyControls::Right)))
	{
		if (curState == State::Captain_Smash)
		{
			return;
		}

		nx = std::abs(nx);
		vel.x = nx * WALKING_SPEED;
		if (!IsInTheAir())
		{
			SetState(State::Captain_Moving);
		}
		return;
	}

	if (wnd.IsKeyPressed(setting.Get(KeyControls::Up)))
	{
		if (!IsInTheAir())
		{
			SetState(State::Captain_LookUp);
		}
		return;
	}

	if (wnd.IsKeyPressed(setting.Get(KeyControls::Down)))
	{
		if (!IsInTheAir() && !(curState == State::Captain_SitPunch))
		{
			SetState(State::Captain_Sitting);
		}
		return;
	}

}

void Captain::CollideWithPassableObjects(float dt, const CollisionEvent& e)
{
	if (e.nx != 0.0f) 
	{
		pos.x += min( e.pCoObj->GetBBox().GetWidth(), (1.0f - e.t) * vel.x * dt );
	}
	if (e.ny != 0.0f)
	{
		pos.y += min(e.pCoObj->GetBBox().GetHeight(), (1.0f - e.t) * vel.y * dt);
	}
}

void Captain::HandleNoCollisions(float dt)
{
	pos.x += vel.x * dt;
	pos.y += vel.y * dt;
}

void Captain::HandleCollisions(float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(*this, coObjects, dt);
	if (coEvents.size() == 0) { HandleNoCollisions(dt); return; }

	float min_tx, min_ty, nx, ny;
	CollisionDetector::FilterCollisionEvents(coEvents, min_tx, min_ty, nx, ny);

	if (coEvents.size() == 0) return;

	pos.x += min_tx * vel.x * dt;
	pos.y += min_ty * vel.y * dt;

	for (auto& e : coEvents)
	{
		if (auto spawner = dynamic_cast<Spawner*>(e.pCoObj))
		{
			spawner->OnCollideWithCap();
			CollideWithPassableObjects(dt, e);
		}
		else if (auto ambush = dynamic_cast<AmbushTrigger*>(e.pCoObj))
		{
			if (!ambush->IsActive())
				ambush->Active(coObjects);
			CollideWithPassableObjects(dt, e);
		}
		else if (auto item = dynamic_cast<Item*>(e.pCoObj))
		{
			// TODO: being hit or collected, need to rewrite
			item->BeingCollected();
			CollideWithPassableObjects(dt, e);
		}
		else if (auto enemy = dynamic_cast<Enemy*>(e.pCoObj))
		{
			if (isFlashing) { // undamagable
				CollideWithPassableObjects(dt, e);
			}
			else {
				// TODO: setState being damaged
			}
		}
		else if (auto block = dynamic_cast<Block*>(e.pCoObj)) {

			switch (block->GetType())
			{
				case ClassId::Water:
					// TODO: setstate on water;
					// NOTE: onwater will have on water animation and vel.y -= vel of water, pos.y = (calculate)
					break;

				case ClassId::Switch:
					// TODO: check if hit switch then toggle light map
					CollideWithPassableObjects(dt, e);
					break;

				case ClassId::NextMap:
					// check if krystal is collected, if yes then move to next map
					CollideWithPassableObjects(dt, e);
					break;

				case ClassId::Door:
					if (wnd.IsKeyPressed(setting.Get(KeyControls::Up))) {
						// TODO: handle scene in pittsburgh
					}
					CollideWithPassableObjects(dt, e);
					break;

				case ClassId::PassableLedge:
					if (e.ny < 0.0f && !wnd.IsKeyPressed(setting.Get(KeyControls::Down))) {
						break;
					}
					else {
						CollideWithPassableObjects(dt, e);
						break;
					}

				case ClassId::ClimbableBar:
					// TODO: handle in pittsburgh map
					break;

				case ClassId::DamageBlock:
					// TODO: set state being damage
				case ClassId::RigidBlock:
					break;

				default:
					AssertUnreachable();
			}
		}
		else if (dynamic_cast<Capsule*>(e.pCoObj)) {
			CollideWithPassableObjects(dt, e);
		}
	}
}

void Captain::SetState(State state)
{
	prevState = curState;

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
	case State::Captain_Falling:
		vel.y = JUMPING_SPEED;
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
		if (IsInTheAir())
		{
			return;
		}
		vel.x = SMASH_SPEED * nx;
		vel.y = 0.0f;
		break;
	case State::Captain_Spin:
		vel.y = -JUMPING_SPEED;
		break;
	default:
		break;
	}
	VisibleObject::SetState(state);
}

void Captain::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	//early checking
	if (curState == State::Destroyed)
		return;

	// if (vel.y < 0)
	// {
	// 	if (pos.y > posWhenJump.y - MAX_HEIGHT_JUMP)
	// 	{
	// 		SetState(State::Captain_Jump);
	// 	}
	// 	else
	// 		if ((pos.y <= posWhenJump.y - MAX_HEIGHT_JUMP) &&
	// 			(pos.y > posWhenJump.y - MAX_HEIGHT_JUMP - MAX_HEIGHT_SPIN))
	// 		{
	// 			SetState(State::Captain_Spin);
	// 		}
	// 		else
	// 		{
	// 			vel.y = -vel.y;
	// 		}
	// }
	// else
	// {
	// 	if (vel.y < 0)
	// 	{
	// 		if (pos.y > posWhenJump.y - MAX_HEIGHT_JUMP)
	// 		{
	// 			SetState(State::Captain_Falling);
	// 		}
	// 		else
	// 			if ((pos.y <= posWhenJump.y - MAX_HEIGHT_JUMP) &&
	// 				(pos.y > posWhenJump.y - MAX_HEIGHT_JUMP - MAX_HEIGHT_SPIN))
	// 			{
	// 				SetState(State::Captain_Spin);
	// 			}
	// 			else
	// 			{
	// 				pos.y = posWhenJump.y - MAX_HEIGHT_JUMP - MAX_HEIGHT_SPIN + 1;
	// 			}
	// 	}
	// }


	/*if (vel.y < 0)
	{
		if (pos.y > posWhenJump.y - MAX_HEIGHT_JUMP)
		{
			SetState(State::Captain_Jump);
		}
		else
		{
			if (pos.y<posWhenJump.y - MAX_HEIGHT_JUMP - MAX_HEIGHT_SPIN)
			{
				SetState(State::Captain_Falling);
			}
			else
			{
				SetState(State::Captain_Spin);
			}
		}
	}
	else if (vel.y > 0)
	{
		SetState(State::Captain_Falling);
	}*/

	//Todo: fix
	//if (canSpin)
	//	OutputDebugString("Can spin???\n");

	vel.y += GRAVITY * dt;

	switch (curState)
	{
	case State::Captain_JumpKick:
		if (animations.at(curState).IsDoneCycle())
			SetState(State::Captain_Falling);
		break;

	case State::Captain_Throw:
		if (animations.at(curState).IsDoneCycle())
			SetState(State::Captain_Standing);
		break;
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
	case State::Captain_Spin:
		if (animations.at(curState).IsDoneCycle())
		{
			SetState(State::Captain_Falling);
		}
	default:
		break;
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

inline bool Captain::IsInTheAir()
{
	switch (curState)
	{
	case State::Invisible: //Todo: solve this
		return false;
		break;
	case State::Destroyed:
		return false;
		break;
	case State::Captain_Standing:
	case State::Captain_Moving:
	case State::Captain_LookUp:
	case State::Captain_Sitting:
	case State::Captain_Punching:
	case State::Captain_Throw:
	case State::Captain_SitPunch:
	case State::Captain_Smash:
		return false;
		break;
	case State::Captain_Jump:
	case State::Captain_Falling:
	case State::Captain_OnTwoKnee:
	case State::Captain_JumpKick:
	case State::Captain_Climb: //TODO: remeber this
	case State::Captain_Spin:
		return true;
		break;
	default:
		Debug::out("State is not handled! Message from [bool Captain::IsInTheAir()]");
		break;
	}
}
