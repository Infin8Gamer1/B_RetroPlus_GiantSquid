#include "stdafx.h"
#include "GhostBehaviorBlue.h"
#include <ColliderTilemap.h>
#include <GameObject.h>
#include <Transform.h>
#include <Space.h>
#include <TileMapNavigation.h>

GhostBehaviorBlue::GhostBehaviorBlue() : Component("GhostBehaviorBlue")
{
	// Components
	PacManTransform = nullptr;
	navigation = nullptr;
	colliderTilemap = nullptr;

	timer = 0;
}

Component * GhostBehaviorBlue::Clone() const
{
	return new GhostBehaviorBlue(*this);
}

void GhostBehaviorBlue::Initialize()
{
	PacManTransform = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("PacMan")->GetComponent<Transform>();
	colliderTilemap = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("TileMap")->GetComponent<ColliderTilemap>();
	navigation = GetOwner()->GetComponent<TileMapNavigation>();

	navigation->SetTarget(colliderTilemap->ConvertWorldCordsToTileMapCords(PacManTransform->GetTranslation()));
}

void GhostBehaviorBlue::Update(float dt)
{
	timer += dt;

	if (timer > 1.0f)
	{
		navigation->SetTarget(colliderTilemap->ConvertWorldCordsToTileMapCords(PacManTransform->GetTranslation()));
		timer = 0;
	}
}
