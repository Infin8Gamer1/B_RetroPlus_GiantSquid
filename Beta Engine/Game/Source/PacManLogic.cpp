#include "stdafx.h"
#include "PacManLogic.h"
#include <Space.h>
#include <Collider.h>
#include <GameObject.h>
#include "Pellet.h"



void PacManCollisionHandler(GameObject & object, GameObject & other)
{
	if (other.GetName() == "Pellet")
	{
		object.GetComponent<PacManLogic>()->score += object.GetComponent<PacManLogic>()->pelletScore;
		//std::cout << object.GetComponent<PacManLogic>()->score << std::endl;
		other.Destroy();
	}
}

PacManLogic::PacManLogic() : Component("PacManLogic"), score(0), highScore(0), pelletScore(10), powerPelletScore(50)
{
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
}
