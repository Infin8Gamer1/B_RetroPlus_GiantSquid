#include "stdafx.h"
#include "Pellet.h"

Pellet::Pellet() : Component("Pellet")
{
}

Component * Pellet::Clone() const
{
	return new Pellet(*this);
}

void Pellet::Initialize()
{
}

void Pellet::Update(float dt)
{
}
