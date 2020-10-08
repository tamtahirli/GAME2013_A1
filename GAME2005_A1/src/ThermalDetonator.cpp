#include "ThermalDetonator.h"
#include "glm/gtx/string_cast.hpp"
#include "PlayScene.h"
#include "TextureManager.h"
#include "Util.h"
#include "Game.h"

ThermalDetonator::ThermalDetonator()
{
	TextureManager::Instance()->load("../Assets/textures/detonator.png", "ThermalDetonator");

	auto size = TextureManager::Instance()->getTextureSize("ThermalDetonator");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(THERMALDETONATOR);
}


ThermalDetonator::~ThermalDetonator()
= default;

void ThermalDetonator::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the ThermalDetonator
	TextureManager::Instance()->draw("ThermalDetonator", x, y, 0.0f, 255, true);
}


void ThermalDetonator::update()
{
	if(doesUpdate) move();
}

void ThermalDetonator::clean()
{

}

void ThermalDetonator::addForce(glm::vec2 Amount)
{
	Force += Amount;
}

void ThermalDetonator::move()
{
	float deltaTime = 1.0f / 60.0f;

	getRigidBody()->acceleration = Gravity;
	getRigidBody()->velocity += getRigidBody()->acceleration * deltaTime;
	getTransform()->position += getRigidBody()->velocity * deltaTime * pixelsPerMeter;

	//getRigidBody()->acceleration = glm::vec2(0.0f, Gravity);
	//getRigidBody()->acceleration += Force / getRigidBody()->mass;
	//getRigidBody()->velocity += getRigidBody()->acceleration;
	//getTransform()->position += getRigidBody()->velocity * deltaTime;

	//Force = glm::vec2(0.0f, 0.0f);
	//getRigidBody()->velocity *= 0.9f;
}

