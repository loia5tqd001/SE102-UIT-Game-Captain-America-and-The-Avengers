#include "pch.h"
#include "Captain.h"
#include"DynamiteNapalm.h"
static auto& setting = Settings::Instance();
static auto& wnd = Window::Instance();

Captain::Captain(const Vector2& pos, Grid* ogrid)
	: VisibleObject(State::Captain_Standing, pos),
	currentState(&stateStanding),
	canPhaseThroughFloor(false),
	phasingState(State::NotExist),
	prePhasingState(State::NotExist),
	grid(ogrid)
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
	animations.emplace(State::Captain_Dead, Animation(SpriteId::Captain_Dead, 2.5f));
	animations.emplace(State::Captain_Swimming, Animation(SpriteId::Captain_Swimming, 0.1f));
	animations.emplace(State::Captain_FallToWater, Animation(SpriteId::Captain_FallToWater, 0.07f));
	animations.emplace(State::Captain_InWater, Animation(SpriteId::Captain_InWater, 0.2f));
	animations.emplace(State::Captain_Spinning, Animation(SpriteId::Captain_Spin, 0.13f));
	animations.emplace(State::Captain_CoverLow, Animation(SpriteId::Captain_CoverLow, 0.1f));
	animations.emplace(State::CaptainElectricShock, Animation(SpriteId::Captain_ElectricShock, 0.1f));

	animations.at(State::Captain_Tackle).SetCusFrameHoldTime(0, 0.05f);

	shield = std::make_unique<Shield>(*this);

	bboxColor = Colors::MyPoisonGreen;
	timeFlashing = 2.5f;
	//nFrameToRender = 2; // 2 - 2 is already default
	nFrameToUnrender = 1;
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
	auto verticalRect = VisibleObject::GetBBox().Trim(GetWidth() / 2.0f - 3, 0, GetWidth() / 2.0f - 3, 0);
	switch (curState)
	{
	case State::Captain_Jumping:
	case State::Captain_Falling:
	case State::Captain_Spinning:
		//return verticalRect.Trim(0, 0, 0, GetHeight() / 2.0f);
	default:
		return verticalRect;
	}
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
	prePhasingState = curState;

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
	case State::CaptainElectricShock:currentState = &stateElectricShock;break;
	}

	const auto oldState = curState;
	VisibleObject::SetState(state); // NOTE: when changing state height will always anchor to low point (sitting)
	currentState->Enter(*this, oldState, std::move(exitData));
	shield->UpdateByCapState(this->curState, this->pos);


#if 1
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
#endif
}

void Captain::PrecheckAABB(const std::vector<GameObject*>& coObjects, float dt)
{
	const auto capBbox = GetBBox();
	Vector2 absoluteLimitPoint = pos;

	for (auto& obj : coObjects)
		if (capBbox.IsIntersect(obj->GetBBox()))
		{
			if (auto enemy = dynamic_cast<Enemy*>(obj))
			{
				if (isFlashing)
					return;
				enemy->TakeDamage(1);
				this->health.Subtract(1);
				if (curState == State::CaptainElectricShock)
					return;
				else if (auto dynamite = dynamic_cast<DynamiteNapalm*>(obj))
				{
					if (dynamite->CanCauseElectricShock())
					{
						SetState(State::CaptainElectricShock);
						continue;
					}
				}
				SetState(State::Captain_Injured);
			}
			else if (auto bullet = dynamic_cast<Bullet*>(obj))
			{
				if (isFlashing) return;
				bullet->HitCaptain();
				this->health.Subtract(bullet->GetDamage());
				SetState(State::Captain_Injured);
			}
			else if (auto item = dynamic_cast<Item*>(obj))
			{
				item->BeingCollected();
			}
			else if (auto spawner = dynamic_cast<Spawner*>(obj))
			{
				spawner->OnCollideWithCap(this);
			}
			else if (auto block = dynamic_cast<Block*>(obj))
			{
				if (block->GetType() == ClassId::DamageBlock)
				{
					if (isFlashing) return;
					health.Subtract(1);
					SetState(State::Captain_Injured);
				}
				else if (block->GetType() == ClassId::ClimbableBar)
				{
					if (curState != State::Captain_Falling) continue;
					else
					{
						SetState(State::Captain_Climbing);
						pos.y = block->GetPos().y;
						if (nx == -1 && pos.x < block->GetPos().x)
						{
							pos.x = block->GetPos().x;
						}
						else if (nx == 1 && pos.x + GetWidth() > block->GetPos().x + block->GetBBox().GetWidth())
						{
							pos.x = block->GetPos().x + block->GetBBox().GetWidth() - GetWidth();
						}
						shield->UpdateByCapState(curState, pos);
					}
				}
				else if (block->GetType() == ClassId::Door)
				{
					if (curState == State::Captain_Standing && wnd.IsKeyPressed(setting.Get(KeyControls::Up)))
					{
						SceneManager::Instance().GetCurScene().Teleport();
					}
				}
				else if (block->GetType() == ClassId::RigidBlock)
				{
					pos = posBeforePhasing - vel * dt;



					// Don't care about these stuff below

					//Vector2 deltaS = pos - posBeforePhasing;

					////Phuong trinh duong thang
					////DeltaS dong thoi la vector chi phuong
					////deltaS.y(x - pos.x) - deltaS.x(y - pos.x) = 0

					//Vector2 limitPoint = { -1,-1 };

					//if (deltaS.x != 0)
					//{
					//	if (vel.x < 0)
					//	{
					//		limitPoint.x = block->GetPos().x + block->GetWidth() + 1;
					//		//Right Edge
					//		limitPoint.y = deltaS.y*(limitPoint.x - pos.x) / deltaS.x + pos.x;

					//		if (limitPoint.x > absoluteLimitPoint.x)
					//		{
					//			absoluteLimitPoint = limitPoint;
					//		}
					//	}
					//	else
					//	{
					//		//Left Edge
					//		limitPoint.x = block->GetPos().x - 1.0f*GetWidth() - 1;
					//		limitPoint.y = deltaS.y*(limitPoint.x - pos.x) / deltaS.x + pos.x;
					//		if (limitPoint.x < absoluteLimitPoint.x)
					//		{
					//			absoluteLimitPoint = limitPoint;
					//		}
					//	}
					//}
					//else
					//{
					//	//Todo: Handle this 
					//}
					//pos = absoluteLimitPoint;
				}
			}
		}
	posBeforePhasing = pos;
	
}

void Captain::CollideWithPassableObjects(float dt, const CollisionEvent& e)
{
	if (e.nx != 0.0f)
	{
		//int a = e.pCoObj->GetBBox().GetWidth() * (-e.nx);
		//int b = (1.0f - e.t) * vel.x * dt;
		pos.x += (0.9f - e.t) * vel.x * dt;
	}
	if (e.ny != 0.0f)
	{
		pos.y += (0.9f - e.t) * vel.y * dt;
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
			// NOTE: I think we should handle keydown for switch
			else if (auto block = dynamic_cast<Block*>(obj))
			{
				if (block->GetType() == ClassId::Switch)
				{
					SceneManager::Instance().GetCurScene().ToggleLight();
				}
			}
			// else if oil barrel in Red Alert ...
		}

}

void Captain::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	animations.at(curState).Update(dt);

	PrecheckAABB(coObjects, dt);
	if (!ignoreUpdate) currentState->Update(*this, dt, coObjects);
	ignoreUpdate = false;
	HandleHitBox(dt, coObjects);

	OnFlashing();
	shield->UpdateByCapState(curState, pos);
	shield->Update(dt, coObjects);
}
