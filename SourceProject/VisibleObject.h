#pragma once

class VisibleObject : public GameObject
{
protected:
	FLOAT nx      ; 
	State curState;
	std::unordered_map<State, Animation> animations;
	bool shouldDrawImage = true;

public:
	VisibleObject(State initState, Vector2 pos, Vector2 vel = { 0.0f, 0.0f }, FLOAT nx = 1.0f);

	void FlipHorizontally();

	State GetState() const override;
	RectF GetBBox () const override;
	virtual int GetWidth () const;
	virtual int GetHeight() const;

	void Update(float dt, const std::vector<GameObject*>& coObjects = {}) override {}
	void Render() const override;

	virtual void SetState(State state);
};
