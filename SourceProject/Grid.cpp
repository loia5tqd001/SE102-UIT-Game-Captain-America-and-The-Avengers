#include "pch.h"
#include "ObjectFactory.h"

Grid::Grid(const Json::Value& root)
{
	LoadResources(root);
}

void Grid::LoadResources(const Json::Value& root)
{		
	const Json::Value& grid = root["grid"];
	cellSize = grid["cellsize"].asUInt();
	width    = grid["width"   ].asUInt();
	height   = grid["height"  ].asUInt();

	cells.resize(width * height);
	for (UINT x = 0; x < width; x++)
	for (UINT y = 0; y < height; y++)
	{
		cells[x * height + y].boundingBox = { float(x * cellSize), float(y * cellSize), cellSize, cellSize };
	}

	LoadObjects(grid);
}

void Grid::LoadObjects(const Json::Value& grid)
{
	const Json::Value& jsonObjects = grid["objects"];
	objectHolder.reserve( jsonObjects.size() );

	for (const auto& jsonObj : jsonObjects)
	{
		static std::unique_ptr<GameObject> object;
		const int classId  = jsonObj[0].asInt();
		switch ((ClassId)classId)
		{
			case ClassId::RigidBlock    :  
			case ClassId::Water         :  
			case ClassId::PassableLedge :  
			case ClassId::ClimbableBar  :  
			case ClassId::DamageBlock   :  
			case ClassId::Switch        :  
			case ClassId::NextMap       :  
			case ClassId::Door   		:
				object = ObjectFactory::Create<Block>(jsonObj, this);
				break;

			case ClassId::Capsule:
				object = ObjectFactory::Create<Capsule>(jsonObj, this);
				break;

			case ClassId::Spawner:
				object = ObjectFactory::Create<Spawner>(jsonObj, this);
				break;

			case ClassId::AmbushTrigger:
				object = ObjectFactory::Create<AmbushTrigger>(jsonObj, this);
				break;

			default:
				ThrowMyException("Can't find class id:", classId);
		}

		SpawnObject(std::move(object), false);
	}
}

GameObject* Grid::SpawnObject(std::unique_ptr<GameObject> obj, bool isMoving)
{
	Area area = CalcCollidableArea( obj->GetBBox() );

	for (UINT x = area.xs; x <= area.xe; x++)
	for (UINT y = area.ys; y <= area.ye; y++)
	{
		if (isMoving) {
			cells[x * height + y].movingObjects.emplace( obj.get() );
		}
		else {
			cells[x * height + y].staticObjects.emplace( obj.get() );
		}
	}

	objectHolder.emplace_back( std::move(obj) );
	return objectHolder.back().get();
}

Area Grid::CalcCollidableArea(const RectF& bbox, int broadX, int broadY) const
{
	return 	{
		UINT(max(0         ,      bbox.left   / cellSize      - broadX)),
		UINT(min(width  - 1, ceil(bbox.right  / cellSize) - 1 + broadX)),
		UINT(max(0         ,      bbox.top    / cellSize      - broadY)),
		UINT(min(height - 1, ceil(bbox.bottom / cellSize) - 1 + broadY))
	};
}

void Grid::UpdateCells()
{
	// recalculate viewPortArea every frame
	viewPortArea = CalcCollidableArea( Camera::Instance().GetBBox() );

	std::unordered_set<GameObject*> shouldBeUpdatedObjects;

	for (UINT x = viewPortArea.xs; x <= viewPortArea.xe; x++)
	for (UINT y = viewPortArea.ys; y <= viewPortArea.ye; y++)
	{
		Cell& cell = cells[x * height + y];
		if (cell.movingObjects.size() == 0) continue;

		Utils::RemoveIf(cell.movingObjects, [&](auto& o)
		{
			static Camera& cam = Camera::Instance();
			const RectF oBbox = o->GetBBox();

			if (!oBbox.IsIntersect( cam.GetBBox() ))
			{
				dynamic_cast<VisibleObject*>(o)->SetState(State::Destroyed);
			}

			// objects IsNone are either Destroyed or Die and won't be moving, so no need to care updating
			// NOTE: but if game has objects flying around after died we should rewrite this
			if (oBbox.IsNone()) return false; 

			// Objects offscreen should be updated when totally left their old cell
			if (!oBbox.IsIntersect( cell.GetBBox() ))
			{
				shouldBeUpdatedObjects.emplace(o);	
				return true;
			}

			return false;
		});
	}

	for (auto& obj : shouldBeUpdatedObjects)
	{
		Area area = CalcCollidableArea( obj->GetBBox() );

		for (UINT x = area.xs; x <= area.xe; x++)
		for (UINT y = area.ys; y <= area.ye; y++)
		{
			cells[x * height + y].movingObjects.emplace( obj );
		}
	}
}

void Grid::RemoveDestroyedObjects()
{
	for (UINT x = viewPortArea.xs; x <= viewPortArea.xe; x++)
	for (UINT y = viewPortArea.ys; y <= viewPortArea.ye; y++)
	{
		Cell& cell = cells[x * height + y];
		Utils::RemoveIf(cell.movingObjects, [](auto o) { return o->GetState() == State::Destroyed;} );
	}

	Utils::RemoveIf(objectHolder, [](auto& o) { return o->GetState() == State::Destroyed; });
	RecalculateObjectsInViewPort();
}

void Grid::RecalculateObjectsInViewPort()
{
	std::unordered_set<GameObject*> result;

	for (UINT x = viewPortArea.xs; x <= viewPortArea.xe; x++)
	for (UINT y = viewPortArea.ys; y <= viewPortArea.ye; y++)
	{
		const Cell& cell = cells[x * height + y];
		result.insert(cell.staticObjects.begin(), cell.staticObjects.end());
		result.insert(cell.movingObjects.begin(), cell.movingObjects.end());
	}

	curObjectsInViewPort = { result.begin(), result.end() };
}

void Grid::RenderCells() const
{
	for (UINT x = viewPortArea.xs; x <= viewPortArea.xe; x++)
	for (UINT y = viewPortArea.ys; y <= viewPortArea.ye; y++)
	{
		const UINT  cellIndex = x * height + y   ;
		const Cell& cell      = cells[cellIndex] ;
		const RectF cellBbox  = cell.GetBBox()   ;

		DebugDraw::DrawRectOutLine( cellBbox, Colors::LightGray ); // cells outline

		DebugDraw::DrawSolidRect( { cellBbox.GetTopLeft(), 45, 37 }, Colors::LightGray ); // background for string

		const Vector2 cellDrawPosition = cellBbox.GetTopLeft() + Vector2{ 2.5f, 0.5f };

		static std::ostringstream os; os.str("");
		os << "index: " << cellIndex << "\n"
		   << "static: " << cell.staticObjects.size() << "\n"
		   << "moving: " << cell.movingObjects.size();

		DebugDraw::DrawString( os.str(), cellDrawPosition, Colors::White ); // actual string
	}
}
