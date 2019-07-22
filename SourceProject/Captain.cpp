#include "pch.h"
#include "Captain.h"

static auto& setting = Settings::Instance();

Captain::Captain(const Vector2& pos) :
	VisibleObject(State::Captain_Standing, pos),
	currentState(&stateStanding)

{
	animations.emplace(State::Captain_Standing, Animation(SpriteId::Captain_Standing));
	animations.emplace(State::Captain_Walking, Animation(SpriteId::Captain_Walking, 0.1f));
	animations.emplace(State::Captain_Jumping, Animation(SpriteId::Captain_Jump, 0.3f));
	animations.emplace(State::Captain_CoverTop, Animation(SpriteId::Captain_LookUp, 0.2f));
	animations.emplace(State::Captain_Sitting, Animation(SpriteId::Captain_Sitting, 0.11f));
	animations.emplace(State::Captain_Punching, Animation(SpriteId::Captain_Punching, 0.1f));
	animations.emplace(State::Captain_Throwing, Animation(SpriteId::Captain_Throw, 0.15f));
	animations.emplace(State::Captain_Kicking, Animation(SpriteId::Captain_JumpKick, 0.9f));
	animations.emplace(State::Captain_SitPunching, Animation(SpriteId::Captain_SitPunch, 0.12f));
	animations.emplace(State::Captain_Tackle, Animation(SpriteId::Captain_Smash, 0.35f));	
	animations.emplace(State::Captain_Climbing, Animation(SpriteId::Captain_Climb, 0.2f));
	animations.emplace(State::Captain_Injured, Animation(SpriteId::Captain_Injure, 0.4f));
	animations.emplace(State::Captain_Dead, Animation(SpriteId::Captain_Dead));
	animations.emplace(State::Captain_Swimming, Animation(SpriteId::Captain_Swimming, 0.1f));
	animations.emplace(State::Captain_FallToWater, Animation(SpriteId::Captain_FallToWater,0.2f));
	animations.emplace(State::Captain_InWater, Animation(SpriteId::Captain_InWater, 0.1f));
	animations.emplace(State::Captain_Spinning, Animation(SpriteId::Captain_Spin, 0.01f));

	animations.at(State::Captain_Tackle).SetCusFrameHoldTime(0, 0.1f);

	shield = std::make_unique<Shield>(*this);

	bboxColor = Colors::MyPoisonGreen;

}

void Captain::OnKeyDown(BYTE keyCode)
{
	currentState->OnKeyDown(*this, keyCode);
	if (setting.IsKeyControl(keyCode))
	{
		lastKeyDown = setting.GetKControl(keyCode);
		timeLastKeyDown = std::chrono::steady_clock::now();
	}
	//switch (keyCode)
	//{
	//case VK_TAB:
	//	OnFlashing(false);
	//	break;
	//case VK_LEFT:
	//	if (prevPressedControlKey!=KeyControls::Left && !isInTheAir)
	//	{
	//		prevPressedControlKey = KeyControls::Left;
	//		timePressed = std::chrono::steady_clock::now();
	//		OutputDebugString("Last key is not Left\n");
	//	}
	//	else
	//	{
	//		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	//		std::chrono::duration<float> timepassed = now - timePressed;

	//		if (timepassed.count()>DOUBLE_KEY_DOWN_TIME_OUT)
	//		{
	//			prevPressedControlKey = KeyControls::Left;
	//			timePressed = now;
	//			OutputDebugString("KeyControl timeout!\n");
	//		}
	//		else
	//		{
	//			OutputDebugString("Event hit\n");
	//			//Todo:Set state
	//			SetState(State::Captain_Smash);
	//		}
	//	}
	//	break;
	//case VK_RIGHT:
	//	if (prevPressedControlKey != KeyControls::Right && !isInTheAir)
	//	{
	//		prevPressedControlKey = KeyControls::Right;
	//		timePressed = std::chrono::steady_clock::now();
	//		OutputDebugString("Last key is not Right\n");
	//	}
	//	else
	//	{
	//		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	//		std::chrono::duration<float> timepassed = now - timePressed;

	//		if (timepassed.count() > DOUBLE_KEY_DOWN_TIME_OUT)
	//		{
	//			prevPressedControlKey = KeyControls::Right;
	//			timePressed = now;
	//			OutputDebugString("KeyControl timeout!\n");
	//		}
	//		else
	//		{
	//			OutputDebugString("Event hit\n");
	//			//Todo:Set state
	//			SetState(State::Captain_Smash);
	//		}
	//	}
	//	break;
	//}

	//if (keyCode == setting.Get(KeyControls::Jump)) { 
	//	if (isInTheAir == false) {
	//		SetState(State::Captain_Jump);
	//		isInTheAir = true;
	//	}
	//}


	//if (keyCode == setting.Get(KeyControls::Attack)) { 
	//	if (isInTheAir == false)
	//		if (curState == State::Captain_Sitting)
	//			SetState(State::Captain_SitPunch);
	//		else
	//		{
	//			if (shieldOn)
	//			{
	//				SetState(State::Captain_Throw);
	//				shieldOn = false;
	//				shield->ThrowAway();
	//			}
	//			else
	//				SetState(State::Captain_Punching);
	//		}
	//	else
	//		SetState(State::Captain_JumpKick);
	//}
}

