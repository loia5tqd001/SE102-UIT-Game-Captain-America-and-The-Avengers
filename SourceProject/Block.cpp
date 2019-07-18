#include "pch.h"
#include "Block.h"

Block::Block(ClassId type, Vector2 pos, UINT w, UINT h, Data&& customData) :
	InvisibleObject(pos, w, h),
	type(type),
	data(customData)
{}

