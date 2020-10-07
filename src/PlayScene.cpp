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
	if (m_pThermalDetonator->getTransform()->position.y >= m_pStormTroopers->getTransform()->position.y + 50.0f)
	{
		m_pThermalDetonator->getTransform()->position = glm::vec2(125.0f, 400.0f);
		m_pThermalDetonator->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pThermalDetonator->doesUpdate = false;
		SetText();
	}

	if (m_pThermalDetonator->doesUpdate)
	{
		SetText();
	}

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
				
				float gravityByDistance = m_pThermalDetonator->Gravity * distance.x;
				float Equation = gravityByDistance / (Speed * Speed);
				float theta = 0.5 * glm::degrees(asin(Equation));

				//std::cout << "DIST: " << distance.x << " THETA: " << theta << "\n";

				glm::vec2 DegreeToVector = glm::vec2(cos(theta), -sin(theta));

				//std::cout << "Degree to vector: " << DegreeToVector.x << " y: " << DegreeToVector.y << "\n";
				m_pThermalDetonator->getRigidBody()->velocity += DegreeToVector * (Speed * 2);
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
	m_pThermalDetonator->getRigidBody()->mass = Mass;
	addChild(m_pThermalDetonator);//

	m_pStormTroopers->getTransform()->position.x = m_pThermalDetonator->getTransform()->position.x + StormTrooperDistance;

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

	const SDL_Color green = { 0, 100, 0, 255 };

	std::string Text = "";
	Text = "Mass: " + std::to_string(m_pThermalDetonator->getRigidBody()->mass);
	MassLabel = new Label(Text, "Consolas", 15, green, glm::vec2(100.0f, 25.0f));
	MassLabel->setParent(this);
	addChild(MassLabel);

	Text = "Velocity: " + std::to_string(magnitude(m_pThermalDetonator->getRigidBody()->velocity));
	VelocityLabel = new Label(Text, "Consolas", 15, green, glm::vec2(100.0f, 50.0f));
	VelocityLabel->setParent(this);
	addChild(VelocityLabel);

	Text = "Acceleration: " + std::to_string(magnitude(m_pThermalDetonator->getRigidBody()->acceleration));
	AccelerationLabel = new Label(Text, "Consolas", 15, green, glm::vec2(100.0f, 75.0f));
	AccelerationLabel->setParent(this);
	addChild(AccelerationLabel);

	Text = "Position (x, y): (" + std::to_string(m_pThermalDetonator->getTransform()->position.x) + ", " + std::to_string(m_pThermalDetonator->getTransform()->position.y) + ")";
	PositionLabel = new Label(Text, "Consolas", 15, green, glm::vec2(175.0f, 100.0f));
	PositionLabel->setParent(this);
	addChild(PositionLabel);

	Text = "Force: " + std::to_string(magnitude(m_pThermalDetonator->Force));
	ForceLabel = new Label(Text, "Consolas", 15, green, glm::vec2(100.0f, 125.0f));
	ForceLabel->setParent(this);
	addChild(ForceLabel);

	float distance = magnitude(m_pStormTroopers->getTransform()->position - m_pThermalDetonator->getTransform()->position);

	Text = "Distance: " + std::to_string(distance);
	DistanceLabel = new Label(Text, "Consolas", 15, green, glm::vec2(100.0f, 150.0f));
	DistanceLabel->setParent(this);
	addChild(DistanceLabel);
}

float PlayScene::magnitude(glm::vec2 vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}

void PlayScene::SetText()
{
	std::string Text = "";
	Text = "Mass: " + std::to_string(m_pThermalDetonator->getRigidBody()->mass);
	MassLabel->setText(Text);
	Text = "Velocity: " + std::to_string(magnitude(m_pThermalDetonator->getRigidBody()->velocity));
	VelocityLabel->setText(Text);
	Text = "Acceleration: " + std::to_string(magnitude(m_pThermalDetonator->getRigidBody()->acceleration));
	AccelerationLabel->setText(Text);
	Text = "Position (x, y): (" + std::to_string(m_pThermalDetonator->getTransform()->position.x) + ", " + std::to_string(m_pThermalDetonator->getTransform()->position.y) + ")";
	PositionLabel->setText(Text);
	Text = "Force: " + std::to_string(magnitude(m_pThermalDetonator->Force));
	ForceLabel->setText(Text);
	float distance = magnitude(m_pStormTroopers->getTransform()->position - m_pThermalDetonator->getTransform()->position);
	float myRadius = m_pThermalDetonator->getWidth() * 0.5f;
	float otherRadius = m_pStormTroopers->getWidth() * 0.5f;
	if (distance <= myRadius + otherRadius)
	{
		Text = "Distance: Colliding";
	}
	else Text = "Distance: " + std::to_string(distance);
	DistanceLabel->setText(Text);
}
