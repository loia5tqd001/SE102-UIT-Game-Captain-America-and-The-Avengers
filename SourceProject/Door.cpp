#include "pch.h"
#include "Door.h"

Door::Door(Vector2 pos) : VisibleObject(State::DoorDarkIdle, pos)
{
	animations.emplace(State::DoorDarkIdle, Animation(SpriteId::DoorDarkIdle));
	animations.emplace(State::DoorDarkOpenning, Animation(SpriteId::DoorDarkOpenning, 0.1f));
	animations.emplace(State::DoorLightIdle, Animation(SpriteId::DoorLightIdle));
	animations.emplace(State::DoorLightOpenning, Animation(SpriteId::DoorLightOpenning, 0.1f));
}

void Door::Update(float dt, const std::vector<GameObject*>& coObjects)
{
	isDark = SceneManager::Instance().GetCurScene().isDark;

	if (curState == State::DoorDarkIdle || curState == State::DoorLightIdle)
	{
		if (isDark) curState = State::DoorDarkIdle;
		else curState = State::DoorLightIdle;
		return;
	}

	// else: openning (only update when state is openning)
	animations.at(curState).Update(dt);
	if (animations.at(curState).IsDoneCycle())
	{
		if (curState == State::DoorDarkOpenning) curState = State::DoorDarkIdle;
		else if (curState == State::DoorLightOpenning) curState = State::DoorLightIdle;
		SceneManager::Instance().GetCurScene().Teleport();
		dynamic_cast<PittsburghScene&>(SceneManager::Instance().GetCurScene()).isPauseGameExceptDoor = false;
	}
}

void Door::Open()
{
	if (curState == State::DoorDarkOpenning || curState == State::DoorLightOpenning) return; // is on openning already
	if (isDark) curState = State::DoorDarkOpenning;
	else curState = State::DoorLightOpenning;
	Sounds::PlayAt(SoundId::DoorOpen);
	dynamic_cast<PittsburghScene&>(SceneManager::Instance().GetCurScene()).isPauseGameExceptDoor = true;
}

RectF Door::GetBBox() const
{
	return VisibleObject::GetBBox().Trim(8, 0, 8, 0);
}
