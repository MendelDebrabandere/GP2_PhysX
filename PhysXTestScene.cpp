#include "stdafx.h"
#include "PhysXTestScene.h"

#include "CubePosColorNorm.h"
#include "Logger.h"

void PhysXTestScene::Initialize()
{
	//Core::GetGameSettings().clearColor = static_cast<XMFLOAT3>(Colors::DarkGray);
	EnablePhysxDebugRendering(true);

	//CUBE ACTOR
	XMFLOAT3 actorDimensions{ 1.5f,1.f,1.f };
	m_pCube = new CubePosColorNorm(actorDimensions.x, actorDimensions.y, actorDimensions.z);
	AddGameObject(m_pCube);

	PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();
	m_pCubeActor = pPhysX->createRigidDynamic(PxTransform(PxIdentity));

	PxBoxGeometry boxGeometry = PxBoxGeometry(actorDimensions.x / 2.f, actorDimensions.y / 2.f, actorDimensions.z / 2.f);
	PxMaterial* pBoxMaterial = pPhysX->createMaterial(0.5f, 0.5f, 0.2f);

	//PxShape* pBoxShape = pPhysX->createShape(boxGeometry, *pBoxMaterial, true);
	//m_pCubeActor->attachShape(*pBoxShape);
	//OR
	PxShape* pBoxShape = PxRigidActorExt::createExclusiveShape(*m_pCubeActor, boxGeometry, &pBoxMaterial, 1);
	m_pCube->AttachRigidActor(m_pCubeActor);
	//Happens in ResetScene (after implementation)
	//m_pCube->RotateDegrees(40.f, 30.f, 20.f);
	//m_pCube->Translate(0.f, 5.f, 0.f);

	//GROUND PLANE
	PxRigidStatic* pGroundActor = pPhysX->createRigidStatic(PxTransform(PxQuat(PxPiDivTwo, PxVec3(0.f,0.f,1.f))));
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry(), &pBoxMaterial, 1);
	m_pPhysxScene->addActor(*pGroundActor);

	//CAMERA
	m_SceneContext.GetCamera()->SetPositionAndLookAt(XMFLOAT3(0.f, 5.f, -20.f), XMFLOAT3{});

	ResetScene();
}

void PhysXTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		ResetScene();
	}
}

void PhysXTestScene::Draw() const
{

}

void PhysXTestScene::ResetScene()
{
	m_pCube->RotateDegrees(40.f, 30.f, 20.f);
	m_pCube->Translate(0.f, 5.f, 0.f);

	//CAMERA
	m_SceneContext.GetCamera()->SetPositionAndLookAt(XMFLOAT3(0.f, 5.f, -20.f), XMFLOAT3{});
}

void PhysXTestScene::OnSceneActivated()
{
	Core::GetGameSettings().clearColor = XMFLOAT3(Colors::CornflowerBlue);
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
	Logger::GetInstance()->LogInfo(L"\t[INPUT > Reset=\'R\']");
	ResetScene();
}