void Captain::OnKeyUp(BYTE keyCode)
{
	currentState->OnKeyUp(*this, keyCode);
	if (setting.IsKeyControl(keyCode))
	{
		lastKeyUp = setting.GetKControl(keyCode);
		timeLastKeyUp = std::chrono::steady_clock::now();
	}
}

//void Captain::ProcessInput()
//{
//	static const Window& wnd = Window::Instance();
//
//	if (wnd.IsKeyPressed(setting.Get(KeyControls::Left)))
//	{
//		if (curState ==State::Captain_Smash)
//		{
//			return;
//		}
//		nx = -std::abs(nx);
//		vel.x = nx * WALKING_SPEED;
//		if (!isInTheAir)
//		{
//			SetState(State::Captain_Moving);
//		}
//		return;
//	}
//
//	if (wnd.IsKeyPressed(setting.Get(KeyControls::Right)))
//	{
//		if (curState ==State::Captain_Smash)
//		{
//			return;
//		}
//		nx = std::abs(nx);
//		vel.x = nx * WALKING_SPEED;
//		if (!isInTheAir)
//		{
//			SetState(State::Captain_Moving);
//		}
//		return;
//	}
//
//	if (wnd.IsKeyPressed(setting.Get(KeyControls::Up)))
//	{
//		if (!isInTheAir)
//		{
//			SetState(State::Captain_LookUp);
//		}
//		return;
//	}
//
//	if (wnd.IsKeyPressed(setting.Get(KeyControls::Down)))
//	{
//		if (!isInTheAir && !(curState == State::Captain_SitPunch))
//		{
//			SetState(State::Captain_Sitting);
//		}
//		return;
//	}
//	
//}

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


//void Captain::HandleCollisions(float dt, const std::vector<GameObject*>& coObjects)
//{
//	auto coEvents = CollisionDetector::CalcPotentialCollisions(*this, coObjects, dt);
//	if (coEvents.size() == 0) { HandleNoCollisions(dt); return; }
//
//	float min_tx, min_ty, nx, ny;
//	CollisionDetector::FilterCollisionEvents(coEvents, min_tx, min_ty, nx, ny);
//
//	if (coEvents.size() == 0) return;
//
//	pos.x += min_tx * vel.x * dt;
//	pos.y += min_ty * vel.y * dt;
//
//	for (auto& e : coEvents)
//	{
//		if (auto spawner = dynamic_cast<Spawner*>(e.pCoObj))
//		{
//			spawner->OnCollideWithCap();
//			CollideWithPassableObjects(dt, e);
//		}
//		else if (auto ambush = dynamic_cast<AmbushTrigger*>(e.pCoObj))
//		{
//			if (!ambush->IsActive())
//				ambush->Active(coObjects);
//			CollideWithPassableObjects(dt, e);
//		}
//		else if (auto item = dynamic_cast<Item*>(e.pCoObj))
//		{
//			// TODO: being hit or collected, need to rewrite
//			item->BeingCollected();
//			CollideWithPassableObjects(dt, e);
//		}
//		else if (auto enemy = dynamic_cast<Enemy*>(e.pCoObj))
//		{
//			if (isFlashing) { // undamagable
//				CollideWithPassableObjects(dt, e);
//			}
//			else {
//				// TODO: setState being damaged
//			}
//		}
//		else if (auto block = dynamic_cast<Block*>(e.pCoObj)) {
//
//			switch (block->GetType())
//			{
//				case ClassId::Water:
//					// TODO: setstate on water;
//					// NOTE: onwater will have on water animation and vel.y -= vel of water, pos.y = (calculate)
//					break;
//
//				case ClassId::Switch:
//					// TODO: check if hit switch then toggle light map
//					CollideWithPassableObjects(dt, e);
//					break;
//
//				case ClassId::NextMap:
//					// check if krystal is collected, if yes then move to next map
//					CollideWithPassableObjects(dt, e);
//					break;
//
//				case ClassId::Door:
//					if (wnd.IsKeyPressed(setting.Get(KeyControls::Up))) {
//						// TODO: handle scene in pittsburgh
//					}
//					CollideWithPassableObjects(dt, e);
//					break;
//
//				case ClassId::PassableLedge:
//					if (e.ny < 0.0f && !wnd.IsKeyPressed(setting.Get(KeyControls::Down))) {
//						break;
//					}
//					else {
//						CollideWithPassableObjects(dt, e);
//						break;
//					}
//
//				case ClassId::ClimbableBar:
//					// TODO: handle in pittsburgh map
//					break;
//
//				case ClassId::DamageBlock:
//					// TODO: set state being damage
//				case ClassId::RigidBlock:
//					break;
//
//				default:
//					AssertUnreachable();
//			}
//		}
//		else if (dynamic_cast<Capsule*>(e.pCoObj)) {
//			CollideWithPassableObjects(dt, e);
//		}
//	}
//}

