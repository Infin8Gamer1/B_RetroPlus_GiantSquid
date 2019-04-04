//------------------------------------------------------------------------------
//
// File Name:	GhostBehaviorOrange.h
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

#include "GhostBehavior.h" // base class

#include <Vector2D.h> // Vector2D


//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class GhostBehaviorOrange : public GhostBehavior
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	GhostBehaviorOrange();

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

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	Vector2D GenerateTarget();

	Vector2D corner;

};

//------------------------------------------------------------------------------
