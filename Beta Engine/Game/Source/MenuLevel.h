//------------------------------------------------------------------------------
//
// File Name:	LevelEditorLevel.h
// Author(s):	Nathan Miller
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

#include <Level.h>

class Mesh;
class SpriteSource;
class Texture;
class SoundManager;
class ColliderTilemap;

namespace Levels
{
	class MenuLevel : public Level
	{
	public:
		MenuLevel();

		// Load archetypes, meshes
		void Load();

		// Initialize the memory associated with the Omega game state.
		void Initialize();

		// Update the Omega game state.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt);

		//Shutdown level
		void Shutdown();

		// Unload textures, sprite sources, meshes
		void Unload();
	};
}