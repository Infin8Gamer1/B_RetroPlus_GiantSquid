#include "stdafx.h"
#include "PacManLogic.h"
#include "Level1.h"
#include <string>
#include <Space.h>
#include <Collider.h>
#include <GameObject.h>
#include <SoundManager.h>
#include <SpriteSource.h>
#include <Sprite.h>
#include <SpriteText.h>
#include <Engine.h>
#include <Texture.h>
#include <ResourceManager.h>
#include <Animation.h>

GameObject* scoreObj;

unsigned score;

void PacManCollisionHandler(GameObject & object, GameObject & other)
{
	if (other.GetName().substr(0, 6) == "Pellet")
	{
		object.GetComponent<PacManLogic>()->score += object.GetComponent<PacManLogic>()->pelletScore;
		object.GetComponent<PacManLogic>()->pelletsLeft -= 1;
		object.GetComponent<PacManLogic>()->soundManager->PlaySound("pac-man_chomp.wav");
		other.Destroy();
	}
	if (other.GetName() == "Cherry")
	{
		object.GetComponent<PacManLogic>()->score += 100;
		object.GetComponent<PacManLogic>()->soundManager->PlaySound("pac-man_chomp.wav");
		other.Destroy();
	}
	if (other.GetName().substr(0, 11) == "PowerPellet")
	{
		object.GetComponent<PacManLogic>()->score += 50;

        scoreObj->GetComponent<SpriteText>()->SetText("Score: " + score);

		object.GetComponent<PacManLogic>()->isInvincible = true;
		object.GetComponent<PacManLogic>()->soundManager->PlaySound("pac-man_power pellet new.wav");
		other.Destroy();
	}
	if (other.GetName().substr(0, 5) == "Ghost" && !object.GetComponent<PacManLogic>()->isInvincible)
	{
		object.GetComponent<Sprite>()->SetSpriteSource(object.GetComponent<PacManLogic>()->pacDeathSpriteSource);
		object.GetComponent<Animation>()->Play(0.1, false, false);
		while (object.GetComponent<PacManLogic>()->deathTimer > 0)
		{
			object.GetComponent<PacManLogic>()->deathTimer -= 0.16f;
		}
		object.GetComponent<PacManLogic>()->deathTimer = 4;
		Engine::GetInstance().Stop();
	}
	else if(object.GetComponent<PacManLogic>()->isInvincible)
	{
		std::cout << "GHOSTED" << std::endl;
	}
}

PacManLogic::PacManLogic()
	: Component("PacManLogic"), score(0), highScore(0), pelletScore(10), powerPelletScore(50), scoreObj(nullptr), highScoreObj(nullptr),
	  pelletsLeft(1), isInvincible(false), invincibleTimer(10.0f)
{
	soundManager = Engine::GetInstance().GetModule<SoundManager>();
	soundManager->AddEffect("pac-man_chomp.wav");
	soundManager->AddEffect("pac-man_power pellet new.wav");
}

Component * PacManLogic::Clone() const
{
	return new PacManLogic(*this);
}

void PacManLogic::Load()
{
}

void PacManLogic::Initialize()
{
	pacDeathSpriteSource = ResourceManager::GetInstance().GetSpriteSource("PacDeath", true);
	GetOwner()->GetComponent<Collider>()->SetCollisionHandler(PacManCollisionHandler);

    scoreObj = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("Score");
    highScoreObj = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("HighScore");
}

void PacManLogic::Update(float dt)
{
	if (isInvincible == true)
	{
		invincibleTimer -= dt;
		if (invincibleTimer <= 0)
		{
			invincibleTimer = 10.0f;
			isInvincible = false;
		}
	}
}

int PacManLogic::GetPellets()
{
	return pelletsLeft;
}

void PacManLogic::SetPellets(int amountOfPellets)
{
	pelletsLeft = amountOfPellets;
}
