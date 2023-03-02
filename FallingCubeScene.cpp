#include "stdafx.h"
#include "FallingCubeScene.h"

#include "CubePosColorNorm.h"
#include "Logger.h"

void FallingCubeScene::Initialize()
{
	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const auto pDefaultMaterial = pPhysX->createMaterial(0.5f, 0.5f, 0.2f);

	//GROUNDPLANE
	auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ {PxPiDivTwo,{0,0,1} } });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	//CUBE
	m_pBox = new CubePosColorNorm(1.f, 3.f, 2.f);
	m_pBox->Translate(0, 10, 0);
	m_pBox->Rotate(1, 0, 0);
	auto pCubeActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pCubeActor, PxBoxGeometry{ 1.f/2, 3.f/2, 2.f/2 }, *pDefaultMaterial);

	m_pBox->AttachRigidActor(pCubeActor);
	AddGameObject(m_pBox);


}

void FallingCubeScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		Reset();
	}
}

void FallingCubeScene::Draw() const
{

}

void FallingCubeScene::OnSceneActivated()
{

}

void FallingCubeScene::Reset()
{
	m_pBox->Translate(0, 10, 0);
	m_pBox->Rotate(1, 0, 0);
}
