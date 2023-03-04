#include "stdafx.h"
#include "W1_AssignmentScene.h"

#include "CubePosColorNorm.h"
#include "Logger.h"
#include "SpherePosColorNorm.h"

void W1_AssignmentScene::Initialize()
{
	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const auto pDefaultMaterial = pPhysX->createMaterial(0.5f, 0.5f, 0.2f);

	//GROUNDPLANE
	auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ {PxPiDivTwo,{0,0,1} } });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	//WALL
	for (int x{}; x < 10; ++x)
	{
		for (int y{}; y < 10; ++y)
		{
			m_Wall.push_back(new CubePosColorNorm(1.f, 1.f, 1.f));
			m_Wall[x * 10 + y]->Translate(-6.f + x * 1.1f, 0.5f + y * 1.0f, 10);
			m_Wall[x * 10 + y]->RotateDegrees(0, rand() % 20 - 10, 0);
			auto pCubeActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
			PxRigidActorExt::createExclusiveShape(*pCubeActor, PxBoxGeometry{ 1.f / 2, 1.f / 2, 1.f / 2 }, *pDefaultMaterial);

			m_Wall[x * 10 + y]->AttachRigidActor(pCubeActor);

			AddGameObject(m_Wall[x * 10 + y]);
		}
	}

	//SPHERE
	m_Ball = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::Gray });
	m_Ball->Translate(0, 10, -20);

	auto pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ 1 }, *pDefaultMaterial);
	PxRigidBodyExt::setMassAndUpdateInertia(*pSphereActor, 100);
	m_Ball->AttachRigidActor(pSphereActor);

	AddGameObject(m_Ball);


	

	//CAMERA
	m_SceneContext.GetCamera()->SetPositionAndLookAt(XMFLOAT3(-30.f, 10.f, 0.f), XMFLOAT3{0,3.14,0});

}

void W1_AssignmentScene::Update()
{
	const float moveSpeed{ 5.f };
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_Ball->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ moveSpeed,0,0 }, PxForceMode::eACCELERATION);
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
	{
		m_Ball->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ -moveSpeed,0,0 }, PxForceMode::eACCELERATION);
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_UP))
	{
		m_Ball->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0,-moveSpeed }, PxForceMode::eACCELERATION);
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_DOWN))
	{
		m_Ball->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0,moveSpeed }, PxForceMode::eACCELERATION);
	}
}

void W1_AssignmentScene::Draw() const
{

}

void W1_AssignmentScene::OnSceneActivated()
{

}


