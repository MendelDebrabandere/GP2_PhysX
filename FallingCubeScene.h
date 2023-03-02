#pragma once
#include "GameScene.h"
class FallingCubeScene final : public GameScene
{
public:
	FallingCubeScene() :GameScene(L"FallingCubeScene") {}
	~FallingCubeScene() override = default;
	FallingCubeScene(const FallingCubeScene& other) = delete;
	FallingCubeScene(FallingCubeScene&& other) noexcept = delete;
	FallingCubeScene& operator=(const FallingCubeScene& other) = delete;
	FallingCubeScene& operator=(FallingCubeScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;

private:
	void Reset();

	GameObject* m_pBox{};
};

