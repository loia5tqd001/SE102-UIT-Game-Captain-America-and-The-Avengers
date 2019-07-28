#include "pch.h"
#include "ObjectFactory.h"

static Camera& cam = Camera::Instance();

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

	std::shared_ptr<GameObject> object;
	for (const auto& jsonObj : jsonObjects)
	{
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
				AmbushTrigger::SetInstance(dynamic_cast<AmbushTrigger*>( object.get() ));
				break;

			case ClassId::Bunker:
				object = ObjectFactory::Create<Bunker>(jsonObj, this);
				break;

			default:
				ThrowMyException("Can't find class id:", classId);
		}

		SpawnObject(object, IsMoving::Static);
	}
}

GameObject* Grid::SpawnObject(std::shared_ptr<GameObject> obj, IsMoving isMoving)
{
	Area area = GetCells( obj->GetBBox() );

	for (UINT x = area.xs; x <= area.xe; x++)
	for (UINT y = area.ys; y <= area.ye; y++)
	{
		if (isMoving == IsMoving::Moving) {
			cells[x * height + y].movingObjects.emplace( obj );
		}
		else {
			cells[x * height + y].staticObjects.emplace( obj );
		}
	}

	return obj.get();
}

Area Grid::GetCells(const RectF& bbox, int broadX, int broadY) const
{
	if (bbox.IsNone()) return {};
	return 	{
		UINT(max(0         ,      bbox.left   / cellSize      - broadX)),
		UINT(min(width  - 1, ceil(bbox.right  / cellSize) - 1 + broadX)),
		UINT(max(0         ,      bbox.top    / cellSize      - broadY)),
		UINT(min(height - 1, ceil(bbox.bottom / cellSize) - 1 + broadY))
	};
}

void Grid::UpdateCells()
{
	// calculate which cells are cells in viewport
	viewPortArea = GetCells( cam.GetBBox() );
	std::unordered_set<GameObject*> objsInViewPort; // use unordered_set to make them unique
	std::unordered_set<std::shared_ptr<GameObject>> objsOutOfCell;
	
	// * Moving objects(Enemy, Item and Bullet), they will be Destroyed when out of Camera.
	// = Update Cells:
	// 1. SetState = Destroyed for moving objects go out of Camera.
	// 2. Remove objects have state of destroyed from Cells 
	// 3. Update cell for objects go out of cell
	// 4. Recalculate curObjectsInViewPort for the outside to handle collision.

	for (UINT x = viewPortArea.xs; x <= viewPortArea.xe; x++)
	for (UINT y = viewPortArea.ys; y <= viewPortArea.ye; y++)
	{
		Cell& cell = cells[x * height + y];
		
		Utils::RemoveIf(cell.movingObjects, [&](auto& mo) { // remove (moving) objects from cell if:
			const auto moBBox = mo->GetBBox();

			if (mo->GetState() == State::Destroyed)
			{
				return true; // remove from cell
			}
			if (moBBox.IsNone()) // objects can't collide - beforeExplode/Explode - not remove
			{
				objsInViewPort.emplace(mo.get()); // we still want to draw them (their image)
				return false; 
			}
			if (!moBBox.IsIntersect(cam.GetBBox()))
			{
				mo->OnOutOfViewPort();
			}

			objsInViewPort.emplace(mo.get()); // to go to this line mean objects should be to handle collision.

			if (!moBBox.IsIntersect(cell.GetBBox())) // if object goes out of cell
			{
				objsOutOfCell.emplace(mo);
				return true; // remove from cell to readd to right cell later
			}
			return false;
		});
		// We insert moving objects above, now static objects below:
		std::transform(cell.staticObjects.begin(), cell.staticObjects.end(), std::inserter(objsInViewPort, objsInViewPort.end()),
					   [](auto& sharedPtr){ return sharedPtr.get(); });
	}

	// curObjectsInViewPort is vector instead of unordered_set to inline the getter GetObjectsInViewPort()
	curObjectsInViewPort = { objsInViewPort.begin(), objsInViewPort.end() };

	for (auto& obj : objsOutOfCell)
	{
		Area newCells = GetCells( obj->GetBBox() );

		for (UINT x = newCells.xs; x <= newCells.xe; x++)
		for (UINT y = newCells.ys; y <= newCells.ye; y++)
		{
			cells[x * height + y].movingObjects.emplace( obj );
		}
	}
}

void Grid::RenderCells() const
{
	for (UINT x = viewPortArea.xs; x <= viewPortArea.xe; x++)
	for (UINT y = viewPortArea.ys; y <= viewPortArea.ye; y++)
	{
		const UINT  cellIndex = x * height + y   ;
		const Cell& cell      = cells[cellIndex] ;
		const RectF cellBbox  = cell.GetBBox()   ;

		DebugDraw::DrawRectOutLine( cellBbox, Colors::DimGray ); // cells outline

		DebugDraw::DrawSolidRect( { cellBbox.GetTopLeft(), 45, 37 }, Colors::DimGray ); // background for string

		const Vector2 cellDrawPosition = cellBbox.GetTopLeft() + Vector2{ 2.5f, 0.5f };

		static std::ostringstream os; os.str("");
		os << "index: " << cellIndex << "\n"
		   << "static: " << cell.staticObjects.size() << "\n"
		   << "moving: " << cell.movingObjects.size();

		DebugDraw::DrawString( os.str(), cellDrawPosition, Colors::White ); // actual string
	}
}
