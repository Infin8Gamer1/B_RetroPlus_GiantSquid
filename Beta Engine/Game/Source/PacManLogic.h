//------------------------------------------------------------------------------
//
// File Name:	PacManLogic.h
// Author(s):	Sage Dupuy (sage.dupuy)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2018 DigiPen (USA) Corporation.
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
class Animation;
class Sprite;
struct MapCollision;
class ColliderTilemap;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------


class PacManLogic : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	PacManLogic();

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

	// Collision handler for monkey.
	// Params:
	//   object = The monkey.
	//   other  = The object the monkey is colliding with.
	friend void PacManCollisionHandler(GameObject& object, GameObject& other);

private:

	unsigned highScore;

	unsigned score;

	int pelletScore;

	int powerPelletScore;

};