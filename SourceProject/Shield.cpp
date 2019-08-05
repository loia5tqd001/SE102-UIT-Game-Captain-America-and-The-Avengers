#include "pch.h"
#include "Shield.h"
#include "BulletEnemyGun.h"
#include "BulletDynamite.h"
#include "BulletEnemyWizard.h"
#include "BulletBunker.h"
//#include "EnemyGun.h"
//#include "EnemyRocket.h"

Shield::Shield(Captain& cap) :
	VisibleObject(State::Shield_Straight, cap.GetPos()),
	cap(cap)
{
	animations.emplace(State::Invisible, Animation(SpriteId::Invisible));
	animations.emplace(State::Shield_Up, Animation(SpriteId::Shield_Up, 0.1f));
	animations.emplace(State::Shield_Down, Animation(SpriteId::Shield_Down, 0.1f));
	animations.emplace(State::Shield_Straight, Animation(SpriteId::Shield_Straight, 0.1f));
	animations.emplace(State::Shield_Side, Animation(SpriteId::Shield_Side, 0.1f));
	UpdateByCapState(State::Captain_Standing, cap.GetPos());
	bboxColor = Colors::MyPoisonGreen;
}

void Shield::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	if (isOnCaptain)
	{
		//vel = cap.GetVelocity(); //Changed by Dung
		nx = cap.GetNx();// get nx to flip posx
	}

	switch (curState)
	{
	case State::Invisible:
		UpdateByCapState(cap.GetState(), cap.GetPos());
		break;
	case State::Destroyed:
		break;
	case State::Shield_Side:
	{
		UpdateByCapState(cap.GetState(), cap.GetPos());
		HandleSideCollison(dt, coObjects);
		break;
	}
	case State::Shield_Down:
	{
		UpdateByCapState(cap.GetState(), cap.GetPos());
		HandleBottomCollison(dt, coObjects);
		break;
	}
	case State::Shield_Straight:
	{
		UpdateByCapState(cap.GetState(), cap.GetPos(), dt);
		PrecheckAABB(dt, coObjects);
		HandleStraightCollison(dt, coObjects);
		break;
	}
	case State::Shield_Up:
	{
		if (isOnCaptain) //Don't handle collison
			UpdateByCapState(cap.GetState(), cap.GetPos()); //Don't handle collison
		else
		{

			timeToThrow += GameTimer::Dt();
			if (timeToThrow < 0.1f)
			{
				if (cap.GetState() == State::Captain_Throwing) {
					UpdateByCapState(cap.GetState(), cap.GetPos());
				}
				else
				{
					if (!isMoved) {
						isMoved = true;
						if (cap.GetNx() == 1) {
							pos.x += 20;
						}
						else
						{
							pos.x -= 20;
						}
						pos.y += 8;
					}
				}
				return;
			}
			if (!isMoved)
			{
				/*pos.x = capPos.x - 5;
				pos.y = capPos.y - 7;*/
				isMoved = true;
				if (cap.GetNx() == 1) {
					pos.x += 10;
					pos.y += 8;
				}
				else
				{
					pos.x -= 10;
					pos.y += 8;
				}
			}
			//to the max_distance
			static float flagDistance = 0;
			static bool turnBack;
			if (distance < MAX_DISTANCE)
			{
				turnBack = true;
				flagDistance += SPEED * dt;
				distance += SPEED * dt;
				vel.x = nx * SPEED;
				pos.x += nx * SPEED*dt;
			}
			else
			{
				if (turnBack)
				{ //i tried flag with (flagDistance>MAX_DISTANCE) then flagDistance-= SPEED but it cause bug: it may do the functiton 2 times
					turnBack = false;
					nx = -nx; //only do this once}
				}
				flagDistance -= SPEED * dt; //we need distance for if else checking, and flagDistance to update pos.y

				//Caculate pos.y base on pos.x by using vector and distance ratio
				Vector2 shieldCenter = this->GetBBox().GetCenter();
				Vector2 capCenter = cap.GetBBox().GetCenter();
				Vector2 trans = capCenter - shieldCenter;
				float ratio = (SPEED * dt) / std::abs(capCenter.x - shieldCenter.x);
				trans.x *= ratio;
				trans.y *= ratio;

				//Save pos.y before update pos.y base on pos.y
				float posyBackup = pos.y;

				pos.x += nx * SPEED*dt;
				vel.x = nx * SPEED;
				pos.y += trans.y;
				//Caculate vel.y with pos before and after
				vel.y = (pos.y - posyBackup) / dt;
				HandleCaptainCollison(dt, coObjects); //distance = 0
			}
		}

		HandleUpCollison(dt, coObjects);
		break;
	}
	default:
		break;
	}
}

