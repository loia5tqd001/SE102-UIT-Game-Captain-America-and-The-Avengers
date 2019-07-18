#pragma once
#include "InvisibleObject.h"

enum class InvisibleBlocks
{
	RigidBlock     = 0, // can't pass through
	Water          = 1, 
	PassableLedge  = 2, 
	ClimbableBar   = 3,
	DamageBlock    = 4, // sprike, electric wall
	Switch         = 5,
	NextMap        = 6,
	Door           = 7,
};

using Data = std::unordered_map<std::string, std::any>; // property-value look-up for data of any type of block

class Block : public InvisibleObject
{
private:
	const InvisibleBlocks type;
	Data data;

public:
	Block(InvisibleBlocks type, Vector2 pos, UINT w, UINT h, Data&& customData = {});

	inline const auto& GetType() const { return type; } // for captain to handle collision

	template<typename T>
	inline auto Get(const std::string& property) const
	{
		assert(data.count(property) == 1);
		return std::any_cast<T>( data.at(property) );
	}
};

/*  Sample use of 'Data':
	Data data;
	data.emplace("water-velocity", 16.9f);
	data.emplace("damage", 420);
	
	Block block( ,,,, std::move(data) );
	cout << block.Get<float>("water-velocity") << endl;
	cout << block.Get<int>("damage") << endl;;
*/

