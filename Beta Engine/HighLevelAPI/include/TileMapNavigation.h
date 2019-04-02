//------------------------------------------------------------------------------
//
// File Name:	TileMapNavigation.h
// Author(s):	Jacob Holyfield (J.Holyfield)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <Component.h> // base class

#include <Vector2D.h> // Vector2D


//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Physics;
class ColliderTilemap;
class Node;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

enum Mode
{
	Stationary,
	Move
};

class TileMapNavigation : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	TileMapNavigation();

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* Clone() const override;

	// Initialize this component (happens at object creation).
	void Initialize() override;

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void Update(float dt) override;

	void SetTarget(Vector2D target);

	void SetMode(Mode mode);

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	//sets up class to start calculating path
	void CalculatePath();

	void DebugDrawPath(std::vector<Vector2D> path);

	Vector2D Lerp(Vector2D v0, Vector2D v1, float t);

	template<class T>
	void DeleteVector(std::vector<T*>& vector)
	{
		class std::vector<T*>::iterator i;

		for (i = vector.begin(); i != vector.end(); ++i)
		{
			delete (*i);
			*i = nullptr;
		}

		vector.clear();
		vector.shrink_to_fit();
	}

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Movement properties
	const float moveSpeed = 1.5f;
	float fraction;

	int startPointIndex;
	int endPointIndex;

	Vector2D target;
	std::vector<Vector2D> path;
	Mode mode;

	// Components
	Transform* transform;
	Physics* physics;
	ColliderTilemap* colliderTilemap;


	//pathfinding vars
	std::vector<Node*> openList;
	std::vector<Node*> closedList;

	bool calculatePathFlag;

};

//------------------------------------------------------------------------------
