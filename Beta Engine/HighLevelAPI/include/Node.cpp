#include "stdafx.h"
#include "Node.h"

Node::Node(Node * parent, Vector2D pos, Vector2D EndPos, Vector2D startPos)
{
	Parent = parent;
	Position = pos;
	
	G = 0.0f;
	H = 0.0f;
	F = 0.0f;
}

bool Node::operator==(const Node & rhs)
{
	return Position.x = rhs.Position.x && Position.y == rhs.Position.y;
}
