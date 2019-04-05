#include "stdafx.h"
#include "GhostBehaviorBlue.h"
#include <ColliderTilemap.h>
#include <GameObject.h>
#include <Transform.h>
#include <Space.h>
#include <TileMapNavigation.h>
#include <Sprite.h>
#include <ResourceManager.h>
#include <Collider.h>
#include <Tilemap.h>

GhostBehaviorBlue::GhostBehaviorBlue() : GhostBehavior()
{
}

Component * GhostBehaviorBlue::Clone() const
{
	return new GhostBehaviorBlue(*this);
}

void GhostBehaviorBlue::Update(float dt)
{
	//on state change
	//IMPORTANT needs to be before previous state gets updated in the base update function
	if (previousState != state)
	{
		switch (state)
		{
		case Dead:
			sprite->SetSpriteSource(ResourceManager::GetInstance().GetSpriteSource("Ghosts/GhostDeadUp", true));
			sprite->RefreshAutoMesh();
			navigation->SetTarget(startPos);
			GetOwner()->GetComponent<Collider>()->Disable();

			navigation->SetMoveSpeed(175.0f);
			break;
		case Chase:
			sprite->SetSpriteSource(ResourceManager::GetInstance().GetSpriteSource("Ghosts/Blue/GhostBlueUp", true));
			sprite->RefreshAutoMesh();

			navigation->SetMoveSpeed(90.0f);
			break;
		case Scatter:
			sprite->SetSpriteSource(ResourceManager::GetInstance().GetSpriteSource("Ghosts/Blue/GhostBlueUp", true));
			sprite->RefreshAutoMesh();

			navigation->SetTarget(Vector2D(1,1));

			navigation->SetMoveSpeed(90.0f);
			break;
		case Frightened:
			sprite->SetSpriteSource(ResourceManager::GetInstance().GetSpriteSource("Ghosts/GhostFrightened", true));
			sprite->RefreshAutoMesh();

			navigation->SetMoveSpeed(80.0f);

			navigation->SetTarget(GenerateTarget());
			break;
		default:
			break;
		}

		if (state != Dead)
		{
			GetOwner()->GetComponent<Collider>()->Enable();
		}
	}

	//updates previous state and timer
	GhostBehavior::Update(dt);

	//on timer update
	if (timer > 0.2f)
	{
		timer = 0;

		switch (state)
		{
		case Dead:
			if (colliderTilemap->ConvertWorldCordsToTileMapCords(transform->GetTranslation()).Distance(startPos) < 0.1f)
			{
				state = Chase;
			}
			break;
		case Chase:
			navigation->SetTarget(colliderTilemap->ConvertWorldCordsToTileMapCords(PacManTransform->GetTranslation()));
			break;
		case Scatter:
			if (colliderTilemap->ConvertWorldCordsToTileMapCords(transform->GetTranslation()).Distance(navigation->GetTarget()) < 0.5f)
			{
				state = Chase;
			}
			break;
		case Frightened:
		{
			if (colliderTilemap->ConvertWorldCordsToTileMapCords(transform->GetTranslation()).Distance(navigation->GetTarget()) < 0.2f)
			{
				//state = Chase;
				navigation->SetTarget(GenerateTarget());
			}
			break;
		}
		default:
			break;
		}
		
	}

	//every frame
	switch (state)
	{
	case Dead:
		break;
	case Chase:
		break;
	case Scatter:
		break;
	case Frightened:
		if (isFrightenedLow)
		{
			sprite->SetSpriteSource(ResourceManager::GetInstance().GetSpriteSource("Ghosts/GhostFrightenedEnd", true));
			sprite->RefreshAutoMesh();

			isFrightenedLow = false;
		}
		break;
	default:
		break;
	}
}

Vector2D GhostBehaviorBlue::GenerateTarget()
{
	Vector2D target = colliderTilemap->ConvertWorldCordsToTileMapCords(transform->GetTranslation());
	target = target + Vector2D(-6 + (std::rand() % (6 - -6 + 1)), -6 + (std::rand() % (6 - -6 + 1)));

	if (colliderTilemap->GetTilemap()->GetCellValue(target.x, target.y) != 0)
	{
		target = GenerateTarget();
	}

	return target;
}
