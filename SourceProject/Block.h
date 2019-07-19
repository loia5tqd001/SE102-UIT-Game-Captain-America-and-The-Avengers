#pragma once
#include "InvisibleObject.h"

class Block : public InvisibleObject
{
private:
	const ClassId type;

public:
	Block(ClassId type, Vector2 pos, UINT w, UINT h);

	inline const auto& GetType() const { return type; } // for captain to handle collision
};
