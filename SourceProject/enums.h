#pragma once

enum class TextureId
{
	Bbox                        = 0,
	Warning                     = 1,
	KeyCodeFont                 = 2,
	CrtEffect                   = 3,
	MapCharleston               = 4,
	MapBossCharlestonLight      = 5,
	MapBossCharlestonDark       = 6,
	MapPittsburghDark           = 7,
	MapPittsburghLight          = 8,
	MapRedAlert                 = 9,
	enemy                       = 10,
	captain                     = 11,
	explode                     = 12,
	Count
};

enum class SpriteId
{
	Invisible			    = 0,

	EnemyGun_Walking		= 1,
	EnemyGun_Stand			= 2,
	EnemyGun_OnKnee			= 3,
	EnemyGun_TakeDamage		= 4,
	EnemyGunBullet			= 5,
	EnemyRocket_Walking     = 6,
	EnemyRocket_Stand       = 7,
	EnemyRocket_OnKnee      = 8,
	EnemyRocket_TakeDamage = 9,
	EnemyRocketBullet_Horizontal = 10,
	EnemyRocketBullet_Cross = 11,
	Explode =12,
	Count
};

enum class SoundId
{
	Greeting       = 0,
	Cursor         = 1,
	CaptainTheme   = 2,
	BossBattle     = 3,
	RedAlert       = 4,
	Ambush         = 5,
	Death          = 6,
	Count
};

enum class ClassId
{
	Captain  = 0,
	Count
};

enum class State
{
	Invisible,
	Destroyed,

	EnemyGun_Walking,
	EnemyGun_Stand,
	EnemyGun_OnKnee,
	EnemyGun_TakeDamage,
	EnemyGunBullet,
	EnemyRocket_Walking,
	EnemyRocket_Stand,
	EnemyRocket_OnKnee,
	EnemyRocket_TakeDamage,
	EnemyRocketBullet_Horizontal,
	EnemyRocketBullet_Cross,
	Explode,

	Count


};

enum class Scene
{
	Greeting,
	Charleston,
	BossCharleston,
	Pittsburgh,
	RedAlert
};




