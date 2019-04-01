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


TileMapNavigation::TileMapNavigation() : Component("TileMapNavigation")
{
	target = Vector2D(18,3);
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
	std::vector<Node*> path = CalculatePath();

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

std::vector<Node*> TileMapNavigation::CalculatePath()
{
	//Create Start and end nodes
	Node startNode = Node(nullptr, colliderTilemap->ConvertWorldCordsToTileMapCords(transform->GetTranslation()));
	Node endNode = Node(nullptr, target);
	
	//init both open and closed list
	std::vector<Node*> openList;
	std::vector<Node*> closedList;

	//add start node to open list
	openList.push_back(&startNode);

	while (!openList.empty())
	{
		//set the current node to the first node in the open list
		Node * CurrentNode = openList[0];
		size_t CurrentNodeIndex = 0;

		//find the node with the lowest f and set it to current node
		for (size_t i = 1; i < openList.size(); i++)
		{
			if (openList[i]->F < CurrentNode->F)
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
			std::vector<Node*> outputPath;
			Node* nextNode = CurrentNode;

			while (nextNode != nullptr)
			{
				outputPath.insert(outputPath.begin(), nextNode);
				nextNode = nextNode->Parent;
			}

			return outputPath;

			//break;
		}
		
		//generate children of current node
		Vector2D pos = CurrentNode->Position + Vector2D(0, 1);
		if (colliderTilemap->GetTilemap()->GetCellValue(pos.x, pos.y) == 0)
		{
			Node upNode = Node(CurrentNode, pos);

			CurrentNode->children.push_back(&upNode);
		}

		pos = CurrentNode->Position + Vector2D(0, -1);
		if (colliderTilemap->GetTilemap()->GetCellValue(pos.x, pos.y) == 0)
		{
			Node downNode = Node(CurrentNode, pos);

			CurrentNode->children.push_back(&downNode);
		}

		
		pos = CurrentNode->Position + Vector2D(1, 0);
		if (colliderTilemap->GetTilemap()->GetCellValue(pos.x, pos.y) == 0)
		{
			Node rightNode = Node(CurrentNode, pos);

			CurrentNode->children.push_back(&rightNode);
		}

		pos = CurrentNode->Position + Vector2D(-1, 0);
		if (colliderTilemap->GetTilemap()->GetCellValue(pos.x, pos.y) == 0)
		{
			Node leftNode = Node(CurrentNode, pos);

			CurrentNode->children.push_back(&leftNode);
		}

		//colliderTilemap->GetTilemap()->GetCellValue();

		for (size_t i = 0; i < CurrentNode->children.size(); i++)
		{
			for each (Node* x in closedList)
			{
				if (*x == *CurrentNode->children[i])
				{
					continue;
				}
			}

			//Calculate G, H, and F
			;alksdjfijwe
			CurrentNode->children[i]->G = CurrentNode->G + 1;
			CurrentNode->children[i]->H = abs(CurrentNode->children[i]->Position.x - endNode.Position.x) + abs(CurrentNode->children[i]->Position.y - endNode.Position.y);
			CurrentNode->children[i]->F = CurrentNode->children[i]->G + CurrentNode->children[i]->H;

			for each (Node* x in openList)
			{
				if (*x == *CurrentNode->children[i])
				{
					if (x->G < CurrentNode->children[i]->G)
					{
						continue;
					}
				}
				
			}

			//add the child to the open list
			openList.push_back(CurrentNode->children[i]);
		}
		
	}


	std::vector<Node*> emptyout;
	return emptyout;
}
