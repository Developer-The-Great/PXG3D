#include "PXGGame.h"
#include "HitInfo.h"
#include "FreeMovementComponent.h"
#include "World.h"
#include "Canvas.h"
#include "FileConfig.h"
#include "ColorMaterial.h"
#include "TextureMaterial.h"
#include "LightComponent.h"

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
#include "PhysicsComponent.h"
#include "CameraMover.h"
#include "StandardLitMaterial.h"

#include "ConvexCollider.h"
#include "PhysicsCollider.h"



namespace PXG
{


	PXGGame::PXGGame() : Game()
	{
	}

	void PXGGame::Initialize()
	{
		AudioClip clip = AudioEngine::GetInstance().createClip(config::PXG_SOUND_PATH + "Music/Level_Music.streamed.wav");
		AudioEngine::GetInstance().Loop(true, clip);
		//AudioEngine::GetInstance().Play(clip);

		font = new Font(config::PXG_FONT_PATH + "Roboto-Regular.ttf", 20);

		Input::AddKeysToTrack(
			KeyCode::A, KeyCode::W, KeyCode::S, KeyCode::D, KeyCode::Q, KeyCode::E, KeyCode::K, KeyCode::J, KeyCode::Z,
			KeyCode::LeftMouse, KeyCode::RightMouse, KeyCode::MiddleMouse
			, KeyCode::Enter
			,KeyCode::KeyUp,KeyCode::KeyRight,KeyCode::KeyDown,KeyCode::KeyLeft);


		//---------------------------Initialize Textures---------------------------------------//
		std::shared_ptr<ColorMaterial> bluetColorMat = std::make_shared<ColorMaterial>(Vector3(0, 0, 1));
		std::shared_ptr<ColorMaterial> redColorMat = std::make_shared<ColorMaterial>(Vector3(1, 0, 0));
		std::shared_ptr<ColorMaterial> purpleColorMat = std::make_shared<ColorMaterial>(Vector3(1, 0, 1));
		std::shared_ptr<ColorMaterial> greenColorMat = std::make_shared<ColorMaterial>(Vector3(0, 1, 0));

		std::shared_ptr<TextureMaterial> textureMaterial = std::make_shared<TextureMaterial>();
		auto diamondMaterial = std::make_shared<StandardLitMaterial>();
		auto brickMaterial = std::make_shared<StandardLitMaterial>();



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
		auto raycaster = std::make_shared<RayCastShooter>();
		auto cameraRotator = std::make_shared<CameraRotator>();
		auto cameraMover = std::make_shared<CameraMover>(10.0f);
		
		GameObj cameraObj = Instantiate();
		cameraObj->name = "cameraObj";

		cameraObj->AddComponent(camera);
		cameraObj->AddComponent(cameraMover);
		cameraObj->AddComponent(raycaster);

		cameraObj->AddComponent(cameraRotator);

		world->AddToChildren(cameraObj);

		cameraObj->GetTransform()->SetLocalPosition(Vector3(0, 8, 4));
		cameraObj->GetTransform()->rotate(Vector3(1, 0, 0), -45.0f);
		cameraObj->AddComponent(std::make_shared<CameraRotator>());

		/*
		

		//--------------------------SetUp bottomFloor--------------------------------//

		GameObj brickFloor = Instantiate();
		brickFloor->name = "brick floor";

		brickFloor->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH +  "cube.obj");
		brickFloor->GetMeshComponent()->SetMaterial(brickMaterial);
		brickFloor->GetMeshComponent()->AddTextureToMeshAt(Texture(PXG::config::PXG_INDEPENDENT_TEXTURES_PATH + "bricks.jpg", TextureType::DIFFUSE), 0);
		brickFloor->GetTransform()->SetLocalPosition(Vector3(0, 0, 0));
		brickFloor->GetTransform()->Scale(Vector3(25, 0.5, 25));
		brickFloor->GetPhysicsComponent()->ConstructCollisionCube();


		world->AddToChildren(brickFloor);


		//-----------------------------setup left wall---------------------------------//
		
		GameObj leftBrickWall = Instantiate();
		leftBrickWall->name = "leftBrickWall";

		leftBrickWall->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "cube.obj");
		leftBrickWall->GetMeshComponent()->SetMaterial(brickMaterial);
		leftBrickWall->GetMeshComponent()->AddTextureToMeshAt(Texture(PXG::config::PXG_INDEPENDENT_TEXTURES_PATH + "concrete.png", TextureType::DIFFUSE), 0);
		leftBrickWall->GetTransform()->SetLocalPosition(Vector3(-25, 9, 16));
		leftBrickWall->GetTransform()->Scale(Vector3(9, 0.5, 9));
		leftBrickWall->GetTransform()->rotate(Vector3(0, 0, 1), 90);
		leftBrickWall->GetPhysicsComponent()->ConstructCollisionCube();


		world->AddToChildren(leftBrickWall);


		GameObj leftBrickWall2 = Instantiate();
		leftBrickWall->name = "leftBrickWall2";

		leftBrickWall2->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "cube.obj");
		leftBrickWall2->GetMeshComponent()->SetMaterial(brickMaterial);
		leftBrickWall2->GetMeshComponent()->AddTextureToMeshAt(Texture(PXG::config::PXG_INDEPENDENT_TEXTURES_PATH + "concrete.png", TextureType::DIFFUSE), 0);
		leftBrickWall2->GetTransform()->SetLocalPosition(Vector3(-25, 9, -2));
		leftBrickWall2->GetTransform()->Scale(Vector3(9, 0.5, 9));
		leftBrickWall2->GetTransform()->rotate(Vector3(0, 0, 1), 90);
		leftBrickWall2->GetPhysicsComponent()->ConstructCollisionCube();


		world->AddToChildren(leftBrickWall2);


		//-----------------------------setup left wall---------------------------------//


		//--------------------------SetUp middleCube--------------------------------//


		GameObj middleCube = Instantiate();
		middleCube->name = "midlle cube";
		middleCube->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "cube.obj");
		middleCube->GetMeshComponent()->SetMaterial(diamondMaterial);
		middleCube->GetMeshComponent()->AddTextureToMeshAt(Texture(PXG::config::PXG_INDEPENDENT_TEXTURES_PATH + "diffuse3.jpg", TextureType::DIFFUSE), 0);
		middleCube->GetTransform()->SetLocalPosition(Vector3(0, 2.5, 0));
		middleCube->GetTransform()->Scale(Vector3(2, 2, 2));
		middleCube->GetPhysicsComponent()->ConstructCollisionCube();
		world->AddToChildren(middleCube);

		//-----------------------Setup Diamond ------------------------------------//
		auto rotator = std::make_shared<RotatorComponent>(Vector3(0,1,0));

		GameObj diamond = Instantiate();
		diamond->name = "midlle cube";
		diamond->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "diamond.obj");
		diamond->GetMeshComponent()->SetMaterial(diamondMaterial);
		diamond->GetMeshComponent()->AddTextureToMeshAt(Texture(PXG::config::PXG_INDEPENDENT_TEXTURES_PATH + "bg-image.png", TextureType::DIFFUSE), 0);
		diamond->GetTransform()->SetLocalPosition(Vector3(0, 6.0, 0));
		diamond->GetTransform()->Scale(Vector3(0.5, 0.5, 0.5));
		diamond->GetPhysicsComponent()->ConstructPhysicsRepresentationFromMeshComponent();
		diamond->AddComponent(rotator);
		world->AddToChildren(diamond);

		GameObj smallDiamond = Instantiate();
		smallDiamond->name = "smallDiamond";
		smallDiamond->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "diamond.obj");
		smallDiamond->GetMeshComponent()->SetMaterial(diamondMaterial);
		smallDiamond->GetMeshComponent()->AddTextureToMeshAt(Texture(PXG::config::PXG_INDEPENDENT_TEXTURES_PATH + "bg-image.png", TextureType::DIFFUSE), 0);
		smallDiamond->GetTransform()->SetLocalPosition(Vector3(2.0, 0.0, 2.0));
		smallDiamond->GetTransform()->Scale(Vector3(0.5, 0.5, 0.5));
		smallDiamond->GetPhysicsComponent()->ConstructPhysicsRepresentationFromMeshComponent();

		diamond->AddToChildren(smallDiamond);



		//--------------------------setup lights----------------------------------------//
		//top spotlight

		auto lightComp = std::make_shared<LightComponent>();
		lightComp->SetIntensity(80.0f);
		lightComp->SetLightType(LightType::SPOTLIGHT);

		GameObj topSpotlight = Instantiate();
		topSpotlight->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "cube.obj");
		topSpotlight->GetMeshComponent()->SetMaterial(bluetColorMat);
		topSpotlight->name = "spotlight top";
		topSpotlight->AddComponent(lightComp);
		topSpotlight->GetTransform()->SetLocalPosition(Vector3(0.0, 18.0, 0.0));
		topSpotlight->GetTransform()->Scale(Vector3(0.5, 0.5, 0.5));
		world->AddToChildren(topSpotlight);

		auto leftlightComp = std::make_shared<LightComponent>();
		leftlightComp->SetIntensity(150.0f);
		GameObj leftPonintLight = Instantiate();
		leftPonintLight->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "cube.obj");
		leftPonintLight->GetMeshComponent()->SetMaterial(bluetColorMat);
		leftPonintLight->name = "leftPonintLight ";
		leftPonintLight->AddComponent(leftlightComp);
		leftPonintLight->GetTransform()->SetLocalPosition(Vector3(-12.0, 18.0, 0.0));
		leftPonintLight->GetTransform()->Scale(Vector3(0.5, 0.5, 0.5));
		world->AddToChildren(leftPonintLight);


		auto rightlightComp = std::make_shared<LightComponent>();
		rightlightComp->SetIntensity(150.0f);
		GameObj rightPonintLight = Instantiate();
		rightPonintLight->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "cube.obj");
		rightPonintLight->GetMeshComponent()->SetMaterial(bluetColorMat);
		rightPonintLight->name = "rightPonintLight";
		rightPonintLight->AddComponent(rightlightComp);
		rightPonintLight->GetTransform()->SetLocalPosition(Vector3(12.0, 18.0, 0.0));
		rightPonintLight->GetTransform()->Scale(Vector3(0.5, 0.5, 0.5));
		world->AddToChildren(rightPonintLight);

		//std::shared_ptr<PhysicsCollider> physicsCollider = std::make_shared<ConvexCollider>();
		//std::shared_ptr<PhysicsCollider> physicsCollider2 = std::make_shared<ConvexCollider>();

		//Manifold mdgsa;
		//physicsCollider->CheckCollision(physicsCollider2);
		//PhysicsCollider 

		*/
		



		//--------------------Physics Test OBB  --------------------------//
		

		///*
		
		GameObj OBBTestObjectPurple = Instantiate();
		OBBTestObjectPurple->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "_cube.obj");
		OBBTestObjectPurple->GetMeshComponent()->SetMaterial(purpleColorMat);
		OBBTestObjectPurple->GetPhysicsComponent()->ConstructPhysicsRepresentationFromMeshComponent();
		OBBTestObjectPurple->name = "OBBTestObjectPurple";
		
