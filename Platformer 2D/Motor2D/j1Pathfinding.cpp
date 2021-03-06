#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.create("pathfinding");
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.Clear();
	RELEASE_ARRAY(map);
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const p2DynArray<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::Find(const iPoint& point) const
{
	p2List_item<PathNode>* item = list.start;
	while (item)
	{
		if (item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::GetNodeLowestScore() const
{
	p2List_item<PathNode>* ret = NULL;
	int min = 65535;

	p2List_item<PathNode>* item = list.end;
	while (item)
	{
		if (item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) const
{
	iPoint cell;
	uint before = list_to_fill.list.count();

	// north
	cell.create(pos.x, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	return list_to_fill.list.count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------

int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	//Origin or destination are not walkable, return -1
	if (IsWalkable(origin) == false || IsWalkable(destination) == false) { return -1; }

	//Create 3 lists, adds origin to open list. Iterate while we have tiles
	//in open list
	PathList open, close, neighbors;
	p2List_item<PathNode>* node;

	PathNode origin_node = { NULL, NULL, origin, nullptr };
	open.list.add(origin_node);

	while (open.list.count() > 0)
	{
		//Moves the  lowestScoreCell from open to close list
		node = open.GetNodeLowestScore();
		p2List_item<PathNode>* lowestScoreCell = close.list.add(node->data);
		open.list.del(node);

	
	// Backtrack to create the final path
		//When we add the destination of the pathfinding, we are done.
		if (lowestScoreCell->data.pos == destination)
		{
			last_path.Clear();
			
			const PathNode* breadcrumbs = &node->data;

			//Go back through breadcrumbs until it reaches the origin aka. backtracking
			while (breadcrumbs->pos != origin)
			{
				last_path.PushBack(breadcrumbs->pos);
				breadcrumbs = breadcrumbs->parent;
			}
			// Use the Pathnode::parent and Flip() the path when you are finish
			//Get the number of tiles from destination to origin

			last_path.Flip();
			return last_path.Count();
		}

		//Fills a list of adjacent nodes
		neighbors.list.clear(); 
		lowestScoreCell->data.FindWalkableAdjacents(neighbors);

		//Iterates adjacent nodes
		p2List_item<PathNode>* openNeightbors = neighbors.list.start;
		for (; openNeightbors; openNeightbors = openNeightbors->next)
		{
			
			//Ignore nodes in the closed list
			if (close.Find(openNeightbors->data.pos) == NULL)
			{
				p2List_item<PathNode>* aux = open.Find(openNeightbors->data.pos);

				// If it is NOT found, calculate its F and add it to the open list
				if (aux == NULL) 
				{
					openNeightbors->data.CalculateF(destination);
					open.list.add(openNeightbors->data);
				}
				// If it is already in the open list, check if it is a better path (compare G)
				else if (aux->data.g > openNeightbors->data.g)
				{
					// If it is a better path, Update the parent
					aux->data.parent = openNeightbors->data.parent;
					aux->data.CalculateF(destination);
				}
			}
		}
	}
}

