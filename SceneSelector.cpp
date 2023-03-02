#include "stdafx.h"
#include "SceneSelector.h"
#include "SceneManager.h"

//Change this define to activate/deactive the corresponding scenes
// W01 - W02  (#define ...)

#define W01
//#define W02

#ifdef W01
#include "W1_AssignmentScene.h"
#include "TestScene.h"
#include "FallingCubeScene.h"
#include "BouncingBallScene.h"
#endif

#ifdef W02
#include "PhysXMeshTestScene.h"
#include "AudioTestScene.h"
#include "TriggerTestScene.h"
#endif

namespace dae
{
	void AddScenes(SceneManager * pSceneManager)
	{

#ifdef W01
		pSceneManager->AddGameScene(new W1_AssignmentScene());
		pSceneManager->AddGameScene(new TestScene());
		pSceneManager->AddGameScene(new FallingCubeScene());
		pSceneManager->AddGameScene(new BouncingBallScene());
#endif

#ifdef W02
		pSceneManager->AddGameScene(new PhysXMeshTestScene());
		pSceneManager->AddGameScene(new TriggerTestScene());
		pSceneManager->AddGameScene(new AudioTestScene());
#endif

	}
}

