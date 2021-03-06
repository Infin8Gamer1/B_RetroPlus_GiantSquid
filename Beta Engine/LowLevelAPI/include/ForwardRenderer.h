//------------------------------------------------------------------------------
//
// File Name:	ForwardRenderer.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Renderer.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class ForwardRenderer : public Renderer
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	ForwardRenderer(unsigned width, unsigned height);
	void FrameStart();
	void FrameEnd(bool syncOperations = false);
};

//------------------------------------------------------------------------------
