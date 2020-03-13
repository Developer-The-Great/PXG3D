#include "PXGGame.h"
#include "HitInfo.h"
#include "FreeMovementComponent.h"
#include "World.h"
#include "Canvas.h"
#include "FileConfig.h"
#include "ColorMaterial.h"
#include "TextureMaterial.h"

#include "CameraComponent.h"
#include "Texture.h"
#include "Input.h"
#include "KeyCode.h"


#include "TextComponent.h"
#include "ButtonComponent.h"

#include "RayCastShooter.h"

#include "Subject.h"
#include "Subscriber.h"

#include "ScreenSize.h"

#include "AudioClip.hpp"
#include "AudioEngine.hpp"
#include "AudioSource.h"

#include "RotatorComponent.h"
#include "CameraRotator.h"


namespace PXG
{


	PXGGame::PXGGame() : Game()
	{
	}

	void PXGGame::Initialize()
	{
		AudioClip clip = AudioEngine::GetInstance().createClip(config::PXG_SOUND_PATH + "Music/Level_Music.streamed.wav");
		AudioEngine::GetInstance().Loop(true, clip);
		AudioEngine::GetInstance().Play(clip);

		font = new Font(config::PXG_FONT_PATH + "Roboto-Regular.ttf", 20);

		Input::AddKeysToTrack(
			KeyCode::A, KeyCode::W, KeyCode::S, KeyCode::D, KeyCode::Q, KeyCode::E, KeyCode::K, KeyCode::J, KeyCode::Z,
			KeyCode::LeftMouse, KeyCode::RightMouse, KeyCode::MiddleMouse, KeyCode::Enter);


		//---------------------------Initialize Textures---------------------------------------//
		std::shared_ptr<ColorMaterial> bluetColorMat = std::make_shared<ColorMaterial>(Vector3(0, 0, 1));
		std::shared_ptr<TextureMaterial> textureMaterial = std::make_shared<TextureMaterial>();




		//--------------------------SetUpUICanvas--------------------------------//

		auto UICam = std::make_shared<CameraComponent>();
		GameObj UICanvasCam = InstantiateUIObject();
		canvas->AddToChildren(UICanvasCam);
		UICanvasCam->name = "UICAM";
		UICanvasCam->AddComponent(UICam);

		//half of game width and half of game height
		UICanvasCam->GetTransform()->SetLocalPosition(Vector3(ScreenSize::WIDTH / 2, ScreenSize::HEIGHT / 2, 0));
		Vector2 BGPos = Vector2(ScreenSize::WIDTH / 2, ScreenSize::HEIGHT / 2);


		//--------------------------SetUp Camera--------------------------------//
		auto camera = std::make_shared<CameraComponent>();
		auto movementComponent = std::make_shared<FreeMovementComponent>();
		auto raycaster = std::make_shared<RayCastShooter>();

		auto cameraRotator = std::make_shared<CameraRotator>();

		GameObj cameraObj = Instantiate();
		cameraObj->name = "cameraObj";

		cameraObj->AddComponent(camera);
		cameraObj->AddComponent(movementComponent);
		cameraObj->AddComponent(raycaster);

		cameraObj->AddComponent(cameraRotator);

		world->AddToChildren(cameraObj);

		cameraObj->GetTransform()->SetLocalPosition(Vector3(0, 0, 5));

		cameraObj->AddComponent(std::make_shared<CameraRotator>());


		//--------------------------SetUp Object--------------------------------//
		GameObj rotatingCube = Instantiate();
		rotatingCube->name = "rotatingCube";

		rotatingCube->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH +  "cube.obj");
		rotatingCube->GetMeshComponent()->SetMaterial(textureMaterial);
		rotatingCube->GetMeshComponent()->AddTextureToMeshAt(Texture(PXG::config::PXG_INDEPENDENT_TEXTURES_PATH + "bricks.jpg", TextureType::DIFFUSE), 0);
		rotatingCube->GetTransform()->SetLocalPosition(Vector3(0, 0, 0));
		rotatingCube->GetTransform()->Scale(Vector3(1, 1, 1));

		world->AddToChildren(rotatingCube);
		





	}

	GameObj PXGGame::MakeChild(const std::string& name)
	{
		GameObj go = Instantiate();
		go->name = name;
		world->AddToChildren(go);
		return go;
	}

	void PXGGame::Start()
	{
		world->Start();
		canvas->Start();
	}

	void PXGGame::Update()
	{
	}

	void PXGGame::FixedUpdate(float tick)
	{
		//frender->restore_queue(&render_queue);
		world->FixedUpdate(tick);
		canvas->FixedUpdate(tick);
	}
}