void Captain::SetState(State state)
{
	auto exitData = currentState->Exit(*this, state);
	switch (state)
	{
		case State::Captain_Standing    : currentState = &stateStanding    ; break;
		case State::Captain_Walking     : currentState = &stateWalking     ; break;
		case State::Captain_Jumping     : currentState = &stateJumping     ; break;
		case State::Captain_Kicking     : currentState = &stateKicking     ; break;
		case State::Captain_Spinning    : currentState = &stateSpinning    ; break;
		case State::Captain_Throwing    : currentState = &stateThrowing    ; break;
		case State::Captain_Tackle      : currentState = &stateTackle      ; break;
		case State::Captain_Punching    : currentState = &statePunching    ; break;
		case State::Captain_Sitting     : currentState = &stateSitting     ; break;
		case State::Captain_SitPunching : currentState = &stateSitPunching ; break;
		case State::Captain_CoverTop    : currentState = &stateCoverTop    ; break;
		case State::Captain_CoverLow    : currentState = &stateCoverLow    ; break;
		case State::Captain_FallToWater : currentState = &stateFallToWater ; break;
		case State::Captain_InWater     : currentState = &stateInWater     ; break;
		case State::Captain_Swimming    : currentState = &stateSwimming    ; break;
		case State::Captain_Climbing    : currentState = &stateClimbing    ; break;
		case State::Captain_Injured     : currentState = &stateInjured     ; break;
		case State::Captain_Dead        : currentState = &stateDead        ; break;
	}

	const auto oldState = curState;
	VisibleObject::SetState(state); // NOTE: when changing state height will always anchor to low point (sitting)
	currentState->Enter(*this, oldState, std::move(exitData));

	//switch (state)
	//{
	//case State::Captain_Standing:
	//	vel.x = 0.0f;
	//	vel.y = 0.0f;
	//	break;
	//case State::Captain_Moving:
	//	vel.x = nx * WALKING_SPEED;
	//	vel.y = 0.0f;
	//	break;
	//case State::Captain_Jump:
	//	vel.y = -JUMPING_SPEED;
	//	break;
	//case State::Captain_Punching:
	//	vel.x = 0;
	//	vel.y = 0;
	//	break;
	//case State::Captain_LookUp:
	//	vel.x = 0.0f;
	//	vel.y = 0.0f;
	//	break;
	//case State::Captain_Sitting:
	//	vel.x = 0.0f;
	//	vel.y = 0.0f;
	//	break;
	//case State::Captain_JumpKick:
	//	vel.x = 0.0f;
	//	break;
	//case State::Captain_Smash:
	//	vel.x = SMASH_SPEED * nx;
	//	vel.y = 0.0f;
	//	break;
	//default:
	//	break;
	//}
}

void Captain::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	//early checking
	//if (curState == State::Destroyed)
	//	return;

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

	//vel.y += GRAVITY * dt;

	//switch (curState)
	//{
	//case State::Captain_JumpKick:
	//	if (animations.at(curState).IsDoneCycle())
	//		SetState(State::Captain_Falling);
	//	break;
