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

}

void W1_AssignmentScene::Update()
{

}

void W1_AssignmentScene::Draw() const
{

}

void W1_AssignmentScene::OnSceneActivated()
{

}


