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
	target = Vector2D(13,15);
	mode = Mode::Stationary;

	// Components
	transform = nullptr;
	physics = nullptr;
	colliderTilemap = nullptr;
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
}

void TileMapNavigation::Update(float dt)
{
	std::vector<Vector2D> path = CalculatePath();

	for (size_t i = 1; i < path.size(); i++)
	{
		DebugDraw::GetInstance().AddLineToStrip(colliderTilemap->ConvertTileMapCordsToWorldCords(path[i - 1]), colliderTilemap->ConvertTileMapCordsToWorldCords(path[i]));
	}
	DebugDraw::GetInstance().EndLineStrip(Graphics::GetInstance().GetCurrentCamera());
	

	switch (mode)
	{
	case Stationary:
		break;
	case Move:
		break;
	default:
		break;
	}
}

void TileMapNavigation::SetTarget(Vector2D _target)
{
	target = _target;
}

void TileMapNavigation::SetMode(Mode _mode)
{
	mode = _mode;
}

std::vector<Vector2D> TileMapNavigation::CalculatePath()
{
	//Create Start and end nodes
	Node* startNode = new Node(nullptr, colliderTilemap->ConvertWorldCordsToTileMapCords(transform->GetTranslation()));
	Node endNode = Node(nullptr, target);
	
	//init both open and closed list
	std::vector<Node*> openList;
	std::vector<Node*> closedList;

	//add start node to open list
	openList.push_back(startNode);

	int loopCount = 0;
	while (!openList.empty() || loopCount > 500)
	{
		loopCount++;

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
		if (*CurrentNode == endNode)
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

			return outputPath;

			//break;
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

		for (size_t i = 0; i < children.size(); i++)
		{
			bool flag = false;
			for each (Node* x in closedList)
			{
				if (*x == *children[i])
				{
					flag = true;

					delete children[i];
					children[i] = nullptr;
					break;
				}
			}

			if (flag)
			{
				continue;
			}

			//Calculate G, H, and F
			float G = CurrentNode->G + 1;
			float H = abs(children[i]->Position.x - endNode.Position.x) + abs(children[i]->Position.y - endNode.Position.y);
			float F = G + H;

			children[i]->G = G;
			children[i]->H = H;
			children[i]->F = F;

			bool flag2 = false;
			for each (Node* x in openList)
			{
				if (*x == *children[i])
				{
					if (x->G < children[i]->G)
					{
						flag2 = true;
						delete children[i];
						children[i] = nullptr;
						break;
					}
				}
			}

			if (flag2)
			{
				continue;
			}

			//add the child to the open list
			openList.push_back(children[i]);
		}
		
	}

	DeleteVector(openList);
	DeleteVector(closedList);

	std::vector<Vector2D> emptyout;
	return emptyout;
}
