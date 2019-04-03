#include "stdafx.h"
#include "GhostBehaviorBlue.h"
#include <ColliderTilemap.h>
#include <GameObject.h>
#include <Transform.h>
#include <Space.h>
#include <TileMapNavigation.h>
#include <Sprite.h>
#include <ResourceManager.h>

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
			//sprite->SetSpriteSource(ResourceManager::GetInstance().GetSpriteSource("GhostEyes", true));
			navigation->SetTarget(colliderTilemap->ConvertWorldCordsToTileMapCords(PacManTransform->GetTranslation()));
			break;
		case Chase:
			sprite->SetSpriteSource(ResourceManager::GetInstance().GetSpriteSource("GhostBlue", true));
			break;
		case Scatter:
			sprite->SetSpriteSource(ResourceManager::GetInstance().GetSpriteSource("GhostBlue", true));
			break;
		case Frightened:
			sprite->SetSpriteSource(ResourceManager::GetInstance().GetSpriteSource("GhostPower", true));
			break;
		default:
			break;
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
			if (colliderTilemap->ConvertWorldCordsToTileMapCords(transform->GetTranslation()).Distance(startPos) < 0.1)
			{
				state = Chase;
			}
			break;
		case Chase:
			navigation->SetTarget(colliderTilemap->ConvertWorldCordsToTileMapCords(PacManTransform->GetTranslation()));
			break;
		case Scatter:
			break;
		case Frightened:
			break;
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
		break;
	default:
		break;
	}
}
