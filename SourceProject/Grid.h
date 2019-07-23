#pragma once

struct Cell
{
	RectF boundingBox;
	std::unordered_set<std::shared_ptr<GameObject>> staticObjects; 
	std::unordered_set<std::shared_ptr<GameObject>> movingObjects; // objects moving around from cells to cells
	inline const RectF& GetBBox() const { return boundingBox; }
};

struct Area { UINT xs = 0, xe = 0, ys = 0, ye = 0; };
enum class IsMoving { Moving, Static };

// We implement grid by this: https://tranminhtuan11a1.blogspot.com/2014/06/
class Grid 
{
private:
	UINT cellSize, width, height;
	std::vector<Cell> cells;
	std::vector<GameObject*> curObjectsInViewPort; // being recalculated every frame 
	Area viewPortArea; // being recalculated every frame

	Grid(const Grid&) = delete;
	Area GetCells(const RectF& bbox, int broadX = 0, int broadY = 0) const;
	void LoadObjects(const Json::Value& grid);
	void LoadResources(const Json::Value& root);

public:
	Grid(const Json::Value& root);
	void UpdateCells();
	void RenderCells() const; // draw cells for better debugging
	GameObject* SpawnObject(std::shared_ptr<GameObject> obj, IsMoving isMoving = IsMoving::Moving);
	inline const auto& GetObjectsInViewPort() const { return curObjectsInViewPort; }
	inline RectF GetGridBBox() const { return { {}, cellSize*width, cellSize*height }; }
};

