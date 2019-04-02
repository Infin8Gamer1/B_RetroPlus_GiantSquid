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
#include <Engine.h>
#include <Texture.h>
#include <ResourceManager.h>


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
	if (other.GetName() == "PowerPellet")
	{
		object.GetComponent<PacManLogic>()->score += 50;
		object.GetComponent<PacManLogic>()->isInvincible = true;
		object.GetComponent<PacManLogic>()->soundManager->PlaySound("pac-man_power pellet new.wav");
		other.Destroy();
	}
	if (other.GetName().substr(0, 5) == "Ghost")
	{
		object.GetComponent<Sprite>()->SetSpriteSource(object.GetComponent<PacManLogic>()->pacDeathSpriteSource);
		while (object.GetComponent<PacManLogic>()->deathTimer > 0)
		{
			object.GetComponent<PacManLogic>()->deathTimer -= 0.16f;
		}
		object.GetComponent<PacManLogic>()->deathTimer = 4;
		Engine::GetInstance().Stop();
	}
}

PacManLogic::PacManLogic()
	: Component("PacManLogic"), score(0), highScore(0), pelletScore(10), powerPelletScore(50), 
	  pelletsLeft(2), isInvincible(false), invincibleTimer(10.0f)
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
