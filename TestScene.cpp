#include "stdafx.h"
#include "TestScene.h"

#include "CubePosColorNorm.h"
#include "CylinderPosColorNorm.h"
#include "Logger.h"
#include "TorusPosColorNorm.h"

enum class KeyboardActions
{
	Up,
	Down,
	Left,
	Right
};

void TestScene::Initialize()
{
	//Exercise A1
	m_pTorus = new TorusPosColorNorm(8.f, 50, 1.f, 10, XMFLOAT4(Colors::Green));
	AddGameObject(m_pTorus);

	m_pBox = new CubePosColorNorm(5.f, 2.f, 1.f);
	AddGameObject(m_pBox);

	m_pCylinder = new CylinderPosColorNorm(1.5f, 10, 4.f, XMFLOAT4(Colors::Blue));
	m_pCylinder->Translate(0.f, 8.f, 0.f);
	AddGameObject(m_pCylinder);
	//m_pBox->AddChild(m_pTorus);

	m_SceneContext.GetCamera()->SetPosition(XMFLOAT3(0.f, 10.f, -25.f));
	m_SceneContext.GetCamera()->SetForward(XMFLOAT3(.0f, -.3f, 1.f));

	//Exercise B1 (Input)
	m_SceneContext.GetInput()->AddInputAction(InputAction{ int(KeyboardActions::Up), InputTriggerState::down, VK_UP });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ int(KeyboardActions::Down), InputTriggerState::down, VK_DOWN });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ int(KeyboardActions::Left), InputTriggerState::down, VK_LEFT });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ int(KeyboardActions::Right), InputTriggerState::down, VK_RIGHT });
}

void TestScene::Update()
{
	//Exercise A1
	m_pBox->Rotate(0.f, m_SceneContext.GetGameTime()->GetTotal() * XM_PIDIV2, 0.f);
	//m_pTorus->Rotate(m_SceneContext.GetGameTime()->GetTotal() * XM_PIDIV4, 0.f, 0.f);

	//Exercise B1 (Input)
	XMFLOAT3 translation{};
	const float movementSpeed{ 10.f * m_SceneContext.GetGameTime()->GetElapsed() };
	if(m_SceneContext.GetInput()->IsActionTriggered(int(KeyboardActions::Down)))
	{
		translation.y -= movementSpeed;
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(int(KeyboardActions::Up)))
	{
		translation.y += movementSpeed;
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(int(KeyboardActions::Right)))
	{
		translation.x += movementSpeed;
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(int(KeyboardActions::Left)))
	{
		translation.x -= movementSpeed;
	}

	XMStoreFloat3(&translation, XMVectorAdd(XMLoadFloat3(&m_pBox->GetPosition()), XMLoadFloat3(&translation)));
	m_pBox->Translate(translation.x, translation.y, translation.z);
}

void TestScene::Draw() const
{

}

void TestScene::OnSceneActivated()
{
	Core::GetGameSettings().clearColor = XMFLOAT3(Colors::CornflowerBlue);
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
	Logger::GetInstance()->LogInfo(L"\t[INPUT > Translate=\'Arrow Keys\']");

	m_SceneContext.GetCamera()->SetPosition(XMFLOAT3(0.f, 10.f, -25.f));
	m_SceneContext.GetCamera()->SetForward(XMFLOAT3(.0f, -.3f, 1.f));
}
