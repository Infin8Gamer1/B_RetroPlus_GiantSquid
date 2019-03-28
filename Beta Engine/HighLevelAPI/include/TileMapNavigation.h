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

	void CalculatePath();

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Movement properties
	const float moveSpeed = 150.0f;

	Vector2D target;
	Mode mode;

	// Components
	Transform* transform;
	Physics* physics;
	ColliderTilemap* colliderTilemap;

};

//------------------------------------------------------------------------------
