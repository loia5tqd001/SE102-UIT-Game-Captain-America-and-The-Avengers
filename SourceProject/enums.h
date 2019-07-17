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
	Captain                     = 10,
	Enemies                     = 11,
	Wizard                      = 12,
	Dynamite                    = 13,
};

enum class SpriteId
{
	Invisible			            =  0,
							           
	EnemyGun_Stand			        =  1,
	EnemyGun_Walking		        =  2,
	EnemyGun_TakeDamage		        =  3,
	EnemyGun_Sitting                =  4,
	BulletEnemyGun			        =  5,

	EnemyRocket_Stand               =  6,
	EnemyRocket_Walking             =  7,
	EnemyRocket_TakeDamage          =  8,
	EnemyRocket_Sitting             =  9,
	BulletEnemyRocket_Horizontal    = 10,
	BulletEnemyRocket_Cross         = 11,

	Explode                         = 12,

	ItemSmallPowerStone             = 13,
	ItemBigPowerStone               = 14,
	ItemFivePointItem               = 15,
	ItemSmallEnergy                 = 16,
	ItemBigEnergy                   = 17,
	ItemKeyKrystal                  = 18,
	ItemExitSign                    = 19
};

enum class SoundId
{
	Invalid          = -1,
	Greeting         =  0,
	Cursor           =  1,
	CaptainTheme     =  2,
	BossBattle       =  3,
	RedAlert         =  4,
	Ambush           =  5,
	Death            =  6,
	ItemKrystal      =  7,
	ItemEnergy       =  8,
	ItemNormal       =  9,
	BulletNormal     = 10,
	BulletLazer      = 11,
	Explosion        = 12,
	ShieldThrow      = 13,
	ShieldCollide    = 14,
	Punch            = 15,
	Tackle           = 16,
	HighJump         = 17,
	Grounding        = 18,
	Water            = 19,
};

enum class ClassId
{
	Captain  = 0,
};

enum class State
{
	Invisible,
	Destroyed,

	EnemyGun_Walking,
	EnemyGun_Stand,
	EnemyGun_Sitting,
	EnemyGun_TakeDamage,
	BulletEnemyGun,

	EnemyRocket_Walking,
	EnemyRocket_Stand,
	EnemyRocket_Sitting,
	EnemyRocket_TakeDamage,
	BulletEnemyRocket_Horizontal,
	BulletEnemyRocket_Cross,

	Explode,
	
	ItemFalling,
	ItemNotMoving
};

enum class Scene
{
	Greeting,
	Charleston,
	BossCharleston,
	Pittsburgh,
	RedAlert
};




