#include "pch.h"
#include "EnemyWizard.h"
#include "BulletFireEnemyWizard.h"
#include "BulletEnemyWizard.h"

EnemyWizard::EnemyWizard(Vector2 spawnPos, Vector2 vel, int nx, Grid * grid, Captain& cap) :
	Enemy(behavior, behaviorData,State::EnemyWizard_Stand, 20, spawnPos, grid),
	cap(cap)
{
	animations.emplace(State::EnemyWizard_BeforeDefeated, Animation(SpriteId::EnemyWizard_BeforeDefeated, 0.2f));
	animations.emplace(State::EnemyWizard_Defeated, Animation(SpriteId::EnemyWizard_Defeated, 50.0f));
	animations.emplace(State::EnemyWizard_Laught, Animation(SpriteId::EnemyWizard_Laught, 0.3f));
	animations.emplace(State::EnemyWizard_FlyDown, Animation(SpriteId::EnemyWizard_FlyDown, 0.6f));
	animations.emplace(State::EnemyWizard_FlyUp, Animation(SpriteId::EnemyWizard_FlyUp, 0.6f));
	animations.emplace(State::EnemyWizard_Flying, Animation(SpriteId::EnemyWizard_FlyDown, 0.6f));
	animations.emplace(State::EnemyWizard_ShootBullet, Animation(SpriteId::EnemyWizard_ShootBullet, 0.07f));
	animations.emplace(State::EnemyWizard_ShootBulletFire, Animation(SpriteId::EnemyWizard_ShootBulletFire, 0.07f));
	animations.emplace(State::EnemyWizard_ShootWhenFly, Animation(SpriteId::EnemyWizard_ShootWhenFly, 0.1f));
	animations.emplace(State::EnemyWizard_Stand, Animation(SpriteId::EnemyWizard_Stand, 0.3f));
	animations.emplace(State::EnemyWizard_Walking, Animation(SpriteId::EnemyWizard_Walking, 0.3f));

	beforeExplode = State::EnemyWizard_BeforeDefeated;
	Explode = State::EnemyWizard_Defeated; //this enemy do not explode
}

void EnemyWizard::SpawnBullet()
{
	if (isFlashing) return;
	if (this->curState == State::EnemyWizard_ShootBullet) {
		Vector2 bulletPos = pos + Vector2{ 34.0f, 11.0f };
		Vector2 bulletVel;
		bulletVel.x = nx * BulletEnemyWizard::GetXSpeed();
		bulletVel.y = - std::abs(bulletPos.y - (cap.GetPos().y + 10.0f)) / std::abs(bulletPos.x - cap.GetPos().x) * abs(bulletVel.x);

		if (abs(bulletVel.y) > abs(bulletVel.x)) bulletVel.y = -abs(bulletVel.x);
		if (pos.y < cap.GetPos().y) bulletVel.y = 0;
		grid->SpawnObject(std::make_unique<BulletEnemyWizard>(nx, bulletPos, bulletVel, this));
		Sounds::PlayAt(SoundId::BulletNormal);
	}
}

void EnemyWizard::SpawnBulletFire()
{
	if (isFlashing) return;
	if (this->curState == State::EnemyWizard_ShootWhenFly) {
		const auto bulletPos = pos + Vector2{ 23.0f, 31.0f };
		grid->SpawnObject(std::make_unique<BulletFireEnemyWizard>(nx, bulletPos, true, this));
		Sounds::PlayAt(SoundId::BulletLazer);
	}
	else if (this->curState == State::EnemyWizard_ShootBulletFire) {
		const auto bulletPos = pos + Vector2{ 34.0f, 11.0f };
		grid->SpawnObject(std::make_unique<BulletFireEnemyWizard>(nx, bulletPos, false, this));
		Sounds::PlayAt(SoundId::BulletLazer);
	}
}

void EnemyWizard::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	/*if (curState == State::EnemyWizard_Defeated) return;*/
	//regular update
	pos.x += vel.x*dt;
	pos.y += vel.y*dt;

	//running action list
	Action();

	//update animations
	UpdateAnimation(dt);
	//
	HandleCollisions(dt, coObjects);
}

