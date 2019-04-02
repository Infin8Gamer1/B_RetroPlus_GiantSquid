#include "stdafx.h"
#include "TileMapNavigation.h"
#include "Node.h"
#include "Transform.h"
#include "Physics.h"
#include "ColliderTilemap.h"
#include "Tilemap.h"
#include "GameObject.h"
#include "Space.h"
#include <functional>
#include <DebugDraw.h>
#include <Graphics.h>

TileMapNavigation::TileMapNavigation() : Component("TileMapNavigation")
{
	target = Vector2D(0,0);
	mode = Mode::Move;

	fraction = 0;
	pointIndex = 0;
	startPointIndex = 0;
	endPointIndex = 1;

	// Components
	transform = nullptr;
	physics = nullptr;
	colliderTilemap = nullptr;

	calculatePathFlag = false;
}

Component * TileMapNavigation::Clone() const
{
	return new TileMapNavigation(*this);
}

void TileMapNavigation::Initialize()
{
	transform = GetOwner()->GetComponent<Transform>();
	physics = GetOwner()->GetComponent<Physics>();
	colliderTilemap = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("TileMap")->GetComponent<ColliderTilemap>();
	//SetTarget(Vector2D(10, 15));
}

void TileMapNavigation::Update(float dt)
{
	//if the path is not empty then follow it
	if (!path.empty())
	{
#ifdef _DEBUG
		DebugDrawPath(path);
#endif // _DEBUG


		switch (mode)
		{
		case Stationary:
			break;
		case Move:
			if (fraction < 1 && pointIndex < path.size() - 1)
			{
				fraction += dt * moveSpeed;
				transform->SetTranslation(Lerp(transform->GetTranslation(), colliderTilemap->ConvertTileMapCordsToWorldCords(path[pointIndex]), fraction));
			}
			else
			{
				fraction = 0;

				if (pointIndex >= path.size() - 1)
				{
					mode = Mode::Stationary;
					pointIndex = 0;
				}
				else
				{
					pointIndex += 1;
				}
			}
			break;
		default:
			break;
		}
	}

	//if we need to calculate the path then do that
	if (calculatePathFlag && !openList.empty())
	{
		//set the current node to the first node in the open list
		Node* CurrentNode = openList[0];
		size_t CurrentNodeIndex = 0;

		//find the node with the lowest f value and set it to current node
		for (size_t i = 1; i < openList.size(); i++)
		{
			if (CurrentNode->F > openList[i]->F)
			{
				CurrentNode = openList[i];
				CurrentNodeIndex = i;
			}
		}

		//remove current node from open list
		openList.erase(openList.begin() + CurrentNodeIndex);

		//add current node to the closed list
		closedList.push_back(CurrentNode);

		//did we found the goal
		if (*CurrentNode == target)
		{
			//Congratz! You've found the end! Backtrack to get path
			std::vector<Vector2D> outputPath;
			Node* nextNode = CurrentNode;

			while (nextNode != nullptr)
			{
				outputPath.insert(outputPath.begin(), nextNode->Position);
				nextNode = nextNode->Parent;
			}

			DeleteVector(openList);
			DeleteVector(closedList);

			path = outputPath;
			calculatePathFlag = false;
			mode = Mode::Move;
			return;
		}

		//generate children of current node
		std::vector<Node*> children;

		for (size_t i = 0; i < 4; i++)
		{
			Vector2D pos;

			switch (i)
			{
			case 0:
				pos = CurrentNode->Position + Vector2D(0, 1);
				break;
			case 1:
				pos = CurrentNode->Position + Vector2D(0, -1);
				break;
			case 2:
				pos = CurrentNode->Position + Vector2D(1, 0);
				break;
			case 3:
				pos = CurrentNode->Position + Vector2D(-1, 0);
				break;
			default:
				break;
			}

			if (colliderTilemap->GetTilemap()->GetCellValue(pos.x, pos.y) == 0)
			{
				Node* node = new Node(CurrentNode, pos);

				children.push_back(node);
			}
		}

		//loop through each child and calculate g,h and f values
		//then add the node into the open set if it isn't already in the open set or closed set
		std::vector<Node*>::iterator ptr;
		for (ptr = children.begin(); ptr < children.end(); ptr++)
		{
			Node* child = *ptr;

			bool flag = false;
			std::vector<Node*>::iterator x;
			for (x = closedList.begin(); x < closedList.end(); x++)
			{
				if (**x == *child)
				{
					flag = true;

					delete child;
					*ptr = nullptr;
					break;
				}
			}

			if (flag)
			{
				continue;
			}

			//Calculate G, H, and F
			float G = CurrentNode->G + 1;
			float H = abs(child->Position.x - target.x) + abs(child->Position.y - target.y);
			float F = G + H;

			child->G = G;
			child->H = H;
			child->F = F;

			bool flag2 = false;
			std::vector<Node*>::iterator y;
			for (y = openList.begin(); y < openList.end(); y++)
			{
				Node* node = *y;
				if (*node == *child)
				{
					if (node->G < child->G)
					{
						flag2 = true;
						delete child;
						*ptr = nullptr;
						break;
					}
				}
			}

			if (flag2)
			{
				continue;
			}

			//add the child to the open list
			openList.push_back(child);
		}

		return;
	}


}

void TileMapNavigation::SetTarget(Vector2D _target)
{
	target = _target;

	if (colliderTilemap->GetTilemap()->GetCellValue(_target.x, _target.y) != 0)
	{
		std::cout << "WARNING: target " << target << " is not a valid target!";
	}
	
	CalculatePath();
}

void TileMapNavigation::SetMode(Mode _mode)
{
	mode = _mode;
}

void TileMapNavigation::CalculatePath()
{
	//reset Open and Closed Lists
	DeleteVector(openList);
	DeleteVector(closedList);

	//Create Start and end nodes
	Node* startNode = new Node(nullptr, colliderTilemap->ConvertWorldCordsToTileMapCords(transform->GetTranslation()));

	//add start node to open list
	openList.push_back(startNode);

	//set calculate Path flag
	calculatePathFlag = true;
}

void TileMapNavigation::DebugDrawPath(std::vector<Vector2D> path)
{
	for (size_t i = 1; i < path.size(); i++)
	{
		DebugDraw::GetInstance().AddLineToStrip(colliderTilemap->ConvertTileMapCordsToWorldCords(path[i - 1]), colliderTilemap->ConvertTileMapCordsToWorldCords(path[i]));
	}

	DebugDraw::GetInstance().EndLineStrip(Graphics::GetInstance().GetCurrentCamera());
}

Vector2D TileMapNavigation::Lerp(Vector2D v0, Vector2D v1, float t)
{
	return Vector2D(((1 - t) * v0.x + t * v1.x), ((1 - t) * v0.y + t * v1.y));
}
