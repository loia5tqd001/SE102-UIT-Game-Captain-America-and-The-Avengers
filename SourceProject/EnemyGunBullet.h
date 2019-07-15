#pragma once
class EnemyGunBullet :
	public VisibleObject
{
private:
	static constexpr float BULLET_MOVING = 800.0f;

public:
	EnemyGunBullet(const Vector2& spawnPos, const Vector2& vel);

	RectF GetBBox() const override;

	void Update(float dt, const std::vector<GameObject*>& coObjects) override;
	void Initnx(int nx);
};

