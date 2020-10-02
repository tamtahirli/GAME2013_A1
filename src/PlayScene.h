#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "ThermalDetonator.h"

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
};

#endif /* defined (__PLAY_SCENE__) */