void Shield::PrecheckAABB(float dt, const std::vector<GameObject*>& coObjects)
{
	auto shieldBox = this->GetBBox();
	for (auto &o : coObjects)
	{
		auto objBox = o->GetBBox();
		if (shieldBox.IsIntersect(objBox))
		{
			if (auto bullet = dynamic_cast<BulletEnemyGun*>(o))
			{
				//Debug::out("Shield PrecheckAABB Detected phasing object\n");
				if (bullet->GetVelocity().x*nx < 0)
				{
					if (bullet->GetPos().x > this->GetPos().x)
					{
						bullet->SetPos(Vector2{ this->GetPos().x + GetWidth() + 1,bullet->GetPos().y });
					}
					else
					{
						bullet->SetPos(Vector2{ this->GetPos().x - bullet->GetWidth() - 1 ,bullet->GetPos().y });
					}
					bullet->Reflect();
					Sounds::PlayAt(SoundId::ShieldCollide);
				}
			}
			else if (auto bullet = dynamic_cast<BulletEnemyWizard*>(o))
			{
				//Debug::out("Shield PrecheckAABB Detected phasing object\n");
				if (bullet->GetVelocity().x*nx < 0)
				{
					if (bullet->GetPos().x > this->GetPos().x)
					{
						bullet->SetPos(Vector2{ this->GetPos().x + GetWidth() + 1,bullet->GetPos().y });
					}
					else
					{
						bullet->SetPos(Vector2{ this->GetPos().x - bullet->GetWidth() - 1 ,bullet->GetPos().y });
					}
					bullet->Reflect();
					Sounds::PlayAt(SoundId::ShieldCollide);
				}
			}
		}
	}
}

void Shield::ThrowAway()
{
	if (isOnCaptain) { //cannot throw when shield is not on cap
		UpdateByCapState(State::Captain_Throwing, cap.GetPos());
		isOnCaptain = false; //go to the moving code in update
		nx = cap.GetNx();
		cap.setShieldOn(false);
	}
}

void Shield::SetVelocity(const Vector2 & vel)
{
	this->vel = vel;
}

void Shield::UpdateByCapState(State capState, Vector2 capPos, float dt)
{
	Vector2 posBackup = Vector2{ -1,-1 };
	if (dt != -1.0f)
	{
		posBackup = pos;
	}

	if (isOnCaptain)
	{
		if (capState == State::Captain_Standing)
		{
			pos.x = capPos.x + 18;
			pos.y = capPos.y + 8;
			SetState(State::Shield_Straight);
		}
		else if (capState == State::Captain_Walking)
		{
			pos.x = capPos.x + 18;
			pos.y = capPos.y + 7;
			SetState(State::Shield_Straight);
		}
		else if (capState == State::Captain_Sitting)
		{
			pos.x = capPos.x + 19;
			pos.y = capPos.y + 10;
			SetState(State::Shield_Straight);
		}
		else if (capState == State::Captain_CoverLow)
		{
			pos.x = capPos.x - 1;
			pos.y = capPos.y + 24;
			nx = -cap.GetNx();
			SetState(State::Shield_Down);
		}
		else if (capState == State::Captain_Falling)
		{
			pos.x = capPos.x + 1;
			pos.y = capPos.y + 3;
			SetState(State::Shield_Side);
		}
		else if (capState == State::Captain_Jumping)
		{
			pos.x = capPos.x + 1;
			pos.y = capPos.y + 3;
			SetState(State::Shield_Side);
		}
		else if (capState == State::Captain_Kicking)
		{
			pos.x = capPos.x - 3;
			pos.y = capPos.y + 5;
			SetState(State::Shield_Straight);
			nx = -nx;
			flipPosx(); //the only state that shield is flip already, so flip 2 times then it will back in the work
		}
		else if (capState == State::Captain_SitPunching)
		{
			pos.x = capPos.x + 7;
			pos.y = capPos.y + 7;
			SetState(State::Shield_Side);
		}
		else if (capState == State::Captain_Throwing)
		{
			pos.x = capPos.x - 12;
			pos.y = capPos.y;
			SetState(State::Shield_Up);
		}
		else if (capState == State::Captain_Tackle)
		{
			pos.x = capPos.x + 35;
			pos.y = capPos.y + 3;
			SetState(State::Shield_Straight);
		}
		else if (capState == State::Captain_Climbing)
		{
			//we dont have sprite to calculate this correctly
			pos.x = capPos.x - 2;
			pos.y = capPos.y + 23;
			SetState(State::Shield_Side);
		}
		else if (capState == State::Captain_Spinning)
		{
			SetState(State::Invisible);
		}
		else if (capState == State::Captain_CoverTop)
		{
			pos.x = capPos.x + 3;
			pos.y = capPos.y - 4;
			SetState(State::Shield_Up);
		}
		else if (capState == State::Captain_Punching)
		{
			pos.x = capPos.x - 1;
			pos.y = capPos.y + 3;
			SetState(State::Shield_Side);
		}
		else if (capState == State::Captain_Injured)
		{
			SetState(State::Invisible);
		}
		else if (capState == State::Captain_InWater)
		{
			SetState(State::Invisible);
		}
		else if (capState == State::Captain_Swimming)
		{
			SetState(State::Invisible);
		}
		else if (capState == State::Captain_FallToWater)
		{
			SetState(State::Invisible);
		}
		else if (capState == State::Captain_Dead)
		{
			SetState(State::Invisible);
		}
		if (nx < 0)
		{
			flipPosx(); //case that captain turn left
		}

		if (dt != -1.0f)
		{
			vel = pos - posBackup;
			vel.x /= dt;
			vel.y /= dt;
		}
	}
}
void Shield::flipPosx()
{
	//only call when captain turn left
	//dont worry i 've done the math
	pos.x = 2 * cap.GetPos().x - pos.x + cap.GetWidth() - this->GetWidth();
}

