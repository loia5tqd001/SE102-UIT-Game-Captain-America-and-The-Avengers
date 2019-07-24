#pragma once

class VisibleObject : public GameObject
{
protected:
	int nx      ;
	State curState;
	std::unordered_map<State, Animation> animations;
	bool shouldDrawImage = true;

	float timeFlashing = 0.3f;
	bool  isFlashing = false;
	float timePassed = 0.0f;
	UINT  nFrameUnrendered = 0u;
	void  OnFlashing(std::optional<bool> setFlashing = {});

public:
	VisibleObject(State initState, Vector2 pos, Vector2 vel = { 0.0f, 0.0f }, int nx = 1);

	void FlipHorizontally();
	inline int GetNx() const { return nx;}
	State GetState() const override;
	RectF GetBBox () const override;
	virtual int GetWidth () const;
	virtual int GetHeight() const;

	void Update(float dt, const std::vector<GameObject*>& coObjects = {}) override {}
	void Render() const override;

	virtual void SetState(State state);
};
