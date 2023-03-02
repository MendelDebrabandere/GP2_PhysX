#pragma once
#include "GameScene.h"
class PhysXMeshTestScene final : public GameScene
{
public:
	PhysXMeshTestScene() :GameScene(L"TestScene") {}
	~PhysXMeshTestScene() override = default;
	PhysXMeshTestScene(const PhysXMeshTestScene& other) = delete;
	PhysXMeshTestScene(PhysXMeshTestScene&& other) noexcept = delete;
	PhysXMeshTestScene& operator=(const PhysXMeshTestScene& other) = delete;
	PhysXMeshTestScene& operator=(PhysXMeshTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	GameObject* m_pConvexChair{ nullptr };

	GameObject* m_pTriangleChair{ nullptr };
	PxRigidDynamic* m_pTriangleActor{ nullptr };
	PxVec3 m_KinematicPosition{};

};