void Shield::HandleCaptainCollison(float dt, const std::vector<GameObject*>& coObjects)
{
	//if (nx < 0 && pos.x < cap.GetPos().x + cap.GetWidth())
	{
		if (nx < 0 && pos.x < cap.GetPos().x + cap.GetWidth() / 4)
		{
			cap.setShieldOn(true);

			UpdateByCapState(cap.GetState(), cap.GetPos());
			isOnCaptain = true;
			distance = 0;
			timeToThrow = 0;
			isMoved = false;
		}
	}
	//else if (nx > 0 && pos.x > cap.GetPos().x) 
	{
		if (nx > 0 && pos.x > cap.GetPos().x + cap.GetWidth() / 4)
		{
			cap.setShieldOn(true);


			UpdateByCapState(cap.GetState(), cap.GetPos());
			isOnCaptain = true;
			distance = 0;
			timeToThrow = 0;
			isMoved = false;
		}
	}
}

void Shield::HandleSideCollison(float dt, const std::vector<GameObject*>& coObjects)
{
}

void Shield::HandleUpCollison(float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(*this, coObjects, dt);
	if (coEvents.size() == 0) return;

	if (isOnCaptain) //deflect bullet, this is use for bulletenemyboss
	{   //TODO: change this to bulletenemyboss
		for (auto& e : coEvents)
		{
			if (auto bullet = dynamic_cast<BulletEnemyGun*>(e.pCoObj)) //
			{
				if (e.ny > 0.0f)
				{
					bullet->Reflect();
					Sounds::PlayAt(SoundId::ShieldCollide);
				}
			}
			if (auto bullet = dynamic_cast<BulletBunker*>(e.pCoObj))
			{
				if (e.ny > 0.0f)
				{
					bullet->Reflect();
					Sounds::PlayAt(SoundId::ShieldCollide);
				}
			}
		}
	}
	else //cause damage to enemy
	{
		for (auto& e : coEvents)
		{
			if (auto enemy = dynamic_cast<Enemy*>(e.pCoObj))
			{
				enemy->TakeDamage(2);
				//if (auto dynamiteNapalm = dynamic_cast<DynamiteNapalm*>(e.pCoObj))
				//{
				//	//if (dynamiteNapalm->CanTakeDamage())
				//		dynamiteNapalm->TakeDamage(1);
				//}
				//else
				//	enemy->TakeDamage(1);
			}
			else if (auto capsule = dynamic_cast<Capsule*>(e.pCoObj))
			{
				capsule->BeingHit();
			}
			else if (auto block = dynamic_cast<Block*>(e.pCoObj))
			{
				if (block->GetType() == ClassId::Switch)
				{
					SceneManager::Instance().GetCurScene().ToggleLight();
				}
			}
			else if (auto dynamite = dynamic_cast<BulletDynamite*>(e.pCoObj))
			{
				dynamite->Trigger();
			}
			// TODO: barrel oil RedAlert
		}
	}
}

