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
#include "Transform.h"
#include <Texture.h>
#include <ResourceManager.h>
#include <Animation.h>
#include "GhostBehavior.h"
#include "PacManMovement.h"

void PacManCollisionHandler(GameObject & object, GameObject & other)
{
	if (other.GetName().substr(0, 6) == "Pellet")
	{
		object.GetComponent<PacManLogic>()->score += object.GetComponent<PacManLogic>()->pelletScore;
		object.GetComponent<PacManLogic>()->pelletsLeft -= 1;
		object.GetComponent<PacManLogic>()->soundManager->PlaySound("pac-man_chomp.wav");
		other.Destroy();
	}
	if (other.GetName().substr(0, 6) == "Cherry")
	{
		object.GetComponent<PacManLogic>()->score += 100;
		object.GetComponent<PacManLogic>()->soundManager->PlaySound("pac-man_chomp.wav");
		other.Destroy();
	}
	if (other.GetName().substr(0, 11) == "PowerPellet")
	{
		object.GetComponent<PacManLogic>()->score += 50;
		//std::cout << object.GetComponent<PacManLogic>()->pelletsLeft;
		object.GetComponent<PacManLogic>()->pelletsLeft -= 1;
		object.GetComponent<PacManLogic>()->isInvincible = true;
		object.GetComponent<PacManLogic>()->soundManager->PlaySound("pac-man_power pellet new.wav");

		object.GetComponent<PacManLogic>()->invincibleTimer = 10.0f;

		for (unsigned i = 0; i < object.GetComponent<PacManLogic>()->ghosts.size(); ++i)
		{
			object.GetComponent<PacManLogic>()->ghosts[i]->GetComponent<GhostBehavior>()->SetState(GhostState::Frightened);
			object.GetComponent<PacManLogic>()->ghosts[i]->GetComponent<GhostBehavior>()->isFrightenedLow = false;
		}

		other.Destroy();
	}
	if (other.GetName().substr(0, 5) == "Ghost" && object.GetComponent<PacManLogic>()->deathTimer == -1 && other.GetComponent<GhostBehavior>()->GetState() != GhostState::Dead && other.GetComponent<GhostBehavior>()->GetState() != GhostState::Frightened)
	{
		object.GetComponent<Sprite>()->SetSpriteSource(ResourceManager::GetInstance().GetSpriteSource("PacMan/PacDeath", true));
		object.GetComponent<Sprite>()->RefreshAutoMesh();
		object.GetComponent<Animation>()->Play(0.075f, false, false);

		//set the death timer (automagicly starts timer countdown to level reset or engine shutdown)
		object.GetComponent<PacManLogic>()->deathTimer = 0.075f * 20;
		object.GetComponent<PacManMovement>()->enableMove = false;
	}
	else if(object.GetComponent<PacManLogic>()->isInvincible && other.GetName().substr(0, 5) == "Ghost" && other.GetComponent<GhostBehavior>()->GetState() == GhostState::Frightened)
	{
		object.GetComponent<PacManLogic>()->score += 200 * object.GetComponent<PacManLogic>()->ghostMultiplier;
		object.GetComponent<PacManLogic>()->ghostMultiplier += 1;

		other.GetComponent<GhostBehavior>()->SetState(GhostState::Dead);
	}
}

PacManLogic::PacManLogic()
	: Component("PacManLogic"), score(0), highScore(10000), pelletScore(10), powerPelletScore(50),
	  pelletsLeft(1), isInvincible(false), isInvincibleLow(false), invincibleTimer(10.0f), lives(3)
{
	deathTimer = -1;
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
	GetOwner()->GetComponent<Collider>()->SetCollisionHandler(PacManCollisionHandler);
	startPos = GetOwner()->GetComponent<Transform>()->GetTranslation();
}

void PacManLogic::Update(float dt)
{
	if (ghosts.size() == 0)
	{
		std::vector<GameObject*> objs = GetOwner()->GetSpace()->GetObjectManager().GetGameObjectActiveList();

		for (unsigned i = 0; i < objs.size(); ++i)
		{
			if (objs[i]->GetName().substr(0, 5) == "Ghost")
			{
				ghosts.push_back(objs[i]);
			}
		}
	}

	if (deathTimer != -1)
	{
		if (deathTimer <= 0)
		{
			if (lives >= 1)
			{
				lives -= 1;
			}
			else
			{
				Engine::GetInstance().Stop();

				//TODO: restart level or go back to the main menu
			}
			GetOwner()->GetComponent<Transform>()->SetTranslation(startPos);
			GetOwner()->GetComponent<PacManMovement>()->enableMove = true;

			for (unsigned i = 0; i < GetOwner()->GetComponent<PacManLogic>()->ghosts.size(); ++i)
			{
				GetOwner()->GetComponent<PacManLogic>()->ghosts[i]->GetComponent<GhostBehavior>()->ResetPos();
			}

			//is managed on PacManMovement
			/*GetOwner()->GetComponent<Sprite>()->SetSpriteSource(ResourceManager::GetInstance().GetSpriteSource("PacMan/PacManStop", true));
			GetOwner()->GetComponent<Sprite>()->RefreshAutoMesh();
			GetOwner()->GetComponent<Animation>()->Play(0.075f, true, false);*/

			deathTimer = -1;
		}
		else 
		{
			deathTimer -= dt;
		}
	}

	

	if (isInvincible == true)
	{
		invincibleTimer -= dt;
		if (invincibleTimer <= 3.0f)
		{
			if (isInvincibleLow == false)
			{
				//set ghost frightened Low to true
				for (size_t i = 0; i < ghosts.size(); ++i)
				{
					ghosts[i]->GetComponent<GhostBehavior>()->isFrightenedLow = true;
				}
			}

			isInvincibleLow = true;
		}

		if (invincibleTimer <= 0)
		{
			invincibleTimer = 10.0f;
			isInvincible = false;
			isInvincibleLow = false;
			ghostMultiplier = 1;

			//put ghosts into scatter
			for (size_t i = 0; i < ghosts.size(); ++i)
			{
				ghosts[i]->GetComponent<GhostBehavior>()->SetState(GhostState::Scatter);
			}
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
