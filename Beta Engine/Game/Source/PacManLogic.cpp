#include "stdafx.h"
#include "PacManLogic.h"
#include "Level1.h"
#include <Space.h>
#include <Collider.h>
#include <GameObject.h>
#include <SoundManager.h>
#include <Engine.h>



void PacManCollisionHandler(GameObject & object, GameObject & other)
{
	if (other.GetName() == "Pellet")
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
	if (other.GetName() == "Ghost")
	{
		Engine::GetInstance().Stop();
	}
}

PacManLogic::PacManLogic()
	: Component("PacManLogic"), score(0), highScore(0), pelletScore(10), powerPelletScore(50), 
	  pelletsLeft(240), isInvincible(false), invincibleTimer(10.0f)
{
	soundManager = Engine::GetInstance().GetModule<SoundManager>();
	soundManager->AddEffect("pac-man_chomp.wav");
	soundManager->AddEffect("pac-man_power pellet new.wav");
}

Component * PacManLogic::Clone() const
{
	return new PacManLogic(*this);
}

void PacManLogic::Initialize()
{
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
