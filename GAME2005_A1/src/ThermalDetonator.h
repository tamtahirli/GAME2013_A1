#pragma once

#include "DisplayObject.h"
#include "TextureManager.h"

class ThermalDetonator final : public DisplayObject
{
public:
	ThermalDetonator();
	~ThermalDetonator();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

	void addForce(glm::vec2 Amount);

	void move();

	bool doesUpdate = false; 

	glm::vec2 Gravity = glm::vec2(0, 9.8f);

	bool calculateTheta = true;
	bool highThrow = true;
	float Speed = 95.0f;
	float pixelsPerMeter = 1.0f;
	float throwAngle = 15.9f;

	glm::vec2 Force = glm::vec2(0.0f, 0.0f);


private:

};


