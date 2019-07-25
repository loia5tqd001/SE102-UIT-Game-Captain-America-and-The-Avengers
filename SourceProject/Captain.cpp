#include "pch.h"
#include "Captain.h"

static auto& setting = Settings::Instance();

Captain::Captain(const Vector2& pos) :
	VisibleObject(State::Captain_Standing, pos),
	currentState(&stateStanding)
{
	animations.emplace(State::Captain_Standing, Animation(SpriteId::Captain_Standing));
	animations.emplace(State::Captain_Walking, Animation(SpriteId::Captain_Walking, 0.15f));
	animations.emplace(State::Captain_Jumping, Animation(SpriteId::Captain_Jump, 0.3f));
	animations.emplace(State::Captain_Falling, Animation(SpriteId::Captain_Falling, 0.3f));
	animations.emplace(State::Captain_CoverTop, Animation(SpriteId::Captain_LookUp, 0.2f));
	animations.emplace(State::Captain_Sitting, Animation(SpriteId::Captain_Sitting, 0.04f));
	animations.emplace(State::Captain_Punching, Animation(SpriteId::Captain_Punching, 0.1f));
	animations.emplace(State::Captain_Throwing, Animation(SpriteId::Captain_Throw, 0.15f));
	animations.emplace(State::Captain_Kicking, Animation(SpriteId::Captain_JumpKick, 0.15f));
	animations.emplace(State::Captain_SitPunching, Animation(SpriteId::Captain_SitPunch, 0.12f));
	animations.emplace(State::Captain_Tackle, Animation(SpriteId::Captain_Smash, 0.35f));
	animations.emplace(State::Captain_Climbing, Animation(SpriteId::Captain_Climb, 0.2f));
	animations.emplace(State::Captain_Injured, Animation(SpriteId::Captain_Injure, 0.3f));
	animations.emplace(State::Captain_Dead, Animation(SpriteId::Captain_Dead));
	animations.emplace(State::Captain_Swimming, Animation(SpriteId::Captain_Swimming, 0.1f));
	animations.emplace(State::Captain_FallToWater, Animation(SpriteId::Captain_FallToWater, 0.07f));
	animations.emplace(State::Captain_InWater, Animation(SpriteId::Captain_InWater, 0.2f));
	animations.emplace(State::Captain_Spinning, Animation(SpriteId::Captain_Spin, 0.15f));
	animations.emplace(State::Captain_CoverLow, Animation(SpriteId::Captain_CoverLow, 0.1f));

	animations.at(State::Captain_Tackle).SetCusFrameHoldTime(0, 0.05f);

	shield = std::make_unique<Shield>(*this);

	bboxColor = Colors::MyPoisonGreen;
	timeFlashing = 1.8f;
}

Vector2 Captain::GetCenter() const
{
	if (curState == State::Captain_SitPunching) {
		if (nx > 0) return GetBBox().GetCenter() - Vector2{ 7.0f, 0.0f };
		else        return GetBBox().GetCenter() + Vector2{ 7.0f, 0.0f };
	}
	else {
		return GetBBox().GetCenter();
	}
}

void Captain::Render() const
{
	VisibleObject::Render();
	DebugDraw::DrawSolidRect(GetHitBox(), Colors::MyChineseBrown);
	shield->Render();
}

RectF Captain::GetBBox() const
{
	return VisibleObject::GetBBox().Trim((float)GetWidth() / 2 - 3, 0, (float)GetWidth() / 2 - 3, 0);
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

		default:
			return {};
	}
}

