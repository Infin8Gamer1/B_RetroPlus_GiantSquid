#include "stdafx.h"
#include "TileMapNavigation.h"
#include "Node.h"
#include "Transform.h"
#include "ColliderTilemap.h"
#include "Tilemap.h"


TileMapNavigation::TileMapNavigation() : Component("TileMapNavigation")
{
}

Component * TileMapNavigation::Clone() const
{
	return new TileMapNavigation(*this);
}

void TileMapNavigation::Initialize()
{
}

void TileMapNavigation::Update(float dt)
{
}

void TileMapNavigation::SetTarget(Vector2D target)
{
}

void TileMapNavigation::SetMode(Mode mode)
{
}

void TileMapNavigation::CalculatePath()
{
	//Create Start and end nodes
	Node startNode = Node(nullptr, colliderTilemap->ConvertWorldCordsToTileMapCords(transform->GetTranslation()));
	startNode.F = 0.0f;
	Node endNode = Node(nullptr, target);
	
	//init both open and closed list
	std::vector<Node> openList;
	std::vector<Node> closedList;

	//add start node to open list
	openList.push_back(startNode);

	while (!openList.empty())
	{
		//set the current node to the first node in the open list
		Node CurrentNode = openList[0];
		size_t CurrentNodeIndex = 0;

		//find the node with the lowest f and set it to current node
		for (size_t i = 1; i < openList.size(); i++)
		{
			if (openList[i].F < CurrentNode.F)
			{
				CurrentNode = openList[i];
				CurrentNodeIndex = i;
			}
		}

		//remove current node from open list
		openList.erase(openList.begin() + CurrentNodeIndex);

		//add current node to the closed list
		closedList.push_back(CurrentNode);

		//we found the goal
		if (CurrentNode == endNode)
		{
			//Congratz!You've found the end! Backtrack to get path
		}
		
		//generate children of current node
		//Node upNode = Node(*CurrentNode, CurrentNode.Position + Vector2D(0, 1), endNode.Position, startNode.Position);
		//colliderTilemap->GetTilemap()->GetCellValue();

		
	}

	/* # Create start and end node
		start_node = Node(None, start)
		start_node.g = start_node.h = start_node.f = 0
		end_node = Node(None, end)
		end_node.g = end_node.h = end_node.f = 0

		# Initialize both open and closed list
		open_list = []
		closed_list = []

		# Add the start node
		open_list.append(start_node)

		# Loop until you find the end
		while len(open_list) > 0:

	# Get the current node
		current_node = open_list[0]
		current_index = 0
		for index, item in enumerate(open_list) :
			if item.f < current_node.f :
				current_node = item
				current_index = index

				# Pop current off open list, add to closed list
				open_list.pop(current_index)
				closed_list.append(current_node)

				# Found the goal
				if current_node == end_node:
	path = []
		current = current_node
		while current is not None :
			path.append(current.position)
			current = current.parent
			return path[:: - 1] # Return reversed path

			# Generate children
			children = []
			for new_position in[(0, -1), (0, 1), (-1, 0), (1, 0), (-1, -1), (-1, 1), (1, -1), (1, 1)]: # Adjacent squares

				# Get node position
				node_position = (current_node.position[0] + new_position[0], current_node.position[1] + new_position[1])

				# Make sure within range
				if node_position[0] > (len(maze) - 1) or node_position[0] < 0 or node_position[1] > (len(maze[len(maze) - 1]) - 1) or node_position[1] < 0:
	continue

		# Make sure walkable terrain
		if maze[node_position[0]][node_position[1]] != 0:
	continue

		# Create new node
		new_node = Node(current_node, node_position)

		# Append
		children.append(new_node)

		# Loop through children
		for child in children :

	# Child is on the closed list
		for closed_child in closed_list :
	if child == closed_child :
		continue

		# Create the f, g, and h values
		child.g = current_node.g + 1
		child.h = ((child.position[0] - end_node.position[0]) ** 2) + ((child.position[1] - end_node.position[1]) ** 2)
		child.f = child.g + child.h

		# Child is already in the open list
		for open_node in open_list :
	if child == open_node and child.g > open_node.g:
	continue

		# Add the child to the open list
		open_list.append(child)

	*/
}