//
	//case State::Captain_Throw:
	//	if (animations.at(curState).IsDoneCycle())
	//		SetState(State::Captain_Standing);
	//	break;
	//case State::Captain_Punching:
	//case State::Captain_Sitting:
	//case State::Captain_LookUp:
	//case State::Captain_Smash:
	//	if (animations.at(curState).IsDoneCycle())
	//		SetState(State::Captain_Standing);
	//	break;
	//case State::Captain_SitPunch:
	//	if (animations.at(curState).IsDoneCycle())
	//		SetState(State::Captain_Sitting);
	//	break;
	//case State::Captain_Moving:
	//	SetState(State::Captain_Standing);
	//	break;
	//case State::Captain_Spin:
	//	if (animations.at(curState).IsDoneCycle())
	//	{
	//		SetState(State::Captain_Falling);
	//	}
	//default:
	//	break;
	//}

	currentState->Update(*this, dt, coObjects);

	animations.at(curState).Update(dt);

	// recalculate if image should be rendered
	OnFlashing();

	shield->Update(dt, coObjects);

	//switch (curState)
	//{
	//case State::Captain_JumpKick:
	//	if (animations.at(curState).IsDoneCycle())
	//		SetState(State::Captain_Falling);
	//	break;

	//case State::Captain_Throw:
	//	if (animations.at(curState).IsDoneCycle())
	//		SetState(State::Captain_Standing);
	//	break;
	//case State::Captain_Punching:
	//case State::Captain_Sitting:
	//case State::Captain_LookUp:
	//case State::Captain_Smash:
	//	if (animations.at(curState).IsDoneCycle())
	//		SetState(State::Captain_Standing);
	//	break;
	//case State::Captain_SitPunch:
	//	if (animations.at(curState).IsDoneCycle())
	//		SetState(State::Captain_Sitting);
	//	break;
	//case State::Captain_Moving:
	//	SetState(State::Captain_Standing);
	//	break;
	//default:
	//	break;
	//}


	//// process input
	//ProcessInput();

	//// handle collisions
	//HandleCollisions(dt, coObjects);


}

Vector2 Captain::GetCenter() const
{
	if (curState == State::Captain_SitPunching) {
		if (nx > 0) return GetBBox().GetCenter() - Vector2{ 7.0f, 0.0f };
		else        return GetBBox().GetCenter() + Vector2{ 7.0f, 0.0f };
	} else {
		return GetBBox().GetCenter();
	}
}

void Captain::Render() const
{
	VisibleObject::Render();
	DebugDraw::DrawSolidRect( GetHitBox(), Colors::MyChineseBrown ); 
	shield->Render();
}

RectF Captain::GetBBox() const
{
	return VisibleObject::GetBBox().Trim( (float)GetWidth() / 2 - 3, 0, (float)GetWidth() / 2 - 3, 0);
}

RectF Captain::GetHitBox() const
{
	switch (curState)
	{
		case State::Captain_Kicking:
			if (nx > 0) return VisibleObject::GetBBox().Trim(28, 10, 0, 8);
			else        return VisibleObject::GetBBox().Trim(0, 10, 28, 8);

		case State::Captain_Punching:
			if (nx > 0) return VisibleObject::GetBBox().Trim(28, 6, 0, 29);
			else        return VisibleObject::GetBBox().Trim(0, 6, 28, 29);

		case State::Captain_SitPunching:
			if (nx > 0) return VisibleObject::GetBBox().Trim(27, 5, 0, 16);
			else        return VisibleObject::GetBBox().Trim(0, 5, 27, 16);

		case State::Captain_Tackle:
			if (nx > 0) return VisibleObject::GetBBox().Trim(31, 5, 0, 11);
			else        return VisibleObject::GetBBox().Trim(0, 5, 31, 11);

		default:
			return {};
	}
}

//inline bool Captain::IsInTheAir()
//{
//	switch (curState)
//	{
//	case State::Invisible: //Todo: solve this
//		return false;
//		break;
//	case State::Destroyed:
//		return false;
//		break;
//	case State::Captain_Standing:
//	case State::Captain_Moving:
//	case State::Captain_LookUp:
//	case State::Captain_Sitting:
//	case State::Captain_Punching:
//	case State::Captain_Throw:
//	case State::Captain_SitPunch:
//	case State::Captain_Smash:
//		return false;
//		break;
//	case State::Captain_Jump:
//	case State::Captain_Falling:
//	case State::Captain_OnTwoKnee:
//	case State::Captain_JumpKick:
//	case State::Captain_Climb: //TODO: remeber this
//	case State::Captain_Spin:
//		return true;
//		break;
//	default:
//		Debug::out("State is not handled! Message from [bool Captain::IsInTheAir()]");
//		break;
//	}
//}
