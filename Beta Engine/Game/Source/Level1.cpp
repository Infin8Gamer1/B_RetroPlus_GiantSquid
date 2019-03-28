//------------------------------------------------------------------------------
//
// File Name:	Level1.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <Space.h>
#include "Level1.h"

//Components
#include <SpriteText.h>
#include <Transform.h>
#include <Physics.h>
#include <ColliderTilemap.h>
#include <SpriteTilemap.h>
#include "MonkeyMovement.h"
#include "PacManMovement.h"
#include "Pellet.h"

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
#include <GameObject.h>
#include <Graphics.h>
#include <Parser.h>

Levels::Level1::Level1() : Level("LevelEditor")
{
	// Sound manager
	soundManager = nullptr;
	musicChannel = nullptr;
}

void Levels::Level1::Load()
{
	std::cout << GetName() << "::Load" << std::endl;

	System::GetInstance().SetWindowTitle(WindowTitle);

	Graphics::GetInstance().GetCurrentCamera().Reset();

	GetSpace()->GetObjectManager().AddArchetype(*GameObjectFactory::GetInstance().CreateObject("Pellet"));

	//Setup Sounds
	soundManager = Engine::GetInstance().GetModule<SoundManager>();
	soundManager->AddMusic("Asteroid_Field.mp3");
	soundManager->AddEffect("teleport.wav");

	soundManager->AddBank("Master Bank.strings.bank");
	soundManager->AddBank("Master Bank.bank");

	SetFileLocation("Assets/Level1.lvl");
}

void Levels::Level1::Initialize()
{
	std::cout << GetName() << "::Initialize" << std::endl;

	LoadLevel();

	colliderTilemap = GetSpace()->GetObjectManager().GetObjectByName("TileMap")->GetComponent<ColliderTilemap>();


/*#ifdef _DEBUG
	std::cout << "Warning Not Adding Dots because they are slow in debug" << std::endl;
#else*/
	for (unsigned i = 0; i < colliderTilemap->GetTilemap()->GetWidth(); i++)
	{
		for (unsigned j = 0; j < colliderTilemap->GetTilemap()->GetHeight(); j++)
		{
			int value = colliderTilemap->GetTilemap()->GetCellValue(i, j);

			Vector2D position = colliderTilemap->ConvertTileMapCordsToWorldCords(Vector2D(i, j));

			if (value == 0 && !IsObjectAt(position))
			{
				GameObject* obj = new GameObject(*GetSpace()->GetObjectManager().GetArchetypeByName("Pellet"));
				obj->GetComponent<Transform>()->SetTranslation(position);

				GetSpace()->GetObjectManager().AddObject(*obj);
			}
		}
	}
//#endif // RELEASE

	
}

void Levels::Level1::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
}

void Levels::Level1::Shutdown()
{
	std::cout << GetName() << "::Shutdown" << std::endl;

	//SaveLevel();

	musicChannel->stop();
	musicChannel = nullptr;
}

void Levels::Level1::Unload()
{
	std::cout << GetName() << "::Unload" << std::endl;

	soundManager->Shutdown();
}

bool Levels::Level1::IsObjectAt(Vector2D pos)
{
	std::vector<GameObject*> objs = GetSpace()->GetObjectManager().GetGameObjectActiveList();

	for (unsigned i = 0; i < objs.size(); i++)
	{
		if (AlmostEqual(objs[i]->GetComponent<Transform>()->GetTranslation(), pos)) 
			return true;
	}

	return false;
}
