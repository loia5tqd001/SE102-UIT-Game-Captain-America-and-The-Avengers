#pragma once

class CaptainHealth
{
private:
	int health = 12;

	CaptainHealth() = default;
	CaptainHealth(const CaptainHealth&) = delete;

public:
	int Subtract(int dh);
	int Add(int dh);
	void Set(int h);
	void Draw() const;
	int Get() const { return health;}
	static CaptainHealth& Instance();

};

