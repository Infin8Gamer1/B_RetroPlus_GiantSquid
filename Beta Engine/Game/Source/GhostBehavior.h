//------------------------------------------------------------------------------
//
// File Name:	PacManMovement.h
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

#include <Component.h> // base class

#include <Vector2D.h> // Vector2D


//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class TileMapNavigation;
class ColliderTilemap;
class Sprite;
class GhostBehaviorBlue;
class GhostBehaviorOrange;
class GhostBehaviorRed;
class GhostBehaviorPink;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

enum GhostState
{
	Dead,
	Chase,
	Scatter,
	Frightened
};

class GhostBehavior : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	GhostBehavior();

	// Initialize this component (happens at object creation).
	void Initialize() override;

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void Update(float dt) override;

	void SetState(GhostState state);

	GhostState GetState();

	void ResetPos();

	friend GhostBehaviorBlue;
	friend GhostBehaviorOrange;
	friend GhostBehaviorRed;
	friend GhostBehaviorPink;

	bool isFrightenedLow;

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	float timer;
	int timeOutCounter;

	GhostState state;
	GhostState previousState;

	Vector2D startPos;

	// Components
	Transform* PacManTransform;
	Transform* transform;
	TileMapNavigation* navigation;
	Sprite* sprite;
	ColliderTilemap* colliderTilemap;

};

//------------------------------------------------------------------------------