void EnemyWizard::SetState(State state)
{
	VisibleObject::SetState(state);

	switch (state)
	{
	case State::EnemyWizard_BeforeDefeated:
		vel.x = -nx * FALL_BACK; 
		vel.y = 0;
		break;
	case State::EnemyWizard_Defeated:
		vel.x = 0; 
		vel.x = 0;
		break;
	case State::EnemyWizard_FlyUp:
		vel.y = -FLYING_SPEED; 
		vel.x = 0;
		break;
	case State::EnemyWizard_FlyDown:
		vel.y = FLYING_SPEED;
		vel.x = 0;
		break;
	case State::EnemyWizard_Flying:
		vel.x = FLYING_SPEED * nx;
		vel.y = 0;
		break;
	case State::EnemyWizard_ShootBullet:
		vel.x = 0;
		vel.y = 0;
		SpawnBullet();
		break;
	case State::EnemyWizard_ShootBulletFire:
		vel.x = 0;
		vel.y = 0;
		SpawnBulletFire();
		break;
	case State::EnemyWizard_ShootWhenFly:
		SpawnBulletFire();
		break;
	case State::EnemyWizard_Stand:
		vel.x = 0;
		vel.y = 0;
		break;
	case State::EnemyWizard_Walking:
		vel.x = WALKING_SPEED*nx;
		vel.y = 0;
		break;
	case State::EnemyWizard_Laught:
		vel.x = 0;
		vel.y = 0;
		break;
	}
}

