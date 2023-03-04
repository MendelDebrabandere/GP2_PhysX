#pragma once
#include "GameScene.h"
#include <vector>
class W2_AssignmentScene final : public GameScene
{
public:
	W2_AssignmentScene() :GameScene(L"W2_AssignmentScene") {}
	~W2_AssignmentScene() override = default;
	W2_AssignmentScene(const W2_AssignmentScene& other) = delete;
	W2_AssignmentScene(W2_AssignmentScene&& other) noexcept = delete;
	W2_AssignmentScene& operator=(const W2_AssignmentScene& other) = delete;
	W2_AssignmentScene& operator=(W2_AssignmentScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;

	void onTrigger(PxTriggerPair* pairs, PxU32 count) override;

private:
	GameObject* m_pLevel{};
	PxRigidStatic* m_pLevelActor{ nullptr };

	GameObject* m_pPlayerBall{};

	GameObject* m_pRedBox{};
	GameObject* m_pBlueBox{};

	PxRigidStatic* m_pTriggerLeft{ nullptr };
	PxRigidStatic* m_pTriggerRight{ nullptr };

	bool m_IsLeftTriggered{ false };
	bool m_IsRightTriggered{ false };

	PxRigidDynamic* m_pRedJointBox{};
	PxRigidDynamic* m_pBlueJointBox{};

	GameObject* m_LeftSphere{};
	GameObject* m_RightSphere{};

	FMOD::Sound* m_pBellSound{};
	FMOD::Channel* m_pChannel2D{ nullptr };
};

