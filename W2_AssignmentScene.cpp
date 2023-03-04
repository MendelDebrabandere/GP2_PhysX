#include "stdafx.h"
#include "W2_AssignmentScene.h"

#include "ContentManager.h"
#include "CubePosColorNorm.h"
#include "Logger.h"
#include "MeshObject.h"
#include "SoundManager.h"
#include "SpherePosColorNorm.h"
#include "extensions/PxRevoluteJoint.h"
#include "extensions/PxRevoluteJoint.h"

void W2_AssignmentScene::Initialize()
{
    const XMFLOAT3 hatchSize{ 2.0f, 0.2f, 6.0f };
    const XMFLOAT4 redColor{ 1.0f, 0.0f, 0.f, 1.0f };
    const XMFLOAT4 blueColor{ 0.0f, 0.0f, 1.f, 1.0f };

    EnablePhysxDebugRendering(true);

    auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
    auto pDefaultMaterial = pPhysX->createMaterial(.5f, .5f, .2f);

    
    { //GROUNDPLANE
        auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ {PxPiDivTwo,{0,0,1} } });
        PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
        m_pPhysxScene->addActor(*pGroundActor);
    }

    
    { //Triangle Level
        m_pLevel = new MeshObject(L"Resources/Meshes/Level.ovm");
        AddGameObject(m_pLevel);
        const auto pLevelMesh = ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Resources/Meshes/Level.ovpt");
        m_pLevelActor = pPhysX->createRigidStatic(PxTransform{ PxIdentity });
        PxRigidActorExt::createExclusiveShape(*m_pLevelActor, PxTriangleMeshGeometry{ pLevelMesh }, *pDefaultMaterial);
        m_pLevel->AttachRigidActor(m_pLevelActor);
    }

    { //Player ball
        m_pPlayerBall = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::Gray });
        m_pPlayerBall->Translate(0, 4, 0);

        auto pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
        PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ 1 }, *pDefaultMaterial);
        PxRigidBodyExt::setMassAndUpdateInertia(*pSphereActor, 100);
        pSphereActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, true);
        m_pPlayerBall->AttachRigidActor(pSphereActor);

        AddGameObject(m_pPlayerBall);
    }
    
    { //Boxes
        m_pRedBox = new CubePosColorNorm(1.5f, 1.5f, 1.5f, XMFLOAT4(Colors::Red));
        m_pRedBox->Translate(-4, 4, 0);
        auto pCubeActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
        PxRigidActorExt::createExclusiveShape(*pCubeActor, PxBoxGeometry{ 1.5f / 2, 1.5f / 2, 1.5f / 2 }, *pDefaultMaterial);
        pCubeActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, true);
        m_pRedBox->AttachRigidActor(pCubeActor);
        AddGameObject(m_pRedBox);

        m_pBlueBox = new CubePosColorNorm(1.5f, 1.5f, 1.5f, XMFLOAT4(Colors::Blue));
        m_pBlueBox->Translate(4, 4, 0);
        pCubeActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
        PxRigidActorExt::createExclusiveShape(*pCubeActor, PxBoxGeometry{ 1.5f / 2, 1.5f / 2, 1.5f / 2 }, *pDefaultMaterial);
        pCubeActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, true);
        m_pBlueBox->AttachRigidActor(pCubeActor);
        AddGameObject(m_pBlueBox);
    }
    
    { //TRIGGER LEFT
        m_pTriggerLeft = pPhysX->createRigidStatic(PxTransform{ -7.3f, 2.6f, 0 });
        auto pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerLeft, PxBoxGeometry{ 0.7f, 0.7f, 2.f }, *pDefaultMaterial);

        pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
        pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

        m_pPhysxScene->addActor(*m_pTriggerLeft);

    	//TRIGGER RIGHT
        m_pTriggerRight = pPhysX->createRigidStatic(PxTransform{ 6.6f, 2.6f, 0 });
        pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerRight, PxBoxGeometry{ 0.7f, 0.7f, 2.f }, *pDefaultMaterial);

        pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
        pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

        m_pPhysxScene->addActor(*m_pTriggerRight);
    }

    { //LEFT JOINT
        auto pRedJoint{ new CubePosColorNorm{ hatchSize.x, hatchSize.y, hatchSize.z, redColor } };
        AddGameObject(pRedJoint);

        m_pRedJointBox = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
        m_pRedJointBox->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
        PxRigidActorExt::createExclusiveShape(*m_pRedJointBox, PxBoxGeometry{ hatchSize.x / 2.0f, hatchSize.y / 2.0f, hatchSize.z / 2.0f }, *pDefaultMaterial);

        pRedJoint->AttachRigidActor(m_pRedJointBox);

        pRedJoint->Translate(-9.0f, 17.0f, 0.0f);

        PxTransform localFrame{ PxVec3{ 1.0f, 0.0f, 0.0f }, PxQuat{ PxHalfPi, PxVec3{ 0.0f, 1.0f, 0.0f } } };
        PxTransform globalFrame{ PxVec3{ -9.0f, 17.0f, 0.0f } };
        PxTransform jointFrame = globalFrame * localFrame;
        PxRevoluteJointCreate(*pPhysX,
            m_pRedJointBox, localFrame,
            NULL, jointFrame);
    }

    { //RIGHT JOINT
        auto pBlueJoint{ new CubePosColorNorm{ hatchSize.x, hatchSize.y, hatchSize.z, blueColor } };
        AddGameObject(pBlueJoint);

        m_pBlueJointBox = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
        m_pBlueJointBox->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
        PxRigidActorExt::createExclusiveShape(*m_pBlueJointBox, PxBoxGeometry{ hatchSize.x / 2.0f, hatchSize.y / 2.0f, hatchSize.z / 2.0f }, *pDefaultMaterial);

        pBlueJoint->AttachRigidActor(m_pBlueJointBox);

        pBlueJoint->Translate(9.0f, 17.0f, 0.0f);

        PxTransform localFrame{ PxVec3(-1.0f, 0.0f, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 1.0f, 0.0f)) };
        PxTransform globalFrame{ PxVec3(9.0f, 17.0f, 0.0f) };
        PxTransform jointFrame = globalFrame * localFrame;
        PxRevoluteJointCreate(*pPhysX,
            m_pBlueJointBox, localFrame,
            NULL, jointFrame);
    }

    { //SPHERES ON TOP
        m_LeftSphere = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::Gray });
        m_LeftSphere->Translate(-2, 23, 0);

        auto pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
        PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ 1 }, * pDefaultMaterial);
        PxRigidBodyExt::setMassAndUpdateInertia(*pSphereActor, 100);
        pSphereActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, true);
        m_LeftSphere->AttachRigidActor(pSphereActor);

        AddGameObject(m_LeftSphere);

        m_RightSphere = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::Gray });
        m_RightSphere->Translate(2, 23, 0);

        pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
        PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ 1 }, * pDefaultMaterial);
        PxRigidBodyExt::setMassAndUpdateInertia(*pSphereActor, 100);
        pSphereActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, true);
        m_RightSphere->AttachRigidActor(pSphereActor);

        AddGameObject(m_RightSphere);
    }

    { //SOUND 2D
        auto pFmod = SoundManager::GetInstance()->GetSystem();

        FMOD_RESULT result = pFmod->createStream("Resources/sounds/bell.mp3", FMOD_DEFAULT, nullptr, &m_pBellSound);
        SoundManager::GetInstance()->ErrorCheck(result);
    }
}