void Shield::HandleStraightCollison(float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(*this, coObjects, dt);
	if (coEvents.size() == 0) return;
	if (isOnCaptain)
	{
		for (UINT i = 0; i < coEvents.size(); i++)
		{
			const CollisionEvent& e = coEvents[i];

			if (auto bullet = dynamic_cast<BulletEnemyGun*>(e.pCoObj)) //
			{
				if (nx > 0 && e.nx < 0.0f || nx <0 && e.nx > 0.0f)
				{
					if (bullet->GetPos().x > this->GetPos().x)
					{
						bullet->SetPos(Vector2{ this->GetPos().x + GetWidth() + 1,bullet->GetPos().y });
					}
					else
					{
						bullet->SetPos(Vector2{ this->GetPos().x - bullet->GetWidth() - 1 ,bullet->GetPos().y });
					}
					bullet->Reflect();
					Sounds::PlayAt(SoundId::ShieldCollide);
				}
			}
			if (auto bullet = dynamic_cast<BulletEnemyWizard*>(e.pCoObj))
			{
				if (e.nx < 0.0f && this->nx > 0 || e.nx > 0.0f && this->nx < 0)
				{
					if (bullet->GetPos().x > this->GetPos().x)
					{
						bullet->SetPos(Vector2{ this->GetPos().x + GetWidth() + 1,bullet->GetPos().y });
					}
					else
					{
						bullet->SetPos(Vector2{ this->GetPos().x - bullet->GetWidth() - 1 ,bullet->GetPos().y });
					}
					bullet->Reflect();
					Sounds::PlayAt(SoundId::ShieldCollide);
				}
			}
			if (auto bullet = dynamic_cast<BulletBunker*>(e.pCoObj))
			{
				if (e.nx < 0.0f && this->nx > 0 || e.nx > 0.0f && this->nx < 0)
				{
					if (bullet->GetPos().x > this->GetPos().x)
					{
						bullet->SetPos(Vector2{ this->GetPos().x + GetWidth() + 1,bullet->GetPos().y });
					}
					else
					{
						bullet->SetPos(Vector2{ this->GetPos().x - bullet->GetWidth() - 1 ,bullet->GetPos().y });
					}
					bullet->Reflect();
					Sounds::PlayAt(SoundId::ShieldCollide);
				}
			}
		}
	}
}

void Shield::HandleBottomCollison(float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(*this, coObjects, dt);
	if (coEvents.size() == 0) return;
	if (isOnCaptain)
	{
		for (UINT i = 0; i < coEvents.size(); i++)
		{
			const CollisionEvent& e = coEvents[i];

			//if (auto enemy = dynamic_cast<Enemy*>(e.pCoObj)) //
			//{
			//	if (e.ny > 0.0f)
			//	{
			//		enemy->TakeDamage(3);
			//	}
			//}
			if (auto block = dynamic_cast<Block*>(e.pCoObj))
			{
				if (block->GetType() == ClassId::PassableLedge ||
					block->GetType() == ClassId::RigidBlock)
				{
					Sounds::PlayAt(SoundId::ShieldCollide);
				}
			}
			else if (auto bullet = dynamic_cast<BulletBunker*>(e.pCoObj))
			{
				if (e.ny < 0.0f)
				{
					bullet->Reflect();
					Sounds::PlayAt(SoundId::ShieldCollide);
				}
			}
		}
	}
}

RectF Shield::GetBBox() const
{
	if (curState != State::Shield_Straight) return VisibleObject::GetBBox();

	if (nx < 0)
		return VisibleObject::GetBBox().Trim((float)GetWidth() / 2 - 0.05f, 0, 0, 0);
	else
		return VisibleObject::GetBBox().Trim(0, 0, (float)GetWidth() / 2 - 0.05f, 0);

	//return VisibleObject::GetBBox().Trim((float)GetWidth() / 2 - 0.05f, 0, (float)GetWidth() / 2 - 0.05f, 0);
}
