#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	drawDisplayList();
}

void PlayScene::update()
{
	updateDisplayList();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();
	std::cout << m_pThermalDetonator->getTransform()->position.y << "\n";
	if (m_pThermalDetonator->getTransform()->position.y >= m_pStormTroopers->getTransform()->position.y + 50.0f)
	{
		m_pThermalDetonator->getTransform()->position = glm::vec2(125.0f, 400.0f);
		m_pThermalDetonator->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pThermalDetonator->doesUpdate = false;
	}
	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			keyDown = true;
		}
		if (EventManager::Instance().isKeyUp(SDL_SCANCODE_A))
		{
			if (keyDown && !m_pThermalDetonator->doesUpdate)
			{
				glm::vec2 distance = m_pStormTroopers->getTransform()->position - m_pThermalDetonator->getTransform()->position;

				float speed = 95.0f;
				float gravityByDistance = m_pThermalDetonator->Gravity * distance.x;
				float Equation = gravityByDistance / (speed * speed);
				float theta = 0.5 * glm::degrees(asin(Equation));

				std::cout << "DIST: " << distance.x << " THETA: " << theta << "\n";

				glm::vec2 DegreeToVector = glm::vec2(cos(theta), -sin(theta));

				std::cout << "Degree to vector: " << DegreeToVector.x << " y: " << DegreeToVector.y << "\n";
				m_pThermalDetonator->getRigidBody()->velocity += DegreeToVector * (speed * 2);
				m_pThermalDetonator->doesUpdate = true;
			}
			keyDown = false;
		}
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			// Press 'A' and throw object here.
			//m_pThermalDetonator->moveForward();
		}
	}
	

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{
	addChild(new Button("../Assets/textures/background.png", "background", BACKGROUND, glm::vec2(400.0f, 300.0f)));

	addChild(new Button("../Assets/textures/wookie.png", "wookie", WOOKIE, glm::vec2(100.0f, 400.0f)));
	m_pStormTroopers = new Button("../Assets/textures/stormtroopers.png", "stormtroopers", STORMTROOPERS, glm::vec2(610.0f, 400.0f));
	m_pStormTroopers->getTransform()->position = glm::vec2(610.0f, 400.0f);
	addChild(m_pStormTroopers);

	// Thermal Detonator Sprite
	m_pThermalDetonator = new ThermalDetonator();
	m_pThermalDetonator->getTransform()->position = glm::vec2(125.0f, 400.0f);
	m_pThermalDetonator->getRigidBody()->mass = 2.2f;
	addChild(m_pThermalDetonator);//

	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(300.0f, 500.0f);
	m_pBackButton->addEventListener(CLICK, [&]()-> void
	{
		m_pBackButton->setActive(false);
		TheGame::Instance()->changeSceneState(START_SCENE);
	});

	m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pBackButton->setAlpha(128);
	});

	m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pBackButton->setAlpha(255);
	});
	addChild(m_pBackButton);

	// Next Button
	m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	m_pNextButton->getTransform()->position = glm::vec2(500.0f, 500.0f);
	m_pNextButton->addEventListener(CLICK, [&]()-> void
	{
		m_pNextButton->setActive(false);
		TheGame::Instance()->changeSceneState(END_SCENE);
	});

	m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pNextButton->setAlpha(128);
	});

	m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pNextButton->setAlpha(255);
	});

	addChild(m_pNextButton);
}
