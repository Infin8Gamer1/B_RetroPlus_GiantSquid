//------------------------------------------------------------------------------
//
// File Name:	Node.h
// Author(s):	Jacob Holyfield (J.Holyfield)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <Vector2D.h> // Vector2D


//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class Node
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	Node(Node* parent, Vector2D pos);

	bool operator==(const Node& rhs);

	Node* Parent;

	Vector2D Position;

	float G;
	float H;
	float F;

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	
};

//------------------------------------------------------------------------------