bool EnemyWizard::Onbehaviors(Behaviors behavior) //return true when current behavior is done
{
	if (curState == State::EnemyWizard_BeforeDefeated)
	{
		if (!animations.at(State::EnemyWizard_BeforeDefeated).IsDoneCycle()) {
			if (pos.y <= GROUND + 8) {
				vel.y = FALLING_SPEED;
			}
			else {
				pos.y = GROUND + 8;
			}
			return false;
		}
		else {
			SetState(State::EnemyWizard_Defeated);
		}
	}
	if (curState == State::EnemyWizard_Defeated) {
		if (!animations.at(State::EnemyWizard_Defeated).IsDoneCycle()) {
			if (pos.y <= GROUND + 25) {
				vel.y = FALLING_SPEED;
			}
			else {
				pos.y = GROUND + 25;
			}
			return false;
		}
		else {
			SetState(State::Destroyed);
		}
	}
	assert(pos.x >= MIN_POS_X - 50 && pos.x <= MAX_POS_X + 50); //warning!!!
	assert(pos.y >= ROOF - 50 && pos.y <= GROUND + 50); //warning!!!
	static bool checkShotOnce = false;
	static int counterFly = 0;
	if (behavior == Behaviors::EnemyWizard_FlyingShoot)
	{
		if (pos.x <= MAX_POS_X && pos.x >= MIN_POS_X)
		{
			if (curState == State::EnemyWizard_ShootWhenFly && !animations.at(State::EnemyWizard_ShootWhenFly).IsDoneCycle()) {
				return false;
			}
			else {
				SetState(State::EnemyWizard_Flying);
			}
			if (nx <0 && cap.GetPos().x < pos.x && cap.GetPos().x + cap.GetWidth() > pos.x || nx > 0 && cap.GetPos().x - cap.GetWidth() < pos.x && cap.GetPos().x > pos.x)
			{
				if (!checkShotOnce) {
					SetState(State::EnemyWizard_ShootWhenFly);
					checkShotOnce = true;
				}
			}
			else
			{
				checkShotOnce = false;
			}
		}
		else if (pos.x > MAX_POS_X)
		{
			nx = -1;
			if (pos.y >= GROUND)
			{
				counterFly++;
				if (curState != State::EnemyWizard_FlyUp) {
					SetState(State::EnemyWizard_FlyUp);
				}
			}
			else if (pos.y <= ROOF) {
				if (curState == State::EnemyWizard_FlyUp) {
					if (pos.x > MAX_POS_X)
						pos.x = MAX_POS_X - 1;
				}
				else {
					SetState(State::EnemyWizard_FlyDown);
				}
			}
			else
			{

			}
		}
		else if (pos.x < MIN_POS_X)
		{
			nx = 1;
			if (pos.y >= GROUND) {
				counterFly++;
				if (curState != State::EnemyWizard_FlyUp) {
					SetState(State::EnemyWizard_FlyUp);
				}
			}
			else if (pos.y <= ROOF) {
				if (curState == State::EnemyWizard_FlyUp) {
					if (pos.x < MIN_POS_X)
						pos.x += 5;
				}
				else {
					SetState(State::EnemyWizard_FlyDown);
				}
			}
			else {

			}
		}
		if (counterFly > 0) {
			counterFly = 0;
			return true;
		}
	}
	else if (behavior == Behaviors::EnemyWizard_GroundShoot)
	{
		static float counterState = 0.0f;
		static int Bullets = 0;
		static float counterRunOutTime = 0.0f;
		counterRunOutTime += GameTimer::Dt();
		counterState += GameTimer::Dt();
		if (cap.GetPos().x < pos.x) nx = -1;
		else nx = 1;
		if (animations.at(curState).IsDoneCycle())
		{
			if (curState == State::EnemyWizard_ShootBullet)
			{
				SetState(State::EnemyWizard_Stand);
				counterState = 0;
			}
			else if (curState == State::EnemyWizard_ShootBulletFire)
			{
				SetState(State::EnemyWizard_Stand);
				counterState = 0;
				counterRunOutTime = 0.0f;
				return true;
			}
			else if (curState == State::EnemyWizard_Stand)
			{
				if (counterState > 0.1f)
				{
					if (Bullets < 2) {
						if (!checkShotOnce) {
							Bullets++;
							SetState(State::EnemyWizard_ShootBullet);
							checkShotOnce = true;
							counterState = 0;
						}
					}
					else {
						if (!checkShotOnce) {
							Bullets = 0;
							SetState(State::EnemyWizard_ShootBulletFire);
							checkShotOnce = true;
							counterState = 0;
						}
					}
				}
			}
		}
		else {
			checkShotOnce = false;
		}
		if (counterRunOutTime > 2.0f) return true;
	}
	else if (behavior == Behaviors::EnemyWizard_Jump)
	{
		//y=x^2-50
		static float runVerX = pos.x + nx * std::sqrt(150);
		static float runVerY = pos.y;
		static bool isJumped = false;
		static int counter = 0; counter++;
		if (!isJumped)
		{
			runVerX = pos.x + nx * std::sqrt(150);
		    runVerY = pos.y;
			isJumped = true;
			if (counter>1) {
				int a = 0;
			}
		}
		vel.x = nx * WALKING_SPEED/2;
		float tempPosY = (pos.x - runVerX)*(pos.x - runVerX) - 150;
		vel.y = (tempPosY - runVerY)/2;
		if (counter == 10) {
			int a = 0;
		}
		if (pos.y > runVerY) {
			pos.y = runVerY - 1;
			isJumped = false;
			return true; 
		}
	}
	else if (behavior == Behaviors::EnemyWizard_FlyBackCorner)
	{
		static bool isFlyDown = false;
		if (!isFlyDown) {
			if (nx < 0) {
				if (pos.y <= GROUND && pos.y >= ROOF && pos.x >= MIN_POS_X) {
					SetState(State::EnemyWizard_FlyUp);
				}
				else if (pos.y < ROOF && pos.x >= MIN_POS_X)
				{
					if (curState == State::EnemyWizard_ShootWhenFly && !animations.at(State::EnemyWizard_ShootWhenFly).IsDoneCycle()) {
						return false;
					}
					else {
						SetState(State::EnemyWizard_Flying);
					}
					if (nx < 0 && cap.GetPos().x < pos.x && cap.GetPos().x + cap.GetWidth() > pos.x || nx > 0 && cap.GetPos().x - cap.GetWidth() < pos.x && cap.GetPos().x > pos.x)
					{
						if (!checkShotOnce) {
							SetState(State::EnemyWizard_ShootWhenFly);
							checkShotOnce = true;
						}
					}
					else
					{
						checkShotOnce = false;
					}
				}
				else if (pos.x <= MIN_POS_X)
				{
					SetState(State::EnemyWizard_FlyDown);
					if (pos.y > GROUND) return true;
				}
			}
			else {
				if (pos.y <= GROUND && pos.y >= ROOF && pos.x <= MAX_POS_X) {
					SetState(State::EnemyWizard_FlyUp);
				}
				else if (pos.y < ROOF && pos.x <= MAX_POS_X)
				{
					if (curState == State::EnemyWizard_ShootWhenFly && !animations.at(State::EnemyWizard_ShootWhenFly).IsDoneCycle()) {
						return false;
					}
					else {
						SetState(State::EnemyWizard_Flying);
					}
					if (nx < 0 && cap.GetPos().x < pos.x && cap.GetPos().x + cap.GetWidth() > pos.x || nx > 0 && cap.GetPos().x - cap.GetWidth() < pos.x && cap.GetPos().x > pos.x)
					{
						if (!checkShotOnce) {
							SetState(State::EnemyWizard_ShootWhenFly);
							checkShotOnce = true;
						}
					}
					else
					{
						checkShotOnce = false;
					}
				}
				else if (pos.x >= MAX_POS_X)
				{
					SetState(State::EnemyWizard_FlyDown);
					if (pos.y > GROUND) return true;
				}
			}
		}
	}
	else if (behavior == Behaviors::EnemyWizard_RunToCap)
	{	
		static float runCounter = 0.0f;
		runCounter += GameTimer::Dt();
		if (pos.x > MAX_POS_X && nx == 1) nx = -1;
		if (pos.x < MIN_POS_X && nx == -1) nx = 1;
		if (runCounter > 1.2f) {
			runCounter = 0.0f;
			return true;
		}
		SetState(State::EnemyWizard_Walking);
	}
	else if (behavior == Behaviors::EnemyWizard_TurnOffLight)
	{
		static float counterRunOutTime = 0.0f;
		static bool moveToLight = false;
		static bool enter = true;
		counterRunOutTime += GameTimer::Dt();
		if (enter) {
			if (pos.y > GROUND) pos.y = GROUND - 1;
			enter = false;
			//if (nx > 0) return true;
			if (pos.x < LIGHT_POS_X) return true;
		}
		if (pos.y <= GROUND && pos.y >= ROOF && pos.x >= LIGHT_POS_X) {
			if (!moveToLight)
				SetState(State::EnemyWizard_FlyUp);
		}
		else if (pos.y < ROOF && pos.x >= LIGHT_POS_X)
		{
			if (!moveToLight) {
				if (curState == State::EnemyWizard_ShootWhenFly && !animations.at(State::EnemyWizard_ShootWhenFly).IsDoneCycle()) {
					return false;
				}
				else {
					SetState(State::EnemyWizard_Flying);
				}
				if (nx < 0 && cap.GetPos().x < pos.x && cap.GetPos().x + cap.GetWidth() > pos.x || nx > 0 && cap.GetPos().x - cap.GetWidth() < pos.x && cap.GetPos().x > pos.x)
				{
					if (!checkShotOnce) {
						SetState(State::EnemyWizard_ShootWhenFly);
						checkShotOnce = true;
					}
				}
				else
				{
					checkShotOnce = false;
				}
			}
			else {
				/*nx = 1;
				if (pos.y >= LIGHT_POS_X) {
					if (curState != State::EnemyWizard_ShootBullet || !animations.at(State::EnemyWizard_ShootBullet).IsDoneCycle())
					{
						SetState(State::EnemyWizard_ShootBullet);
					}
					else
					{
						enter = false;
						return true;
					}
				}
				else {
					SetState(State::EnemyWizard_Walking);
				}*/
			}
		}
		else if (pos.x <= LIGHT_POS_X)
		{
			moveToLight = true;
			if (pos.y >= LIGHT_POS_Y) {
				pos.y = LIGHT_POS_Y;
				nx = 1;
				static bool isTurned = false;
				if (!isTurned)
				{
					isTurned = true;
					SetState(State::EnemyWizard_ShootBullet);
				}
				else
				{
					if (animations.at(State::EnemyWizard_ShootBullet).IsDoneCycle()) {
						isTurned = false;
						enter = false;
						SceneManager::Instance().GetCurScene().ToggleLight();
						return true;
					}
					else {
						return false;
					}
				}
			}
			else {
				SetState(State::EnemyWizard_FlyDown);
			}
			if (pos.y > GROUND) {
				enter = false;
				return true; //case
			}
		}
		if (counterRunOutTime > 3.0f)
		{
			return true;
		}
	}
	else if (behavior == Behaviors::EnemyWizard_Laught) {
		static float counter = 0.0f;
		counter += GameTimer::Dt();
		if (counter > 1.0f) {
			counter = 0;
			return true;
		}
		SetState(State::EnemyWizard_Laught);
	}
	return false;
}

