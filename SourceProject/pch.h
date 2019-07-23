#pragma once

// === Winapi, directx ===
#include <d3dx9.h>

// === Stl library ===
#include <algorithm>
#include <assert.h>
#include <string>
#include <sstream>
#include <fstream>
#include <memory>
#include <optional>
#include <iterator>
#include <any>

// === STL containers ===
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <queue>

// === Third party library ===
#include <json/json.h>
#include <dsutil.h>

// === My utils ===
#include "DebugOut.h"
#include "Colors.h"
#include "RectF.h"
#include "MyException.h"
#include "GameTimer.h"
#include "Random.h"
#include "Utils.h"
#include "Data.h"

// === Resources ===
#include "enums.h"
#include "Textures.h"
#include "Font.h"
#include "KeyCodeFonts.h"
#include "Sounds.h"
#include "Sprite.h"
#include "Sprites.h"
#include "Animation.h"

// === Objects ===
#include "GameObject.h"
#include "VisibleObject.h"
#include "InvisibleObject.h"

// === Managers ===
#include "Collision.h"
#include "Window.h"
#include "Game.h"
#include "Camera.h"
#include "Grid.h"
#include "Map.h"
#include "Settings.h"
#include "DebugDraw.h"
#include "AbstractScene.h"
#include "SceneManager.h"

// == Ingame Objects == 
#include "CaptainHealth.h"
#include "ExitSign.h"
#include "Capsule.h"
#include "Item.h"

#include "Block.h"
#include "Spawner.h"
#include "AmbushTrigger.h"
#include "BreakableLedge.h"
#include "MovingLedge.h"

#include "Enemy.h"
#include "Bullet.h"

#include "Shield.h"

#include "CaptainStanding.h"
#include "CaptainWalking.h"
#include "CaptainJumping.h"
#include "CaptainKicking.h"
#include "CaptainSpinning.h"
#include "CaptainThrowing.h"
#include "CaptainTackle.h"
#include "CaptainPunching.h"
#include "CaptainSitting.h"
#include "CaptainSitPunching.h"
#include "CaptainCoverTop.h"
#include "CaptainCoverLow.h"
#include "CaptainFallToWater.h"
#include "CaptainInWater.h"
#include "CaptainSwimming.h"
#include "CaptainClimbing.h"
#include "CaptainInjured.h"
#include "CaptainDead.h"
#include "CaptainFalling.h"
#include "Captain.h"



