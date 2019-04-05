#include "stdafx.h"
#include "GhostBehaviorBlue.h"
#include <ColliderTilemap.h>
#include <GameObject.h>
#include <Transform.h>
#include <Space.h>
#include <TileMapNavigation.h>
#include <Sprite.h>
#include <ResourceManager.h>

GhostBehavior::GhostBehavior() : Component("GhostBehavior")
{
	// Components
	PacManTransform = nullptr;
	transform = nullptr;
	navigation = nullptr;
	sprite = nullptr;
	colliderTilemap = nullptr;

	timer = 0.0f;
	timeOutCounter = -1;

	state = GhostState::Chase;
	previousState = state;

	startPos = Vector2D();

	isFrightenedLow = false;
}

void GhostBehavior::Initialize()
{
	PacManTransform = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("PacMan")->GetComponent<Transform>();
	colliderTilemap = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("TileMap")->GetComponent<ColliderTilemap>();
	sprite = GetOwner()->GetComponent<Sprite>();
	navigation = GetOwner()->GetComponent<TileMapNavigation>();
	transform = GetOwner()->GetComponent<Transform>();

	navigation->SetTarget(colliderTilemap->ConvertWorldCordsToTileMapCords(PacManTransform->GetTranslation()));
	navigation->SetMoveSpeed(90.0f);

	startPos = colliderTilemap->ConvertWorldCordsToTileMapCords(transform->GetTranslation());
}

void GhostBehavior::Update(float dt)
{
	timer += dt;

	previousState = state;
}

void GhostBehavior::SetState(GhostState _state)
{
	switch (state)
	{
	case Dead:
		break;
	default:
		state = _state;
		break;
	}
}

GhostState GhostBehavior::GetState()
{
	return state;
}

void GhostBehavior::ResetPos()
{
	transform->SetTranslation(colliderTilemap->ConvertTileMapCordsToWorldCords(startPos));
	navigation->SetTarget(startPos);

	timeOutCounter = -1;
	timer = 0.0f;

	state = Chase;
}
