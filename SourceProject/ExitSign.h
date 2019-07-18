#pragma once
#include "VisibleObject.h"

class ExitSign : public VisibleObject
{
private:
	bool isKrystalCollected;
	int nFramesAfterKrystalCollected;
	Vector2 krystalPos;

	ExitSign();
	ExitSign(const ExitSign&) = delete;

public:
	void Update(float dt, const std::vector<GameObject*>& coObjects = {}) override; 
	void Render() const override;
	bool IsKrystalCollected() const { return isKrystalCollected; } // check if captain can move to next stage
	void KrystalCollected(const Vector2& krystalPos); // call when krystal item is collected
	void ResetNewStage(); // call when captain move to next stage
	static ExitSign& Instance();

};

