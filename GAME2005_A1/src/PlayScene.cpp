#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance()->draw("background", 300.0f, 400.0f, 0, 255, true);
	TextureManager::Instance()->draw("stormtroopers", StormTrooperPos.x, StormTrooperPos.y, 0, 255, true);
	TextureManager::Instance()->draw("wookie", 100.0f, 400.0f, 0, 255, true);

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);

	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}
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
	if (m_pDetonator->doesUpdate)
	{
		SetText();
	}

	EventManager::Instance().update();

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
	TextureManager::Instance()->load("../Assets/textures/background.png", "background");
	TextureManager::Instance()->load("../Assets/textures/stormtroopers.png", "stormtroopers");
	TextureManager::Instance()->load("../Assets/textures/wookie.png", "wookie");

	StormTrooperPos.x = 610.0f;
	StormTrooperPos.y = 400.0f; //610.0f, 400.0f

	// Set GUI Title
	m_guiTitle = "Play Scene";
	
	// Player Sprite
	m_pDetonator = new ThermalDetonator();
	m_pDetonator->getTransform()->position = glm::vec2(125.0f, 400.0f);
	addChild(m_pDetonator);
	m_pDetonator->getRigidBody()->mass = 2.2f;
	m_pDetonator->pixelsPerMeter = 1.0f;
	m_pDetonator->Gravity = glm::vec2(0, 9.8f);
	m_pDetonator->calculateTheta = true;
	m_pDetonator->highThrow = true;
	m_pDetonator->Speed = 95.0f;
	m_pDetonator->throwAngle = 0.0f;

	CreateLabels();

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

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas", 20.0f, { 255, 255, 255, 255 });
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 550.0f);

	addChild(m_pInstructionsLabel);
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Edit Variables", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	if (ImGui::Button("Play"))
	{
		glm::vec2 DegreeToVector;
		if (m_pDetonator->calculateTheta)
		{
			glm::vec2 distance = StormTrooperPos.x - m_pDetonator->getTransform()->position;

			float gravityByDistance = m_pDetonator->Gravity.y * distance.x;
			float Equation = gravityByDistance / (m_pDetonator->Speed * m_pDetonator->Speed);
			float theta = 0.5 * glm::degrees(asin(Equation));

			if (m_pDetonator->highThrow)
				theta = 90.0f - theta;

			float cosTheta = cos(glm::radians(theta));
			float sinTheta = sin(glm::radians(theta));

			DegreeToVector = glm::vec2(cosTheta, -sinTheta);

			std::cout << "DIST: " << distance.x << " THETA: " << theta << "\n";
		}
		else DegreeToVector = glm::vec2(cos(m_pDetonator->throwAngle), -sin(m_pDetonator->throwAngle));

		std::cout << "Degree to vector: " << DegreeToVector.x << " y: " << DegreeToVector.y << "\n";
		m_pDetonator->getRigidBody()->velocity = DegreeToVector * m_pDetonator->Speed;
		m_pDetonator->doesUpdate = true;
		
	}

	if (ImGui::Button("Reset"))
	{
		m_pDetonator->doesUpdate = false;
		m_pDetonator->getTransform()->position = glm::vec2(125.0f, 400.0f);
		m_pDetonator->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pDetonator->getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);

		SetText();
	}

	ImGui::Separator();

	//mass, position, velocity, acceleration, force

	static float xPos = 485.0f;
	if (ImGui::SliderFloat("Storm Trooper Distance", &xPos, 125.0f, Config::SCREEN_WIDTH))
		StormTrooperPos.x = 125.0f + xPos;

	static float throwSpeed = 95.0f;
	if (ImGui::SliderFloat("Throw Speed", &throwSpeed, 0, 500))
		m_pDetonator->Speed = throwSpeed;

	ImGui::Checkbox("Calculate theta?", &m_pDetonator->calculateTheta);

	if (!m_pDetonator->calculateTheta)
		ImGui::SliderAngle("Set Angle", &m_pDetonator->throwAngle, 0.0f, 90.0f);
	else
		ImGui::Checkbox("Throw at high angle?", &m_pDetonator->highThrow);
	
	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

void PlayScene::SetText()
{
	std::string Text = "";
	Text = "Mass: " + std::to_string(m_pDetonator->getRigidBody()->mass);
	MassLabel->setText(Text);

	Text = "Position (x, y): (" + std::to_string(m_pDetonator->getTransform()->position.x) + ", " + std::to_string(m_pDetonator->getTransform()->position.y) + ")";
	PositionLabel->setText(Text);

	Text = "Velocity: " + std::to_string(magnitude(m_pDetonator->getRigidBody()->velocity));
	VelocityLabel->setText(Text);

	Text = "Acceleration: " + std::to_string(magnitude(m_pDetonator->getRigidBody()->acceleration));
	AccelerationLabel->setText(Text);

	Text = "Force: " + std::to_string(magnitude(m_pDetonator->Force));
	ForceLabel->setText(Text);
}

void PlayScene::CreateLabels()
{
	const SDL_Color green = { 0, 100, 0, 255 };

	std::string Text = "";
	Text = "Mass: " + std::to_string(m_pDetonator->getRigidBody()->mass);
	MassLabel = new Label(Text, "Consolas", 15, green, glm::vec2(100.0f, 25.0f));
	MassLabel->setParent(this);
	addChild(MassLabel);

	Text = "Position (x, y): (" + std::to_string(m_pDetonator->getTransform()->position.x) + ", " + std::to_string(m_pDetonator->getTransform()->position.y) + ")";
	PositionLabel = new Label(Text, "Consolas", 15, green, glm::vec2(175.0f, 100.0f));
	PositionLabel->setParent(this);
	addChild(PositionLabel);

	Text = "Velocity: " + std::to_string(magnitude(m_pDetonator->getRigidBody()->velocity));
	VelocityLabel = new Label(Text, "Consolas", 15, green, glm::vec2(100.0f, 50.0f));
	VelocityLabel->setParent(this);
	addChild(VelocityLabel);

	Text = "Acceleration: " + std::to_string(magnitude(m_pDetonator->getRigidBody()->acceleration));
	AccelerationLabel = new Label(Text, "Consolas", 15, green, glm::vec2(100.0f, 75.0f));
	AccelerationLabel->setParent(this);
	addChild(AccelerationLabel);

	Text = "Force: " + std::to_string(magnitude(m_pDetonator->Force));
	ForceLabel = new Label(Text, "Consolas", 15, green, glm::vec2(100.0f, 125.0f));
	ForceLabel->setParent(this);
	addChild(ForceLabel);
}

float PlayScene::magnitude(glm::vec2 vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}
