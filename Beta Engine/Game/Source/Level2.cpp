//------------------------------------------------------------------------------
//
// File Name:	Level1.cpp
// Author(s):	Jacob Holyfield & Nathan Miller
// Project:		BetaEngine
// Course:		CS230
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <Space.h>
#include "Level1.h"
#include "Level2.h"
#include <sstream>
#include <iomanip>

//Components
#include <Transform.h>
#include <SpriteText.h>
#include <Transform.h>
#include <Physics.h>
#include <ColliderTilemap.h>
#include <SpriteTilemap.h>
#include "MonkeyMovement.h"
#include "PacManMovement.h"
#include "PacManLogic.h"

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

Levels::Level2::Level2() : Level("LevelEditor"), cherryPos(Vector2D(0, 0)), cherrySpawned(false)
{
	//Objects
	player = nullptr;
	scoreObj = nullptr;
	highScoreObj = nullptr;

	// Sound manager
	soundManager = nullptr;
	musicChannel = nullptr;
}

void Levels::Level2::Load()
{
	std::cout << GetName() << "::Load" << std::endl;

	System::GetInstance().SetWindowTitle(WindowTitle);

	Graphics::GetInstance().GetCurrentCamera().Reset();

	//GetSpace()->GetObjectManager().AddArchetype(*GameObjectFactory::GetInstance().CreateObject("Cherry"));

	GetSpace()->GetObjectManager().AddArchetype(*GameObjectFactory::GetInstance().CreateObject("Pellet"));

	//Setup Sounds
	soundManager = Engine::GetInstance().GetModule<SoundManager>();
	soundManager->AddMusic("Asteroid_Field.mp3");
	soundManager->AddEffect("teleport.wav");
	//soundManager->AddEffect("pac-man_ghost new.wav");

	soundManager->AddBank("Master Bank.strings.bank");
	soundManager->AddBank("Master Bank.bank");

	SetFileLocation("Assets/Level2.lvl");
}

void Levels::Level2::Initialize()
{
	Graphics::GetInstance().SetBackgroundColor(Colors::White);
	std::cout << GetName() << "::Initialize" << std::endl;

	LoadLevel();

	soundManager->PlaySound("pac-man_ghost new.wav");

	player = GetSpace()->GetObjectManager().GetObjectByName("PacMan");

	scoreObj = GetSpace()->GetObjectManager().GetObjectByName("Score");
	highScoreObj = GetSpace()->GetObjectManager().GetObjectByName("HighScore");

	colliderTilemap = GetSpace()->GetObjectManager().GetObjectByName("TileMap")->GetComponent<ColliderTilemap>();


#ifdef _DEBUG
	std::cout << "WARNING Not Adding Dots because they are slow in debug" << std::endl;
#else
	int pelletsSetToLevel = 0;
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
				pelletsSetToLevel += 1;
			}
		}
	}
	player->GetComponent<PacManLogic>()->SetPellets(pelletsSetToLevel);
#endif RELEASE


}

void Levels::Level2::Update(float dt)
{
	/*
	if (player->GetComponent<PacManLogic>()->GetPellets() == 70 && cherrySpawned == false)
	{
	GameObject* cherryObj = new GameObject(*GetSpace()->GetObjectManager().GetArchetypeByName("Cherry"));
	cherryPos = colliderTilemap->ConvertTileMapCordsToWorldCords(Vector2D(50, 50));
	cherryObj->GetComponent<Transform>()->SetTranslation(cherryPos);
	GetSpace()->GetObjectManager().AddObject(*cherryObj);
	cherrySpawned = true;
	}
	*/
	if (player->GetComponent<PacManLogic>()->GetPellets() == 0)
	{
		Engine::GetInstance().Stop();
	}

	std::stringstream ss;

	ss << std::setw(6) << std::setfill('0') << player->GetComponent<PacManLogic>()->score;

	scoreObj->GetComponent<SpriteText>()->SetText(ss.str());
}

void Levels::Level2::Shutdown()
{
	std::cout << GetName() << "::Shutdown" << std::endl;

	//SaveLevel();

	musicChannel->stop();
	musicChannel = nullptr;
}

void Levels::Level2::Unload()
{
	std::cout << GetName() << "::Unload" << std::endl;

	soundManager->Shutdown();
}

bool Levels::Level2::IsObjectAt(Vector2D pos)
{
	std::vector<GameObject*> objs = GetSpace()->GetObjectManager().GetGameObjectActiveList();

	for (unsigned i = 0; i < objs.size(); i++)
	{
		if (AlmostEqual(objs[i]->GetComponent<Transform>()->GetTranslation(), pos))
			return true;
	}

	return false;
}