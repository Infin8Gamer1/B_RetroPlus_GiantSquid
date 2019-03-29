//------------------------------------------------------------------------------
//
// File Name:	LevelEditorLevel.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <Space.h>
#include "LevelEditorLevel.h"
#include "LevelEditorUI.h"

//Components
#include <SpriteText.h>
#include <Transform.h>
#include <Physics.h>
#include <ColliderTilemap.h>
#include <SpriteTilemap.h>

//Resources
#include <Mesh.h>
#include <Color.h>
#include <Tilemap.h>
#include <SpriteSource.h>

//Systems
#include <GameObjectFactory.h>
#include <ResourceManager.h>
#include <System.h>
#include <Input.h>
#include <MeshHelper.h>
#include <Engine.h>
#include <SoundManager.h>
#include <Random.h>
#include <SpaceManager.h>
#include <Graphics.h>
#include <Parser.h>
#include <DebugDraw.h>

Levels::LevelEditorLevel::LevelEditorLevel() : Level("LevelEditor")
{
	// Sound manager
	soundManager = nullptr;
	musicChannel = nullptr;
}

void Levels::LevelEditorLevel::Load()
{
	std::cout << GetName() << "::Load" << std::endl;

	System::GetInstance().SetWindowTitle(WindowTitle);

	Graphics::GetInstance().GetCurrentCamera().Reset();

	////Register Custom Components

	//Setup Sounds
	soundManager = Engine::GetInstance().GetModule<SoundManager>();
	soundManager->AddMusic("Asteroid_Field.mp3");
	soundManager->AddEffect("teleport.wav");

	soundManager->AddBank("Master Bank.strings.bank");
	soundManager->AddBank("Master Bank.bank");

	SetFileLocation("Assets/Level1.lvl");
}

void Levels::LevelEditorLevel::Initialize()
{
	std::cout << GetName() << "::Initialize" << std::endl;

	LoadLevel();

	GetSpace()->GetObjectManager().OpenTWBarForAllObjects();
	
}

void Levels::LevelEditorLevel::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	GameObject* tilemap = GetSpace()->GetObjectManager().GetObjectByName("TileMap");

	float wid = tilemap->GetComponent<Transform>()->GetScale().x;

	float hei = tilemap->GetComponent<Transform>()->GetScale().y;

	for (int x = -gridSize; x < gridSize; x++)
	{
		float s = x*wid+wid/2;

		DebugDraw::GetInstance().AddLineToStrip(Vector2D(s, hei * gridSize), Vector2D(s, hei*gridSize*-1), Colors::Grey);
	}

	for (int y = -gridSize; y < gridSize; y++)
	{
		float s =y*hei + hei / 2;

		DebugDraw::GetInstance().AddLineToStrip(Vector2D(wid * gridSize, s), Vector2D(wid * gridSize*-1, s), Colors::Grey);
	}

	DebugDraw::GetInstance().EndLineStrip(Graphics::GetInstance().GetCurrentCamera());
}

void Levels::LevelEditorLevel::Shutdown()
{
	std::cout << GetName() << "::Shutdown" << std::endl;

	//Save();

	musicChannel->stop();
	musicChannel = nullptr;
}

void Levels::LevelEditorLevel::Unload()
{
	std::cout << GetName() << "::Unload" << std::endl;

	soundManager->Shutdown();
}