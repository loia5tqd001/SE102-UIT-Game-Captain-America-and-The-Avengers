#include "pch.h"

VisibleObject::VisibleObject(State initState, Vector2 pos, Vector2 vel, int nx) :
	GameObject(pos, vel),
	curState(initState), 
	nx(nx),
	timePassed(0.0f),
	nFrameRendered(0u)
{
	assert(std::abs(nx) == 1.0f);
	animations.emplace( State::Destroyed, Animation(SpriteId::Invisible) );
}

void VisibleObject::FlipHorizontally()
{
	assert(std::abs(nx) == 1);
	nx = -nx;
}

void VisibleObject::OnFlashing(std::optional<bool> setFlashing)
{
	if (setFlashing.has_value()) {
		isFlashing = setFlashing.value();
		timePassed = 0.0f;
		nFrameRendered = 0;
	}

	if (isFlashing == false) { // currently not in flashing
		shouldDrawImage = true;
	}
	else if ((timePassed += GameTimer::Dt()) <= timeFlashing) { // if in flashing, accumulate timePassed, check if still in flashing
		if (shouldDrawImage) { // is in rendering mode
			if (++nFrameRendered > nFrameToRender) {
				nFrameRendered = 0;
				shouldDrawImage = !shouldDrawImage;
			}
		}
		else { // is in unrendering mode
			if (++nFrameUnrendered > nFrameToUnrender) {
				nFrameUnrendered = 0;
				shouldDrawImage = !shouldDrawImage;
			}
		}
	}
	else { // after calculating timePassed, realise time to done flashing
		shouldDrawImage = true;
		timePassed = 0.0f;
		isFlashing = false;
	}
}

State VisibleObject::GetState() const
{
	assert(animations.count(curState) == 1);
	return curState;
}

RectF VisibleObject::GetBBox() const
{
	assert(animations.count(curState) == 1);
	const auto animationFrame = animations.at(curState).GetFrameSize();
	return { pos, animationFrame.GetWidth(), animationFrame.GetHeight() };
}

int VisibleObject::GetWidth() const
{
	return (int) animations.at(curState).GetFrameSize().GetWidth();
}

int VisibleObject::GetHeight() const
{
	return (int) animations.at(curState).GetFrameSize().GetHeight();
}

void VisibleObject::Render() const
{
	assert(animations.count(curState) == 1);
	assert(std::abs(nx) == 1);

	if (shouldDrawImage && !DebugDraw::IsInDeepDebug()) 
	{
		const auto drawablePosition = Camera::Instance().GetPositionInViewPort( pos );
		animations.at(curState).Render( drawablePosition, { (float)nx, 1.0f } ); // draw object's actual image
	}
	
	RenderBoundingBox(); // draw object's bounding box for better debugging
}

void VisibleObject::SetState(State state)
{
	assert(animations.count(state) == 1); // make sure the state already has a corresponding animation 
	const auto oldHeight = GetHeight();
	const auto oldWidth = GetWidth();
	curState = state;
	if (state == State::Explode) Sounds::PlayAt(SoundId::Explosion);
	if (dynamic_cast<Shield*>(this))
	{
		return;
	}
	pos.x += float(oldWidth - GetWidth()) / 2.0f;
	pos.y += float(oldHeight - GetHeight());
}