void EnemyWizard::Action()
{
	static const int ActionsLength = sizeof(ACTIONS_LIST) / sizeof(ACTIONS_LIST[0]);
	static int counterAction = 0;
	static State TranferState = State::EnemyWizard_Stand;
	if (Onbehaviors(ACTIONS_LIST[counterAction]))
	{
		static Behaviors nextBehavior = ACTIONS_LIST[counterAction + 1];
		if (counterAction < ActionsLength - 1)
		{
			nextBehavior = ACTIONS_LIST[counterAction + 1];
		}
		else
		{
			nextBehavior = ACTIONS_LIST[0];
		}
		switch (nextBehavior)
		{
		case Behaviors::EnemyWizard_FlyBackCorner:
			TranferState = State::EnemyWizard_FlyUp;
			break;
		case Behaviors::EnemyWizard_FlyingShoot:
			TranferState = State::EnemyWizard_Flying;
			break;
		case Behaviors::EnemyWizard_GroundShoot:
			TranferState = State::EnemyWizard_Stand;
			break;
		case Behaviors::EnemyWizard_Jump:
			TranferState = State::EnemyWizard_FlyUp;
			break;
		case Behaviors::EnemyWizard_RunToCap:
			TranferState = State::EnemyWizard_Walking;
			break;
		case Behaviors::EnemyWizard_TurnOffLight:
			TranferState = State::EnemyWizard_Stand;
			break;
		case Behaviors::EnemyWizard_Laught:
			TranferState = State::EnemyWizard_Stand;
			break;
		default:
			break;
		}
		if (counterAction < ActionsLength - 1) counterAction++;
		else counterAction = 0;
		SetState(TranferState);
	}
}

