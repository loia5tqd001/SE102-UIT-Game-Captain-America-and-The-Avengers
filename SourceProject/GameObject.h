#pragma once

class GameObject
{
protected: 
	Vector2 pos; 
	Vector2 vel;
	D3DCOLOR bboxColor;

public: 
	void GetDxDy(float dt, float& dx, float& dy) const;
	static void FlipPosXToLeft(float &xFlip,const float xHost, const int widthFlip,const int widthHost) //flip object that its pos are base on another Host Object pos
	{
		xFlip = 2 * xHost - xFlip + widthHost - widthFlip;
	};
	void SetPos(const Vector2& p) { pos = p; }

	Vector2 GetPos      () const;
	Vector2 GetVelocity () const;

	virtual void ClampWithin(const RectF& theBox);
	virtual void RenderBoundingBox() const; // draw bounding box for better debugging

	virtual State GetState() const = 0;
	virtual RectF GetBBox () const = 0;

	virtual void Update(float dt, const std::vector<GameObject*>& coObjects = {}) = 0;
	virtual void Render() const = 0;
	virtual void OnOutOfViewPort() {}

	virtual ~GameObject() = default; 
	GameObject(const GameObject&) = delete; 
	GameObject(Vector2 pos, Vector2 vel = { 0.0f, 0.0f });
	
#if DEBUG
	bool is_debugging = false; // To select which objects targeted for debugging
#endif

};




 