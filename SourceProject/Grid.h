#pragma once

struct Cell
{
	RectF boundingBox;
	std::unordered_set<GameObject*> staticObjects; 
	std::unordered_set<GameObject*> movingObjects; // objects moving around from cells to cells
	const RectF& GetBBox() const { return boundingBox; }
};

struct Area { UINT xs = 0, xe = 0, ys = 0, ye = 0; };

// We implement grid by this: https://tranminhtuan11a1.blogspot.com/2014/06/
class Grid 
{
private:
	UINT cellSize, width, height;
	std::vector<Cell> cells;
	std::vector<std::unique_ptr<GameObject>> objectHolder; // responsible for deleting objects, use this to avoid using slow and complicated shared_ptr
	std::vector<GameObject*> curObjectsInViewPort; // being recalculated every frame 
	Area viewPortArea;

private:
	Grid(const Grid&) = delete;
	Area CalcCollidableArea(const RectF& bbox, int broadX = 0, int broadY = 0) const;

	void LoadObjects(const Json::Value& grid);
	void LoadResources(const Json::Value& root);
	void RemoveDestroyedObjects();
	void RecalculateObjectsInViewPort();

public:
	Grid(const Json::Value& root);

	void UpdateCells();
	void RenderCells() const;

	GameObject* SpawnObject(std::unique_ptr<GameObject> obj, bool isMoving = true);
	inline const auto& GetObjectsInViewPort() const { return curObjectsInViewPort; }

};

