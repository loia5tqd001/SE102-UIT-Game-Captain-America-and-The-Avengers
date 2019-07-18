#pragma once

class ExitSign
{
private:
	bool isKrystalCollected;
	float timePassedAfterKrystalCollected;
	Vector2 krystalPos;

	ExitSign();
	ExitSign(const ExitSign&) = delete;

public:
	void Draw();
	bool IsKrystalCollected() const { return isKrystalCollected; } // check if captain can move to next stage
	void KrystalCollected(const Vector2& krystalPos); // call when krystal item is collected
	void ResetNewStage(); // call when captain move to next stage
	static ExitSign& Instance();

};

