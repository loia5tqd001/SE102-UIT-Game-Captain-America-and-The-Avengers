#include "pch.h"
#include "CaptainFallToWater.h"



void CaptainFallToWater::Enter(Captain& cap, State fromState, Data&& data)
{
	//Todo: posx.y = .... Cap's positon must be underwater
	cap.pos.y = WATER_LEVEL - cap.GetBBox().GetHeight();
	posy = cap.pos.y;
	cap.vel.x = WATER_FLOW_SPEED * 2 / 3;
	cap.vel.y = 110.0f;
}

Data CaptainFallToWater::Exit(Captain& cap, State toState)
{
	Sounds::PlayAt(SoundId::Water);
	return Data{};
}

void CaptainFallToWater::OnKeyUp(Captain& cap, BYTE keyCode)
{
}

void CaptainFallToWater::OnKeyDown(Captain& cap, BYTE keyCode)
{
	//Jump out the water
	if (keyCode==setting.Get(KeyControls::Jump))
	{
		cap.SetState(State::Captain_Jumping);
		//NOTE:this seems not right according to physics :))  // anyway ~
		//SORRY THIS CAUSE BUG IN MY CODE SO NOPE
	}
}

void CaptainFallToWater::Update(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
	if (cap.animations.at(cap.curState).GetCurFrameIndex()==0||
		cap.animations.at(cap.curState).GetCurFrameIndex() == 1)
	{
		cap.pos.x += cap.vel.x*dt;
		cap.pos.y += cap.vel.y*dt;
	}
	else
	{
		cap.pos.y = posy;
	}
	if (cap.animations.at(cap.curState).IsDoneCycle())
	{
		cap.SetState(State::Captain_Swimming);
		return;
	}

	cap.pos.x += WATER_FLOW_SPEED * dt;
}

void CaptainFallToWater::HandleCollisions(Captain& cap, float dt, const std::vector<GameObject*>& coObjects)
{
}
