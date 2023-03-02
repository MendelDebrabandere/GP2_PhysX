#pragma once
#include "GameScene.h"
class PhysXTestScene final: public GameScene
{
public:
	PhysXTestScene():GameScene(L"PhysXTestScene"){}
	~PhysXTestScene() override = default;
	PhysXTestScene(const PhysXTestScene& other) = delete;
	PhysXTestScene(PhysXTestScene&& other) noexcept = delete;
	PhysXTestScene& operator=(const PhysXTestScene& other) = delete;
	PhysXTestScene& operator=(PhysXTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;

private:

	void ResetScene();

	GameObject* m_pCube{};
	PxRigidDynamic* m_pCubeActor{};
};