void Captain::OnKeyDown(BYTE keyCode)
{
	currentState->OnKeyDown(*this, keyCode);
	if (setting.IsKeyControl(keyCode))
	{
		lastKeyDown = setting.GetKControl(keyCode);
		timeLastKeyDown = std::chrono::steady_clock::now();
	}
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

void Captain::SetState(State state)
{
	auto exitData = currentState->Exit(*this, state);
	switch (state)
	{
	case State::Captain_Standing: currentState = &stateStanding; break;
	case State::Captain_Walking: currentState = &stateWalking; break;
	case State::Captain_Jumping: currentState = &stateJumping; break;
	case State::Captain_Falling: currentState = &stateFalling; break;
	case State::Captain_Kicking: currentState = &stateKicking; break;
	case State::Captain_Spinning: currentState = &stateSpinning; break;
	case State::Captain_Throwing: currentState = &stateThrowing; break;
	case State::Captain_Tackle: currentState = &stateTackle; break;
	case State::Captain_Punching: currentState = &statePunching; break;
	case State::Captain_Sitting: currentState = &stateSitting; break;
	case State::Captain_SitPunching: currentState = &stateSitPunching; break;
	case State::Captain_CoverTop: currentState = &stateCoverTop; break;
	case State::Captain_CoverLow: currentState = &stateCoverLow; break;
	case State::Captain_FallToWater: currentState = &stateFallToWater; break;
	case State::Captain_InWater: currentState = &stateInWater; break;
	case State::Captain_Swimming: currentState = &stateSwimming; break;
	case State::Captain_Climbing: currentState = &stateClimbing; break;
	case State::Captain_Injured: currentState = &stateInjured; break;
	case State::Captain_Dead: currentState = &stateDead; break;
	}

	const auto oldState = curState;
	VisibleObject::SetState(state); // NOTE: when changing state height will always anchor to low point (sitting)
	currentState->Enter(*this, oldState, std::move(exitData));
	shield->UpdateByCapState(this->curState, this->pos);
	//For debuging
	if (true)
	{
		switch (state)
		{
		case State::Invisible:  
			Debug::out("Cap.SetState(State::Invisible)\n");
			break;
		case State::Destroyed:
			Debug::out("Cap.SetState(State::Destroyed)\n");
			break;
		case State::Captain_Standing:
			Debug::out("Cap.SetState(State::Captain_Standing)\n");
			break;
		case State::Captain_Walking:
			Debug::out("Cap.SetState(State::Captain_Walking)\n");
			break;
		case State::Captain_Jumping:
			Debug::out("Cap.SetState(State::Captain_Jumping)\n");
			break;
		case State::Captain_Falling:
			Debug::out("Cap.SetState(State::Captain_Falling)\n");
			break;
		case State::Captain_Kicking:
			Debug::out("Cap.SetState(State::Captain_Kicking)\n");
			break;
		case State::Captain_Spinning:
			Debug::out("Cap.SetState(State::Captain_Spinning)\n");
			break;
		case State::Captain_Throwing:
			Debug::out("Cap.SetState(State::Captain_Throwing)\n");
			break;
		case State::Captain_Tackle:
			Debug::out("Cap.SetState(State::Captain_Tackle)\n");
			break;
		case State::Captain_Punching:
			Debug::out("Cap.SetState(State::Captain_Punching)\n");
			break;
		case State::Captain_Sitting:
			Debug::out("Cap.SetState(State::Captain_Sitting)\n");
			break;
		case State::Captain_SitPunching:
			Debug::out("Cap.SetState(State::Captain_SitPunching)\n");
			break;
		case State::Captain_CoverTop:
			Debug::out("Cap.SetState(State::Captain_CoverTop)\n");
			break;
		case State::Captain_CoverLow:
			Debug::out("Cap.SetState(State::Captain_CoverLow)\n");
			break;
		case State::Captain_FallToWater:
			Debug::out("Cap.SetState(State::Captain_FallToWater)\n");
			break;
		case State::Captain_InWater:
			Debug::out("Cap.SetState(State::Captain_InWater)\n");
			break;
		case State::Captain_Swimming:
			Debug::out("Cap.SetState(State::Captain_Swimming)\n");
			break;
		case State::Captain_Climbing:
			Debug::out("Cap.SetState(State::Captain_Climbing)\n");
			break;
		case State::Captain_Injured:
			Debug::out("Cap.SetState(State::Captain_Injured)\n");
			break;
		case State::Captain_Dead:
			Debug::out("Cap.SetState(State::Captain_Dead)\n");
			break;
		default:
			break;
		}

	}
}

void Captain::PrecheckAABB(float dt, const std::vector<GameObject*>& coObjects)
{
	if (isFlashing) return;
	const auto capBbox = GetBBox();

	for (auto& obj : coObjects)
	if (capBbox.IsIntersect(obj->GetBBox())) 
	{
		if (auto enemy = dynamic_cast<Enemy*>(obj))
		{
			enemy->TakeDamage(1);
			this->health.Subtract(1);
			SetState(State::Captain_Injured);
		}
		else if (auto bullet = dynamic_cast<Bullet*>(obj))
		{
			this->health.Subtract(bullet->GetDamage());
			SetState(State::Captain_Injured);
		}
		else if (auto item = dynamic_cast<Item*>(obj))
		{
			item->BeingCollected();
		}
	}
}

void Captain::CollideWithPassableObjects(float dt, const CollisionEvent& e)
{
	if (e.nx != 0.0f)
	{
		pos.x += min(e.pCoObj->GetBBox().GetWidth(), (1.0f - e.t) * vel.x * dt);
	}
	if (e.ny != 0.0f)
	{
		pos.y += min(e.pCoObj->GetBBox().GetHeight(), (1.0f - e.t) * vel.y * dt);
	}
}

void Captain::HandleHitBox(float dt, const std::vector<GameObject*>& coObjects)
{
	if (GetHitBox().IsNone()) return;

	for (auto& obj : coObjects)
	if (GetHitBox().IsIntersect(obj->GetBBox()))
	{
		if (auto capsule = dynamic_cast<Capsule*>(obj))
		{
			capsule->BeingHit();
		}
		else if (auto enemy = dynamic_cast<Enemy*>(obj))
		{
			enemy->TakeDamage(1);
		}
		else if (auto block = dynamic_cast<Block*>(obj))
		{
			if (block->GetType() == ClassId::Switch)
			{
				SceneManager::Instance().GetCurScene().isDark = 
					!SceneManager::Instance().GetCurScene().isDark;
			}
		}
		// else if oil barrel in Red Alert ...
	}
	
}

void Captain::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	animations.at(curState).Update(dt);

	PrecheckAABB(dt, coObjects);	
	currentState->Update(*this, dt, coObjects);
	HandleHitBox(dt, coObjects);
	

	OnFlashing();
	shield->Update(dt, coObjects);
}
