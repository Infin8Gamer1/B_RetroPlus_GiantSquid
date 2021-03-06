#include <stdafx.h>
#include "MenuLevel.h"
#include <Input.h>
#include <Space.h>
#include <Level.h>
#include "Level1.h"

Levels::MenuLevel::MenuLevel():Level("LevelMenu")
{
}

void Levels::MenuLevel::Load()
{
	SetFileLocation("Assets/LevelMenu.lvl");
}

void Levels::MenuLevel::Initialize()
{
	LoadLevel();
}

void Levels::MenuLevel::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	if (Input::GetInstance().CheckTriggered(VK_RETURN) || Input::GetInstance().CheckTriggered(VK_SPACE))
	{
		MenuLevel::GetSpace()->SetLevel<Levels::Level1>();
	}
}

void Levels::MenuLevel::Shutdown()
{
}

void Levels::MenuLevel::Unload()
{
}
