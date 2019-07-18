#pragma once
#include "EnemyGun.h"
#include "EnemyRocket.h"
#include "Block.h"
#include "Spawner.h"
#include "AmbushTrigger.h"
#include "Capsule.h"

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
	static std::unique_ptr<Capsule> Create(const Json::Value& objJson, Grid* grid)
	{
		const auto itemType = (SpriteId)objJson[1].asInt();
		const auto maxY = objJson[2].asFloat();
		const auto pos = Vector2{ objJson[3].asFloat(), objJson[4].asFloat() };
		return std::make_unique<Capsule>(pos, itemType, maxY, grid);
	}

	template<>
	static std::unique_ptr<Spawner> Create(const Json::Value& objJson, Grid* grid)
	{
		// may call Create<EnemyGun>, Create<EnemyRocket> below...
		return {};
	}

	template<>
	static std::unique_ptr<AmbushTrigger> Create(const Json::Value& objJson, Grid* grid)
	{
		// may call Create<EnemyGun>, Create<EnemyRocket> below...
		return {};
	}

	template<>
	static std::unique_ptr<EnemyGun> Create(const Json::Value& objJson, Grid* grid)
	{
		return {};
	}

	template<>
	static std::unique_ptr<EnemyRocket> Create(const Json::Value& objJson, Grid* grid)
	{
		return {};
	}
};

