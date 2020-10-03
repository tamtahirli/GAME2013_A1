#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Button.h"
#include "ThermalDetonator.h"
#include "Label.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
private:
	glm::vec2 m_mousePosition;

	ThermalDetonator* m_pThermalDetonator;
	bool m_playerFacingRight;

	Button* m_pBackButton;
	Button* m_pNextButton;

	Button* m_pStormTroopers;

	bool Playing = false;

	bool keyDown = false;

	Label* DistanceLabel{};
	Label* MassLabel{};
	Label* PositionLabel{};
	Label* VelocityLabel{};
	Label* AccelerationLabel{};
	Label* ForceLabel{};

	float Speed = 95.0f;
	float StormTrooperDistance = 485.0f;
	float Mass = 2.2f;

	static float magnitude(glm::vec2 vec);
	void SetText();
};

#endif /* defined (__PLAY_SCENE__) */