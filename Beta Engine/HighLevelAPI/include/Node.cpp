#include "stdafx.h"
#include "Node.h"

Node::Node(Node * parent, Vector2D pos)
{
	Parent = parent;
	Position = pos;
	
	G = 0.0f;
	H = 0.0f;
	F = 0.0f;
}

bool Node::operator==(const Node & rhs) const
{
	return Position.x == rhs.Position.x && Position.y == rhs.Position.y;
}

bool Node::operator==(const Vector2D & rhs) const
{
	return Position.x == rhs.x && Position.y == rhs.y;
}
