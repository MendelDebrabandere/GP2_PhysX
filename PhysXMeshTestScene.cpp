#include "stdafx.h"
#include "PhysXMeshTestScene.h"

#include "ContentManager.h"
#include "Logger.h"
#include "MeshObject.h"
#include "PhysxManager.h"

void PhysXMeshTestScene::Initialize()
{
    EnablePhysxDebugRendering(true);

    auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
    auto pDefaultMaterial = pPhysX->createMaterial(.5f, .5f, .2f);

    //GROUNDPLANE
    auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ {PxPiDivTwo,{0,0,1} } });
    PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
    m_pPhysxScene->addActor(*pGroundActor);

    //Convex Chair
    m_pConvexChair = new MeshObject(L"Resources/Meshes/Chair.ovm");
    AddGameObject(m_pConvexChair);

    auto pConvexMesh = ContentManager::GetInstance()->Load<PxConvexMesh>(L"Resources/Meshes/Chair.ovpc");
    auto pConvexActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
    PxRigidActorExt::createExclusiveShape(*pConvexActor, PxConvexMeshGeometry{ pConvexMesh }, *pDefaultMaterial);
    auto centerOfMass = PxVec3{ 0.f, 5.f,0.f };
    PxRigidBodyExt::updateMassAndInertia(*pConvexActor, 10.f, &centerOfMass);

    m_pConvexChair->AttachRigidActor(pConvexActor);
    m_pConvexChair->Translate(0.f, 12.f, 0.f);
    m_pConvexChair->RotateDegrees(30.f, 0.f, 0.f);

    //Triangle Chair
    m_pTriangleChair = new MeshObject(L"Resources/Meshes/Chair.ovm");
    AddGameObject(m_pTriangleChair);

    const auto pTriangleMesh = ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Resources/Meshes/Chair.ovpt");
    
    //const auto pTriangleStatic = pPhysX->createRigidStatic(PxTransform{ PxIdentity });
    //PxRigidActorExt::createExclusiveShape(*pTriangleStatic, PxTriangleMeshGeometry{ pTriangleMesh }, *pDefaultMaterial);
    //m_pTriangleChair->AttachRigidActor(pTriangleStatic);

    m_pTriangleActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
    m_pTriangleActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);


    PxRigidActorExt::createExclusiveShape(*m_pTriangleActor, PxTriangleMeshGeometry{ pTriangleMesh }, *pDefaultMaterial);

   
	m_pTriangleChair->AttachRigidActor(m_pTriangleActor);
    m_pTriangleChair->Translate(10.f, 0.f, 0.f);

    m_KinematicPosition = { 10.f,0,0 };
}

void PhysXMeshTestScene::Update()
{
    if(m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
    {
        m_KinematicPosition.x -= 5.f * m_SceneContext.GetGameTime()->GetElapsed();
        m_pTriangleActor->setKinematicTarget(PxTransform{ m_KinematicPosition });
    }
}

void PhysXMeshTestScene::Draw() const
{

}

void PhysXMeshTestScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void PhysXMeshTestScene::OnSceneDeactivated()
{
}
