#pragma once
#include "InvisibleObject.h"

class Spawner : public InvisibleObject
{


public:
	void OnOutOfViewPort() override;
};

