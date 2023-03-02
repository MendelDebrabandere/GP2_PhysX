#pragma once
#include "GameScene.h"
class AudioTestScene final : public GameScene
{
public:
	AudioTestScene() :GameScene(L"TestScene") {}
	~AudioTestScene() override = default;
	AudioTestScene(const AudioTestScene& other) = delete;
	AudioTestScene(AudioTestScene&& other) noexcept = delete;
	AudioTestScene& operator=(const AudioTestScene& other) = delete;
	AudioTestScene& operator=(AudioTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	GameObject* m_pSphere{ nullptr };

	enum InputIds
	{
		Play2D,
		Play3D,
		IncreaseVolume,
		DecreaseVolume
	};

	FMOD::Channel* m_pChannel2D{ nullptr };
	FMOD::Channel* m_pChannel3D{ nullptr };

	FMOD_VECTOR m_PrevListenerPos{};
};

