#include "stdafx.h"
#include "Node.h"

Node::Node(Node * parent, Vector2D pos, Vector2D EndPos, Vector2D startPos)
{
	Parent = parent;
	Position = pos;
	
	G = abs(Position.x - startPos.x) + abs(Position.y - startPos.y);
	H = abs(Position.x - EndPos.x) + abs(Position.y - EndPos.y);
	F = G + H;
}

bool Node::operator==(const Node & rhs)
{
	return Position.x = rhs.Position.x && Position.y == rhs.Position.y;
}
