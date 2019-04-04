//------------------------------------------------------------------------------
//
// File Name:	Main.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

// BetaFramework Engine
#include <Engine.h>
#include <Graphics.h>

// Engine modules
#include "Space.h"
#include <SoundManager.h>
#include <SpaceManager.h>
#include <GameObject.h>
#include <GameObjectFactory.h>
#include "PacManMovement.h"
#include "PacManLogic.h"
#include <TileMapNavigation.h>
#include "GhostBehaviorBlue.h"

// Initial game state
#include "Level1.h"
#include "Level2.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Main function
int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prevInstance, _In_ LPSTR command_line, _In_ int show)
{
	// Enable memory leak checking
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	// Unused parameters
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(command_line);
	UNREFERENCED_PARAMETER(show);
	UNREFERENCED_PARAMETER(instance);

	// Add Sound Manager
	Engine::GetInstance().AddModule(new SoundManager());

	////Register Custom Components
	GameObjectFactory::GetInstance().RegisterComponent<PacManMovement>();
	GameObjectFactory::GetInstance().RegisterComponent<PacManLogic>();
	GameObjectFactory::GetInstance().RegisterComponent<GhostBehaviorBlue>();
	GameObjectFactory::GetInstance().RegisterComponent<TileMapNavigation>();

	// Create a new space called "Level"
	Space* space = new Space("Level", false);
	// Set initial level to the second level.
	space->SetLevel(new Levels::Level1());

	SpaceManager* spaceManager = new SpaceManager();
	spaceManager->AddSpace(*space);

	Engine::GetInstance().AddModule(spaceManager);

	// Game engine goes!
	int Height = 720;
	int Width = 1280;
	Engine::GetInstance().Start(Width, Height, 200);

	return 0;
}
