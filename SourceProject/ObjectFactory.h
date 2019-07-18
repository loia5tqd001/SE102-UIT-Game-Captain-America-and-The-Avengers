#pragma once
#include "EnemyGun.h"
#include "EnemyRocket.h"
#include "Block.h"
#include "Spawner.h"
#include "AmbushTrigger.h"

struct ObjectFactory
{
	// use 'template specialization'
	// sample use: auto newEnemyGun = ObjectFactory::Create<EnemyGun>(json);
	template<class Object>
	static std::unique_ptr<Object> Create(const Json::Value& objJson)
	{}

	template<>
	static std::unique_ptr<Block> Create(const Json::Value& objJson)
	{
		return {};
	}

	template<>
	static std::unique_ptr<Spawner> Create(const Json::Value& objJson)
	{
		// may call Create<EnemyGun>, Create<EnemyRocket> below...
		return {};
	}

	template<>
	static std::unique_ptr<AmbushTrigger> Create(const Json::Value& objJson)
	{
		// may call Create<EnemyGun>, Create<EnemyRocket> below...
		return {};
	}

	template<>
	static std::unique_ptr<EnemyGun> Create(const Json::Value& objJson)
	{
		return {};
	}

	template<>
	static std::unique_ptr<EnemyRocket> Create(const Json::Value& objJson)
	{
		return {};
	}
};

