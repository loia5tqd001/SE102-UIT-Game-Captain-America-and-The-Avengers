#pragma once

enum class TextureId
{
	Bbox              = 0,
	Greeting          = 1,
	KeyCodeFont       = 2,
	CrtEffect         = 3,
	MapCharleston     = 4,
	MapBossCharleston = 5,
	Count
};

enum class SpriteId
{
	Invisible         = 0,
	Count
};

enum class SoundId
{
	Greeting       = 0,
	Cursor         = 1,
	CaptainTheme   = 2,
	BossBattle     = 3,
	RedAlert       = 4,
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
	Count


};

enum class Scene
{
	Greeting,
	Transition,
	Charleston,
	BossCharleston,
};




