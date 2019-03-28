//------------------------------------------------------------------------------
//
// File Name:	Collider.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Collider.h"
#include "Transform.h"
#include "Physics.h"
#include "GameObject.h"
#include <Parser.h>

Collider::Collider(ColliderType _type) : Component("Collider")
{
	type = _type;

	enabled = true;

	handler = nullptr;
	mapHandler = nullptr;
	bar = nullptr;
}

Collider::~Collider()
{
	if (bar != nullptr)
	{
		TwRemoveVar(bar, "Collision Group");
	}
}

void Collider::Initialize()
{
	transform = GetOwner()->GetComponent<Transform>();
	physics = GetOwner()->GetComponent<Physics>();
}

void Collider::AddVarsToTweakBar(TwBar * bar_)
{
	bar = bar_;

	Component::AddVarsToTweakBar(bar);
	std::string params = " group='" + GetName() + "' ";

	TwAddVarRW(bar, "Collision Group", TW_TYPE_INT32, &CollisionGroup, params.c_str());
	//TwAddVarRW(bar, "Collision Flags", TW_TYPE_STDSTRING, &CollisionFlags, params.c_str());
}

void Collider::CheckCollision(const Collider & other)
{
	if (!enabled || !other.enabled) {
		return;
	}
	
	/*NOTES:
		Groups:
			do not set group to negitive value or 0
			default = 1
		Flags:
			0 = All
			- values are considered not flags and + values are considered true flags
	*/

	bool canCollide = false;
	
	//if the flag all is in the flags or the flags are empty then collide
	bool isAllFlag = false;
	for (const int& i : CollisionFlags)
	{
		if (i == 0) {
			isAllFlag = true;
			break;
		}
	}

	if (isAllFlag || CollisionFlags.empty()) {
		canCollide = true;
	} else {
		//loop through each of the flags
		for (size_t i = 0; i < CollisionFlags.size(); i++)
		{
			int currentFlag = CollisionFlags[i];
			bool doesEqual = currentFlag == other.CollisionGroup;
			//if the current flag starts with -
			if (currentFlag < 0 && doesEqual)
			{
				//if the flag and the others collision group match then we don't want to collide
				canCollide = false;
				break;
			} 
			else if (doesEqual)
			{
				//there isn't an - on the flag so if the collision group matches we do want to collide
				canCollide = true;
				break;
			}
		}
	}

	//handle the auctual collision maths, math gets shortcircited if canCollide is false
	if (canCollide && IsCollidingWith(other)) {
		if (this->handler != nullptr) {
			this->handler(*this->GetOwner(), *other.GetOwner());
		}
		if (other.handler != nullptr) {
			other.handler(*other.GetOwner(), *this->GetOwner());
		}
	}
}

ColliderType Collider::GetType() const
{
	return type;
}

void Collider::SetCollisionHandler(CollisionEventHandler _handler)
{
	handler = _handler;
}

void Collider::SetMapCollisionHandler(MapCollisionEventHandler _mapHandler)
{
	mapHandler = _mapHandler;
}

MapCollisionEventHandler Collider::GetMapCollisionHandler() const
{
	return mapHandler;
}

void Collider::BaseSerialize(Parser & parser) const
{
	parser.WriteVariable("CollisionGroup", CollisionGroup);
	
	std::string flags = printVector(CollisionFlags);

	parser.WriteVariable("CollisionFlags", flags);
}

void Collider::BaseDeserialize(Parser & parser)
{
	parser.ReadVariable("CollisionGroup", CollisionGroup);

	std::string collisionGroupsString;
	parser.ReadVariable("CollisionFlags", collisionGroupsString);

	CollisionFlags = readVector(collisionGroupsString, ',');
}

void Collider::Disable()
{
	enabled = false;
}

void Collider::Enable()
{
	enabled = true;
}

MapCollision::MapCollision(bool _bottom, bool _top, bool _left, bool _right)
{
	bottom = _bottom;
	top = _top;
	left = _left;
	right = _right;
}

std::vector<int> Collider::readVector(const std::string & str, const char & ch)
{
	std::string next;
	std::vector<int> result;

	// For each character in the string
	for (std::string::const_iterator it = str.begin(); it != str.end(); it++) {
		// If we've hit the terminal character
		if (*it == ch) {
			// If we have some characters accumulated
			if (!next.empty()) {
				// Add them to the result vector
				result.push_back(std::stoi(next));
				next.clear();
			}
		}
		else {
			// Accumulate the next character into the sequence
			next += *it;
		}
	}
	if (!next.empty())
		result.push_back(std::stoi(next));
	return result;
}

std::vector<std::string> Collider::explodeString(const std::string& str, const char& ch) {
	std::string next;
	std::vector<std::string> result;

	// For each character in the string
	for (std::string::const_iterator it = str.begin(); it != str.end(); it++) {
		// If we've hit the terminal character
		if (*it == ch) {
			// If we have some characters accumulated
			if (!next.empty()) {
				// Add them to the result vector
				result.push_back(next);
				next.clear();
			}
		}
		else {
			// Accumulate the next character into the sequence
			next += *it;
		}
	}
	if (!next.empty())
		result.push_back(next);
	return result;
}
