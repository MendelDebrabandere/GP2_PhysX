#include "stdafx.h"
#include "AudioTestScene.h"

#include "ContentManager.h"
#include "Logger.h"
#include "MeshObject.h"
#include "PhysxManager.h"
#include "SoundManager.h"
#include "SpherePosColorNorm.h"

void AudioTestScene::Initialize()
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

    //SOUND 2D
    auto pFmod = SoundManager::GetInstance()->GetSystem();

    FMOD::Sound* pSound{};
    FMOD_RESULT result = pFmod->createStream("Resources/sounds/wave.mp3", FMOD_DEFAULT | FMOD_LOOP_NORMAL, nullptr, &pSound);
    SoundManager::GetInstance()->ErrorCheck(result);

    result = pFmod->playSound(pSound, nullptr, true, &m_pChannel2D);
    SoundManager::GetInstance()->ErrorCheck(result);

    //SOUND 3D
    result = pFmod->createStream("Resources/sounds/thrones.mp3", FMOD_3D | FMOD_3D_LINEARROLLOFF, nullptr, &pSound);
    SoundManager::GetInstance()->ErrorCheck(result);

    result = pFmod->playSound(pSound, nullptr, true, &m_pChannel3D);
    SoundManager::GetInstance()->ErrorCheck(result);

    m_pChannel3D->setMode(FMOD_LOOP_NORMAL);
    m_pChannel3D->set3DMinMaxDistance(0, 100);

    //INPUT
    auto pInput = m_SceneContext.GetInput();
    pInput->AddInputAction({ Play2D, InputTriggerState::pressed, VK_SPACE });
    pInput->AddInputAction({ Play3D, InputTriggerState::pressed, 'b' });
    pInput->AddInputAction({ IncreaseVolume, InputTriggerState::pressed, VK_UP });
    pInput->AddInputAction({ DecreaseVolume, InputTriggerState::pressed, VK_DOWN });
}

inline FMOD_VECTOR ToFmod(XMFLOAT3 v)
{
    return FMOD_VECTOR(v.x, v.y, v.z);
}

inline FMOD_VECTOR ToFmod(PxVec3 v)
{
    return FMOD_VECTOR(v.x, v.y, v.z);
}

void AudioTestScene::Update()
{
    //SPHERE MOVEMENT
    const float force{ 5.f };
    if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
    {
        m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0,force });
    }

    if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
    {
        m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0,-force });
    }

    //SOUND CONTROLS
    if (m_SceneContext.GetInput()->IsActionTriggered(Play2D))
    {
        bool bPaused = false;
        m_pChannel2D->getPaused(&bPaused);
    	m_pChannel2D->setPaused(!bPaused);
    }
    if (m_SceneContext.GetInput()->IsActionTriggered(Play3D))
    {
        bool bPaused = false;
        m_pChannel3D->getPaused(&bPaused);
        m_pChannel3D->setPaused(!bPaused);
    }
    if (m_SceneContext.GetInput()->IsActionTriggered(IncreaseVolume))
    {
        float volume{};
        m_pChannel2D->getVolume(&volume);
       
        volume += 0.1f;

        m_pChannel2D->setVolume(volume);
        Logger::GetInstance()->LogFormat(LogLevel::Info, L"Sound Volume Changed > %f", volume);

    }


    //3D sound config
    auto pCam = m_SceneContext.GetCamera();
    auto pos = ToFmod(pCam->GetPosition());
    auto forward = ToFmod(pCam->GetForward());
    auto up = ToFmod(pCam->GetUp());

    FMOD_VECTOR vel{};
    auto deltaT = m_SceneContext.GetGameTime()->GetElapsed();
    vel.x = (pos.x - m_PrevListenerPos.x / deltaT);
    vel.y = (pos.y - m_PrevListenerPos.y / deltaT);
    vel.z = (pos.z - m_PrevListenerPos.z / deltaT);
    m_PrevListenerPos = pos;

    SoundManager::GetInstance()->GetSystem()->set3DListenerAttributes(0, &pos, &vel, &forward, &up);

    //Source
    auto spherePos = ToFmod(m_pSphere->GetPosition());
    auto sphereVel = ToFmod(m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->getLinearVelocity());
    m_pChannel3D->set3DAttributes(&spherePos, &sphereVel);
}

void AudioTestScene::Draw() const
{

}

void AudioTestScene::OnSceneActivated()
{
    Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void AudioTestScene::OnSceneDeactivated()
{
}
