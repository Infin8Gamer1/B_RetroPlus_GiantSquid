#include "stdafx.h"
#include "PacManMovement.h"
#include <GameObject.h>
#include <Collider.h>
#include <Animation.h>
#include <Sprite.h>
#include <Transform.h>
#include <Physics.h>
#include <CallbackInputManager.h>
#include <Space.h>
#include <GameObjectManager.h>
#include <ColliderTilemap.h>
#include <Tilemap.h>
#include <DebugDraw.h>
#include <Graphics.h>
#include <ResourceManager.h>
#include <Sprite.h>

Directions direction = Directions::Up;
MapCollision mapCollision = MapCollision(false, false, false, false);
bool move = false;

void PacManMapCollisionHandler(GameObject & object, const MapCollision & collision)
{
	mapCollision = collision;
}

void PacManMovement::OnKeyInputDown(int key)
{
	switch (key)
	{
	case GLFW_KEY_UP:
	case GLFW_KEY_W:
		direction = Directions::Up;
		move = true;
		break;
	case GLFW_KEY_A:
	case GLFW_KEY_LEFT:
		direction = Directions::Left;
		move = true;
		break;
	case GLFW_KEY_S:
	case GLFW_KEY_DOWN:
		direction = Directions::Down;
		move = true;
		break;
	case GLFW_KEY_D:
	case GLFW_KEY_RIGHT:
		direction = Directions::Right;
		move = true;
		break;
	default:
		break;
	}
}

PacManMovement::PacManMovement() : Component("PacManMovement")
{
	// Components
	transform = nullptr;
	physics = nullptr;
	sprite = nullptr;
	colliderTilemap = nullptr;

	enableMove = true;
}

Component * PacManMovement::Clone() const
{
	return new PacManMovement(*this);
}

void PacManMovement::Initialize()
{
	//get Components
	transform = GetOwner()->GetComponent<Transform>();
	physics = GetOwner()->GetComponent<Physics>();
	sprite = GetOwner()->GetComponent<Sprite>();
	colliderTilemap = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("TileMap")->GetComponent<ColliderTilemap>();

	CallbackInputManager::GetInstance().addKeyPressBinding(ALL_KEYS, OnKeyInputDown);

	//set the collision handler for the monkey
	GetOwner()->GetComponent<Collider>()->SetMapCollisionHandler(PacManMapCollisionHandler);
}

void PacManMovement::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	if (enableMove)
	{
		//update the sprite baised on if PacMan is moving or not
		if (abs(physics->GetVelocity().x) < 0.1f && abs(physics->GetVelocity().y) < 0.1f)
		{
			sprite->SetSpriteSource(ResourceManager::GetInstance().GetSpriteSource("PacMan/PacManStop", true));
			sprite->RefreshAutoMesh();
		}
		else if (sprite->GetSpriteSource() != ResourceManager::GetInstance().GetSpriteSource("PacMan/PacManGo", true))
		{
			sprite->SetSpriteSource(ResourceManager::GetInstance().GetSpriteSource("PacMan/PacManGo", true));
			sprite->RefreshAutoMesh();
		}

		Move();
	}
	else {
		physics->SetVelocity(Vector2D());
	}

	
}

void PacManMovement::Move()
{
	float previousRotation = transform->GetRotation();

	if (CheckDirection(direction))
	{
		switch (direction)
		{
		case Up:
		{
			transform->SetRotation(M_PI_2);
			break;
		}
		case Down:
		{
			transform->SetRotation(3 * M_PI / 2);
			break;
		}
		case Left:
		{
			transform->SetRotation(M_PI);
			break;
		}
		case Right:
		{
			transform->SetRotation(0);
			break;
		}
		default:
			break;
		}
	}

	if (previousRotation != transform->GetRotation())
	{
		Vector2D CurrentTile = colliderTilemap->ConvertWorldCordsToTileMapCords(transform->GetTranslation());
		if (colliderTilemap->GetTilemap()->GetCellValue(CurrentTile.x, CurrentTile.y) == 0)
		{
			transform->SetTranslation(colliderTilemap->ConvertTileMapCordsToWorldCords(CurrentTile));
		}
	}

	if (move)
	{
		physics->SetVelocity(transform->Forward() * moveSpeed);


        if (transform->GetTranslation().x > 280) transform->SetTranslation(Vector2D(-280, transform->GetTranslation().y));
        else if (transform->GetTranslation().x < -280) transform->SetTranslation(Vector2D(280, transform->GetTranslation().y));
	}
}

bool PacManMovement::CheckDirection(Directions _direction)
{
	Vector2D CurrentTile = colliderTilemap->ConvertWorldCordsToTileMapCords(transform->GetTranslation());

	int value = -1;
	Vector2D nextTile = Vector2D(0, 0);

	switch (_direction)
	{
		case Up:
		{
			nextTile = Vector2D(CurrentTile.x, CurrentTile.y - 1);
			value = colliderTilemap->GetTilemap()->GetCellValue(nextTile.x, nextTile.y);
			break;
		}
		case Down:
		{
			nextTile = Vector2D(CurrentTile.x, CurrentTile.y + 1);
			value = colliderTilemap->GetTilemap()->GetCellValue(nextTile.x, nextTile.y);
			break;
		}
		case Left:
		{
			nextTile = Vector2D(CurrentTile.x - 1, CurrentTile.y);
			value = colliderTilemap->GetTilemap()->GetCellValue(nextTile.x, nextTile.y);
			break;
		}
		case Right:
		{
			nextTile = Vector2D(CurrentTile.x + 1, CurrentTile.y);
			value = colliderTilemap->GetTilemap()->GetCellValue(nextTile.x, nextTile.y);
			break;
		}
	}

	Color debugColor = (value == 0) ? Color(0, 1, 0, 1) : Color(1, 0, 0, 1);
	DebugDraw::GetInstance().AddRectangle(colliderTilemap->ConvertTileMapCordsToWorldCords(nextTile), Vector2D(12.5f, 12.5f), Graphics::GetInstance().GetCurrentCamera(), debugColor);
	//DebugDraw::GetInstance().AddCircle(colliderTilemap->ConvertTileMapCordsToWorldCords(nextTile), 10, Graphics::GetInstance().GetCurrentCamera(), debugColor);


	return (value == 0);
}
