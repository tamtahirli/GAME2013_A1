#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"
#include "Renderer.h"
#include <IMGUI/imgui.h>

StartScene::StartScene()
{
	StartScene::start();
}

StartScene::~StartScene()
= default;

void StartScene::draw()
{
	drawDisplayList();
}

void StartScene::update()
{
	updateDisplayList();
}

void StartScene::clean()
{
	removeAllChildren();
}

void StartScene::handleEvents()
{
	EventManager::Instance().update();

	// Keyboard Events
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	}
}

void StartScene::start()
{
	addChild(new Button("../Assets/textures/background.png", "background", BACKGROUND, glm::vec2(400.0f, 300.0f)));

	const SDL_Color green = { 0, 100, 0, 255 };
	const SDL_Color red = { 200, 0, 0, 255 };
	const SDL_Color cyan = { 0, 200, 200, 255 };

	m_pStartLabel = new Label("Wookie Throw Grenade", "Consolas", 60, red, glm::vec2(400.0f, 40.0f));
	m_pStartLabel->setParent(this);
	addChild(m_pStartLabel);

	m_pGroupMemberLabel = new Label("Tamerlan Tahirli - 101055392 -- Alexander Barnes - 101086806", "Consolas", 20, cyan, glm::vec2(400.0f, 130.0f));
	m_pGroupMemberLabel->setParent(this);
	addChild(m_pGroupMemberLabel);

	m_pInstructionsLabel = new Label("Press Start or 1 to Play", "Consolas", 40, green, glm::vec2(400.0f, 200.0f));
	m_pInstructionsLabel->setParent(this);
	addChild(m_pInstructionsLabel);


	addChild(new Button("../Assets/textures/wookie.png", "wookie", WOOKIE, glm::vec2(400.0f, 300.0f)));

	// Start Button
	m_pStartButton = new Button();
	m_pStartButton->getTransform()->position = glm::vec2(400.0f, 400.0f); 

	m_pStartButton->addEventListener(CLICK, [&]()-> void
	{
		m_pStartButton->setActive(false);
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	});
	
	m_pStartButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pStartButton->setAlpha(128);
	});

	m_pStartButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pStartButton->setAlpha(255);
	});
	addChild(m_pStartButton);

	
}