void EnemyWizard::HandleCollisions(float dt, const std::vector<GameObject*>& coObjects)
{
	auto coEvents = CollisionDetector::CalcPotentialCollisions(cap, coObjects, dt);
	if (coEvents.size() == 0)
	{
		return;
	}

	float min_tx, min_ty, nx, ny;
	CollisionDetector::FilterCollisionEvents(coEvents, min_tx, min_ty, nx, ny);

	if (coEvents.size() == 0) return;


	for (auto&e : coEvents)
	{
		if (auto block = dynamic_cast<Block*>(e.pCoObj))
		{
			switch (block->GetType())
			{
			case ClassId::Switch:
				pos.x += (1.0f - e.t) * vel.x;
				pos.y += (1.0f - e.t) * vel.y;
				break;
			case ClassId::PassableLedge:
				if (e.ny != 0) {
					vel.y = 0;
				}
				break;
			case ClassId::RigidBlock:
				if (e.ny != 0) {
					vel.y = 0;
				}
				break;
			default:
				break;
			}
		}
	}
}
static float groundPosY = 390.0f; // testing ground pos.y
void EnemyWizard::testing(Window &win)
{	
	if (win.IsKeyPressed(VK_UP))
		this->SetState(State::EnemyWizard_FlyUp);
	else if (win.IsKeyPressed(VK_DOWN))
	{
		if (pos.y < groundPosY)
			this->SetState(State::EnemyWizard_FlyDown);
		else
			this->SetState(State::EnemyWizard_Stand);
	}
	else if (win.IsKeyPressed(VK_LEFT))
	{
		nx = -1;
		if (pos.y < groundPosY)
			this->SetState(State::EnemyWizard_Flying);
		else
			this->SetState(State::EnemyWizard_Walking);
	}
	else if (win.IsKeyPressed(VK_RIGHT))
	{
		nx = 1;
		if (pos.y < groundPosY)
			this->SetState(State::EnemyWizard_Flying);
		else
			this->SetState(State::EnemyWizard_Walking);
	}
	if (win.IsKeyPressed('J')) // in real data we shoot base on cap pos if shoot vertical
	{
		if (curState == State::EnemyWizard_Flying || curState == State::EnemyWizard_FlyUp || curState == State::EnemyWizard_FlyDown)
			this->SetState(State::EnemyWizard_ShootWhenFly);
		else
			this->SetState(State::EnemyWizard_ShootBullet);
		SpawnBulletFire();
	}
	else if (win.IsKeyPressed('K')) // we always shoot base on cap pos
	{
		if (curState != State::EnemyWizard_Flying && curState != State::EnemyWizard_FlyUp && curState != State::EnemyWizard_FlyDown)
			this->SetState(State::EnemyWizard_ShootBullet);
		SpawnBullet();
	}	
	if (win.IsKeyPressed('B'))
		TakeDamage(1);
}

void EnemyWizard::TakeDamage(int damage)
{
	assert(damage > 0);
	if (curState == State::EnemyWizard_Defeated) return;

	health -= damage;
	//SetState(State::EnemyWizard_BeforeDefeated);
	if (health <= 0)
	{
		SetState(State::EnemyWizard_BeforeDefeated);
	}
	else 
	{
		OnFlashing(true);
	}
}
