#include "pch.h"
#include "Capsule.h"

//Capsule::Capsule(Vector2 pos, State initState):VisibleObject(initState,pos),OpenTime(1.0f)
//{
//	animations.emplace(State::ItemNotMoving, Animation(SpriteId::CapsuleNotMoving));
//	animations.emplace(State::ItemMoving, Animation(SpriteId::CapsuleMoving));
//}
//
//void Capsule::Update(float dt, const std::vector<GameObject*>& coObjects)
//{
//	switch (curState)
//	{
//	case State::ItemNotMoving:
//		break;
//	case State::ItemMoving:
//	{
//		this->OpenTime -= GameTimer::Dt();
//		if (OpenTime<0)
//		{
//			SetState(State::ItemNotMoving);
//			OpenTime = 1.0f;
//		}
//		break;
//	}
//	}
//}
