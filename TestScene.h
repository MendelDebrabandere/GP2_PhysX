#pragma once
#include "GameScene.h"
class TestScene final: public GameScene
{
public:
	TestScene():GameScene(L"TestScene"){}
	~TestScene() override = default;
	TestScene(const TestScene& other) = delete;
	TestScene(TestScene&& other) noexcept = delete;
	TestScene& operator=(const TestScene& other) = delete;
	TestScene& operator=(TestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;

private:

	GameObject* m_pTorus{};
	GameObject* m_pBox{};
	GameObject* m_pCylinder{};
};