void W2_AssignmentScene::Update()
{
    constexpr float moveSpeed{ 15.f };
    constexpr float jumpForce{ 800.f };
    if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
    {
        m_pPlayerBall->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0,moveSpeed }, PxForceMode::eACCELERATION);
    }

    if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
    {
        m_pPlayerBall->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0,-moveSpeed }, PxForceMode::eACCELERATION);
    }

    if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, VK_SPACE))
    {
        m_pPlayerBall->GetRigidActor()->is<PxRigidDynamic>()->addForce({ 0,jumpForce,0 }, PxForceMode::eIMPULSE);
    }


    if (m_IsLeftTriggered)
    {
        m_pRedJointBox->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
        m_pRedJointBox->addForce({ 0.0f, -10.0f, 0.0f });
        m_pChannel2D->setPaused(false);
        m_IsLeftTriggered = false;
    }
    if (m_IsRightTriggered)
    {
        m_pBlueJointBox->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
        m_pBlueJointBox->addForce({ 0.0f, -10.0f, 0.0f });
        m_pChannel2D->setPaused(false);
        m_IsRightTriggered = false;
    }
}

void W2_AssignmentScene::Draw() const
{

}

void W2_AssignmentScene::OnSceneActivated()
{

}

void W2_AssignmentScene::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
    for (auto i{ 0 }; i < count; ++i)
    {
        //ignore if deleted
        if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_OTHER | PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER))
            continue;

        const PxTriggerPair& pair = pairs[i];
        
        if (pair.triggerActor == m_pTriggerLeft && pair.otherActor == m_pRedBox->GetRigidActor())
        {
            if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND) //ENTER
            {
                Logger::GetInstance()->LogInfo(L"Red box entered the trigger");
                m_IsLeftTriggered = true;

                FMOD_RESULT result = SoundManager::GetInstance()->GetSystem()->playSound(m_pBellSound, nullptr, false, &m_pChannel2D);
                SoundManager::GetInstance()->ErrorCheck(result);
            }
            else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST) //LEFT
            {
                m_IsLeftTriggered = false;
            }
        }

        if (pair.triggerActor == m_pTriggerRight && pair.otherActor == m_pBlueBox->GetRigidActor())
        {
            if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND) //ENTER
            {
                Logger::GetInstance()->LogInfo(L"Blue box entered the trigger");
                m_IsRightTriggered = true;

                FMOD_RESULT result = SoundManager::GetInstance()->GetSystem()->playSound(m_pBellSound, nullptr, false, &m_pChannel2D);
                SoundManager::GetInstance()->ErrorCheck(result);
            }
            else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST) //LEFT
            {
                m_IsRightTriggered = false;
            }
        }
    }
}