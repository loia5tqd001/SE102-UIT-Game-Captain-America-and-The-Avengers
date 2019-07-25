#include "pch.h"

void Tile::Update(float dt)
{
	if (frames.size() == 1) return;

	holdingTime += dt;

	while (holdingTime >= holdTime)
	{
		holdingTime -= holdTime;
		curFrame++;
		if (curFrame >= frames.size()) curFrame = 0;
	}
}

Map::Map(const Json::Value & root)
{
	LoadResources(root);
}

Rect Map::GetPortionFromTileNumber(int number, int columns) const
{
	if (number == 0) return {};
	long left   = ((number - 1) % columns) * tileSize;
	long top    = ((number - 1) / columns) * tileSize;
	long right  = left + tileSize;
	long bottom = top  + tileSize;

	return { left, top, right, bottom };	
}

void Map::LoadResources(const Json::Value& root)
{
	const Json::Value&   tileMap = root["map"];
	const UINT textureId = tileMap["texture-id"].asUInt();
	const UINT columns   = tileMap["columns"   ].asUInt();
	           tileSize  = tileMap["tilesize"  ].asUInt();
	           height    = tileMap["height"    ].asUInt();
	           width     = tileMap["width"     ].asUInt();

	texture = Textures::Get( (TextureId)textureId );
	worldBoundary = { 0.0f, 0.0f, width * tileSize, height * tileSize };
	tiles.resize(height * width);

	const Json::Value& data = tileMap["data"];
	for (UINT y = 0; y < height; y++)
	for (UINT x = 0; x < width ; x++)
	{
		// construct position ..
		auto& tile = tiles[y * width + x];
		tile.position = { x * tileSize, y * tileSize };

		// construct frames ..
		const auto jsonTile = data[y * width + x];

		if (jsonTile.isNumeric()) // static tile
		{
			tile.frames = { GetPortionFromTileNumber(jsonTile.asInt(), columns) };
		}
		else // dynamic tile
		{
			for (auto& json : jsonTile)
			{
				tile.frames.emplace_back( GetPortionFromTileNumber(json.asInt(), columns) );
			}
		}
	}
}

void Map::UpdateAnimatedTiles(float dt)
{
	for (auto& tile : tiles)
	{
		tile.Update(dt);
	}
}

void Map::Render()
{
	if (DebugDraw::IsInDeepDebug()) return;

	const auto& cam = Camera::Instance();
	const auto camBbox = cam.GetBBox();

	const auto xs = UINT(max(0         , camBbox.left   / tileSize));
	const auto xe =	UINT(min(width  - 1, camBbox.right  / tileSize));
	const auto ys =	UINT(max(0         , camBbox.top    / tileSize));
	const auto ye = UINT(min(height - 1, camBbox.bottom / tileSize));

	for (UINT y = ys; y <= ye; y++)
	for (UINT x = xs; x <= xe; x++)
	{
		auto& tile = tiles[y * width + x];
		auto drawablePos = cam.GetPositionInViewPort(tile.position);
		Game::Instance().Draw(drawablePos, texture, tile.GetCurFrame());
	}
}
