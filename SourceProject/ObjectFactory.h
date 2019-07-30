#pragma once
#include "Block.h"
#include "Spawner.h"
#include "AmbushTrigger.h"
#include "Capsule.h"
#include "Bunker.h"
#include "BreakableLedge.h"
#include "MovingLedgeUpdater.h"

struct ObjectFactory
{
	// use 'template specialization'
	// sample use: auto newEnemyGun = ObjectFactory::Create<EnemyGun>(json);
	template<class Object>
	static std::unique_ptr<Object> Create(const Json::Value&, Grid*)
	{
		AssertUnreachable();
	}

	template<>
	static std::unique_ptr<Block> Create(const Json::Value& objJson, Grid* grid)
	{
		const auto blockType = (ClassId)objJson[0].asInt();
		const auto pos = Vector2{ objJson[1].asFloat(), objJson[2].asFloat() };
		const auto width = objJson[3].asUInt();
		const auto height = objJson[4].asUInt();
		return std::make_unique<Block>(blockType, pos, width, height);
	}

	template<>
	static std::unique_ptr<BreakableLedge> Create(const Json::Value& objJson, Grid* grid)
	{
		assert(objJson[0].asInt() == (int)ClassId::BreakableLedge);
		const auto pos = Vector2{ objJson[1].asFloat(), objJson[2].asFloat() };
		return std::make_unique<BreakableLedge>(pos);
	}

	template<>
	static std::unique_ptr<MovingLedgeUpdater> Create(const Json::Value& objJson, Grid* grid)
	{
		assert(objJson[0].asInt() == (int)ClassId::MovingLedgeUpdater);
		const auto pos = Vector2{ objJson[1].asFloat(), objJson[2].asFloat() };
		const auto width = objJson[3].asUInt();
		const auto height = objJson[4].asUInt();
		return std::make_unique<MovingLedgeUpdater>(pos, width, height, grid);
	}

	template<>
	static std::unique_ptr<Bunker> Create(const Json::Value& objJson, Grid* grid)
	{
		assert(objJson[0].asInt() == (int)ClassId::Bunker);
		const auto initState = objJson[1].asInt() + (int)State::Bunker_Idle_0;
		const auto pos = Vector2{ objJson[2].asFloat(), objJson[3].asFloat() };
		return std::make_unique<Bunker>((State)initState, pos, grid);
	}

	template<>
	static std::unique_ptr<Capsule> Create(const Json::Value& objJson, Grid* grid)
	{
		assert(objJson[0].asInt() == (int)ClassId::Capsule);
		const auto itemType = (SpriteId)objJson[1].asInt();
		const auto maxY = objJson[2].asFloat();
		const auto pos = Vector2{ objJson[3].asFloat(), objJson[4].asFloat() };
		const auto noBg = objJson[5].asBool();
		return std::make_unique<Capsule>(pos, itemType, maxY, grid, noBg);
	}

	template<>
	static std::unique_ptr<Spawner> Create(const Json::Value& objJson, Grid* grid)
	{
		assert(objJson[0].asInt() == (int)ClassId::Spawner);
		const auto behavior = (Behaviors)objJson[1].asInt();
		const auto pos = Vector2{ objJson[2].asFloat(), objJson[3].asFloat() };
		const auto width = objJson[4].asUInt();
		const auto height = objJson[5].asUInt();
		const auto spawnPos = Vector2{ objJson[6].asFloat(), objJson[7].asFloat() };
		const auto expectCapNx = pos.x < spawnPos.x ? 1 : -1; 
		return std::make_unique<Spawner>(pos, width, height, behavior, spawnPos, expectCapNx, Data{}, grid);
	}

	template<>
	static std::unique_ptr<AmbushTrigger> Create(const Json::Value& objJson, Grid* grid)
	{
		assert(objJson[0].asInt() == (int)ClassId::AmbushTrigger);
		const auto lockRegion = RectF{
			objJson[1][0].asFloat(),
			objJson[1][1].asFloat(),
			objJson[1][2].asFloat(),
			objJson[1][3].asFloat()
		};
		const auto pos = Vector2{ objJson[2].asFloat(), objJson[3].asFloat() };
		const auto width = objJson[4].asUInt();
		const auto height = objJson[5].asUInt();
		return std::make_unique<AmbushTrigger>(pos, width, height, lockRegion, grid);
	}
};

