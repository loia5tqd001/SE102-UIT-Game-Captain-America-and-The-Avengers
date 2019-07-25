#pragma once

struct Tile
{
	static constexpr auto holdTime = 0.1f;

	Vector2 position; // on world map
	int curFrame = 0;
	float holdingTime; // time holded in this tile
	std::vector<Rect> frames; // portions in texture

	inline const Rect& GetCurFrame() const { return frames.at(curFrame); } // to draw
	void Update(float dt); // for animated tiles to update
};

class Map
{
private:
	LPDIRECT3DTEXTURE9 texture;
	UINT height, width, tileSize;
	RectF worldBoundary;
	std::vector<Tile> tiles;

	Rect GetPortionFromTileNumber(int number, int columns) const;
public:
	Map(const Map&) = delete;
	Map(const Json::Value& root);

	void LoadResources(const Json::Value& root);
	void UpdateAnimatedTiles(float dt);
	void Render();

	inline RectF GetWorldBoundary() const { return worldBoundary; }
	inline UINT  GetMapWidth     () const { return width  * tileSize; }
	inline UINT  GetMapHeight    () const { return height * tileSize; }
	inline UINT  GetTileSize     () const { return          tileSize; }
};