#include "stdafx.h"
#include "BouncingBallScene.h"

#include "Logger.h"
#include "SpherePosColorNorm.h"

void BouncingBallScene::Initialize()
{
	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const auto pDefaultMaterial = pPhysX->createMaterial(1, 1, 1);

	std::vector<PxMaterial*> ballMaterials{};
	ballMaterials.push_back(pPhysX->createMaterial(1, 1, 1.f));
	ballMaterials.push_back(pPhysX->createMaterial(1, 1, 0.5f));
	ballMaterials.push_back(pPhysX->createMaterial(1, 1, 0));


	//GROUNDPLANE
	auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ {PxPiDivTwo,{0,0,1} } });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	//BALLS
	for (size_t i{}; i < 3; ++i)
	{
		m_BallsVec.push_back(new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::Gray }));
	}
	m_BallsVec[0]->Translate(5, 15, 0);
	m_BallsVec[1]->Translate(0, 15, 0);
	m_BallsVec[2]->Translate(-5, 15, 0);




	for (size_t i{}; i < 3; ++i)
	{
		auto pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
		PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ 1 }, *ballMaterials[i]);
		m_BallsVec[i]->AttachRigidActor(pSphereActor);
		AddGameObject(m_BallsVec[i]);
	}

}

void BouncingBallScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		Reset();
	}
}

void BouncingBallScene::Draw() const
{

}

void BouncingBallScene::OnSceneActivated()
{

}

void BouncingBallScene::Reset()
{
	m_BallsVec[0]->Translate(5, 5, 0);
	m_BallsVec[1]->Translate(0, 10, 0);
	m_BallsVec[2]->Translate(-5, 15, 0);
}
