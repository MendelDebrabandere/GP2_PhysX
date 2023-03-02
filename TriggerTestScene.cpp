#include "stdafx.h"
#include "TriggerTestScene.h"

#include "ContentManager.h"
#include "Logger.h"
#include "MeshObject.h"
#include "PhysxManager.h"
#include "SpherePosColorNorm.h"

void TriggerTestScene::Initialize()
{
    EnablePhysxDebugRendering(true);

    const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
    const auto pDefaultMaterial = pPhysX->createMaterial(0.5f, 0.5f, 0.2f);

	//GROUNDPLANE
	auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ {PxPiDivTwo,{0,0,1} } });
    PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
    m_pPhysxScene->addActor(*pGroundActor);

    //SPHERE
    m_pSphere = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::Gray });
    AddGameObject(m_pSphere);

    auto pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
    PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ 1.f }, *pDefaultMaterial);

    m_pSphere->AttachRigidActor(pSphereActor);
    m_pSphere->Translate(0.0f, 2.f, 0.f);

    //TRIGGER LEFT
    m_pTriggerLeft = pPhysX->createRigidStatic(PxTransform{ -8, 0.5f, 0 });
    auto pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerLeft, PxBoxGeometry{ 0.5f, 0.5f, 0.5f }, *pDefaultMaterial);

    pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
    pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

    m_pPhysxScene->addActor(*m_pTriggerLeft);

    //TRIGGER RIGHT
    m_pTriggerRight = pPhysX->createRigidStatic(PxTransform{ 8, 0.5f, 0 });
    pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerRight, PxBoxGeometry{ 0.5f, 0.5f, 0.5f }, *pDefaultMaterial);

    pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
    pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

    m_pPhysxScene->addActor(*m_pTriggerRight);

}

void TriggerTestScene::Update()
{
    const float force{ 5.f };
    if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
    {
        m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0,force });
    }

    if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
    {
        m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0,-force });
    }

    if (m_IsLeftTriggered)
    {
        m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addForce({ 0,10,0 }, PxForceMode::eIMPULSE);

        m_IsLeftTriggered = false;
    }
}

void TriggerTestScene::Draw() const
{

}

void TriggerTestScene::OnSceneActivated()
{
    Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void TriggerTestScene::OnSceneDeactivated()
{
}

void TriggerTestScene::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
    for (auto i{0}; i < count; ++i)
    {
	    //ignore if deleted
        if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_OTHER | PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER))
            continue;

        const PxTriggerPair& pair = pairs[i];

        if (pair.triggerActor == m_pTriggerLeft)
        {
	        if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND) //ENTER
	        {
                Logger::GetInstance()->LogInfo(L"ENTERED Trigger LEFT");

                m_IsLeftTriggered = true;
	        }
            else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST) //LEFT
            {
                Logger::GetInstance()->LogInfo(L"LEFT Trigger LEFT");
            }
        }

        if (pair.triggerActor == m_pTriggerRight)
        {
            if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND) //ENTER
            {
                Logger::GetInstance()->LogInfo(L"ENTERED Trigger RIGHT");
            }
            else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST) //LEFT
            {
                Logger::GetInstance()->LogInfo(L"LEFT Trigger RIGHT");
            }
        }
    }
}