		OBBTestObjectPurple->GetTransform()->SetLocalPosition(Vector3(0.0, 0.0, -3.0));
		OBBTestObjectPurple->GetTransform()->Scale(Vector3(1.0, 1.0, 1.0));
		world->AddToChildren(OBBTestObjectPurple);

		GameObj OBBTestObjectGreen = Instantiate();
		OBBTestObjectGreen->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "_cube.obj");
		OBBTestObjectGreen->GetMeshComponent()->SetMaterial(greenColorMat);
		OBBTestObjectGreen->GetPhysicsComponent()->ConstructPhysicsRepresentationFromMeshComponent();
		OBBTestObjectGreen->name = "OBBTestObjectGreen";

		OBBTestObjectGreen->GetTransform()->SetLocalPosition(Vector3(3.0, 0.0, -3.0));
		OBBTestObjectGreen->GetTransform()->rotate(Vector3(1, 0, 0), 45.0f);
		OBBTestObjectGreen->GetTransform()->rotate(Vector3(0, 1, 0), 45.0f);
		OBBTestObjectGreen->GetTransform()->Scale(Vector3(1.0, 1.0, 1.0));
		world->AddToChildren(OBBTestObjectGreen);

		//*/

		//--------------------Physics Test AABB --------------------------//

		///*
		
		GameObj firstObj = Instantiate();
		firstObj->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "_cube.obj");
		firstObj->GetMeshComponent()->SetMaterial(redColorMat);
		firstObj->GetPhysicsComponent()->ConstructPhysicsRepresentationFromMeshComponent();
		firstObj->name = "firstObj";
		
		firstObj->GetTransform()->SetLocalPosition(Vector3(0.0, 0.0, 0.0));
		firstObj->GetTransform()->Scale(Vector3(1.0, 1.0, 1.0));
		world->AddToChildren(firstObj);

		GameObj secondObj = Instantiate();
		secondObj->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "_cube.obj");
		secondObj->GetMeshComponent()->SetMaterial(bluetColorMat);
		secondObj->GetPhysicsComponent()->ConstructPhysicsRepresentationFromMeshComponent();
		secondObj->name = "secondObj";

		secondObj->GetTransform()->SetLocalPosition(Vector3(3.0, 0.0, 0.0));
		secondObj->GetTransform()->Scale(Vector3(1.0, 1.0, 1.0));
		world->AddToChildren(secondObj);

		//*/

		









		world->SetPhysicsComponentDrawActive(true);


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
		world->Update();

	}

	void PXGGame::FixedUpdate(float tick)
	{
		//frender->restore_queue(&render_queue);
		world->FixedUpdate(tick);
		canvas->FixedUpdate(tick);
	}
}

