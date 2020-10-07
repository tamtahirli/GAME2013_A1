#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "ThermalDetonator.h"
#include "Button.h"
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
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	Plane* m_pPlaneSprite;
	ThermalDetonator* m_pDetonator;
	bool m_playerFacingRight;

	glm::vec2 StormTrooperPos;
	float Speed;

	// UI Items
	Button* m_pBackButton;
	Button* m_pNextButton;
	Label* m_pInstructionsLabel;

	// mass, position, velocity, acceleration, force
	Label* MassLabel{};
	Label* PositionLabel{};
	Label* VelocityLabel{};
	Label* AccelerationLabel{};
	Label* ForceLabel{};

	void SetText();
	void CreateLabels();

	static float magnitude(glm::vec2 vec);
};

#endif /* defined (__PLAY_SCENE__) */