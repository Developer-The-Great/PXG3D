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

#include "AABBBox.h"
#include "DebugDrawingManager.h"

#include "PhysicsCollider.h"
#include "ConvexCollider.h"
#include "Time.h"
#include "Rigidbody.h"
#include "ForceAdderTestComponent.h"

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
			KeyCode::A, KeyCode::W, KeyCode::S, KeyCode::D, KeyCode::Q, KeyCode::E, KeyCode::K, KeyCode::J, KeyCode::Z,KeyCode::F, KeyCode::N, KeyCode::B,
			KeyCode::LeftMouse, KeyCode::RightMouse, KeyCode::MiddleMouse
			, KeyCode::Enter
			,KeyCode::KeyUp,KeyCode::KeyRight,KeyCode::KeyDown,KeyCode::KeyLeft
			, KeyCode::X, KeyCode::Y, KeyCode::Z,KeyCode::T
			,KeyCode::KP0, KeyCode::KP2);

		//---------------------------Initialize Textures---------------------------------------//
		std::shared_ptr<ColorMaterial> bluetColorMat = std::make_shared<ColorMaterial>(Vector3(0, 0, 1));
		std::shared_ptr<ColorMaterial> redColorMat = std::make_shared<ColorMaterial>(Vector3(1, 0, 0));
		std::shared_ptr<ColorMaterial> purpleColorMat = std::make_shared<ColorMaterial>(Vector3(1, 0, 1));
		std::shared_ptr<ColorMaterial> greenColorMat = std::make_shared<ColorMaterial>(Vector3(0, 1, 0));
		std::shared_ptr<ColorMaterial> yellowColorMat = std::make_shared<ColorMaterial>(Vector3(0.8, 0.8, 0));
		std::shared_ptr<ColorMaterial> orangeColorMat = std::make_shared<ColorMaterial>(Vector3(1, 69.0f/255.0f, 0));
		std::shared_ptr<ColorMaterial> cremeColorMat = std::make_shared<ColorMaterial>(Vector3(1.0, 253.0f / 255.0f, 208.0f/ 255.0f));
		std::shared_ptr<ColorMaterial> magentaColorMat = std::make_shared<ColorMaterial>(Vector3(139.0f / 255.0f, 0.0f / 255.0f, 138.0f / 255.0f));
		std::shared_ptr<ColorMaterial> slateGrayColorMat = std::make_shared<ColorMaterial>(Vector3(112.0f / 255.0f, 128.0f / 255.0f, 144.0f / 255.0f));
		//255, 253, 208
		//129

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
		auto cameraMover = std::make_shared<CameraMover>(0.5f);
		
		GameObj cameraObj = Instantiate();
		cameraObj->name = "cameraObj";

		cameraObj->AddComponent(camera);
		cameraObj->AddComponent(cameraMover);
		cameraObj->AddComponent(raycaster);

		cameraObj->AddComponent(cameraRotator);

		world->AddToChildren(cameraObj);

		cameraObj->GetTransform()->SetLocalPosition(Vector3(0, 8, 4));
		cameraObj->GetTransform()->rotate(Vector3(1, 0, 0), -45.0f);
		
		/*
		////--------------------------SetUp ceiling--------------------------------//

		GameObj upperFloor = Instantiate();
		upperFloor->name = "brick floor";

		upperFloor->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "cube.obj");
		upperFloor->GetMeshComponent()->SetMaterial(brickMaterial);
		upperFloor->GetMeshComponent()->AddTextureToMeshAt(Texture(PXG::config::PXG_INDEPENDENT_TEXTURES_PATH + "bricks.jpg", TextureType::DIFFUSE), 0);
		upperFloor->GetTransform()->SetLocalPosition(Vector3(0,30, 0));
		upperFloor->GetTransform()->Scale(Vector3(26,0.5, 26));
		upperFloor->GetPhysicsComponent()->ConstructCollisionCube();


		world->AddToChildren(upperFloor);

		////--------------------------SetUp BackWall--------------------------------//

		GameObj backFloor = Instantiate();
		backFloor->name = "brick floor";

		backFloor->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "cube.obj");
		backFloor->GetMeshComponent()->SetMaterial(brickMaterial);
		backFloor->GetMeshComponent()->AddTextureToMeshAt(Texture(PXG::config::PXG_INDEPENDENT_TEXTURES_PATH + "bricks.jpg", TextureType::DIFFUSE), 0);
		backFloor->GetTransform()->SetLocalPosition(Vector3(0, 10, 25));
		backFloor->GetTransform()->Scale(Vector3(26, 0.5, 26));
		backFloor->GetPhysicsComponent()->ConstructCollisionCube();
		backFloor->GetTransform()->rotate(Vector3(1, 0, 0), 90);

		world->AddToChildren(backFloor);

		////--------------------------SetUp FrontWall--------------------------------//

		GameObj frontFloor = Instantiate();
		frontFloor->name = "brick floor";

		frontFloor->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "cube.obj");
		frontFloor->GetMeshComponent()->SetMaterial(brickMaterial);
		frontFloor->GetMeshComponent()->AddTextureToMeshAt(Texture(PXG::config::PXG_INDEPENDENT_TEXTURES_PATH + "bricks.jpg", TextureType::DIFFUSE), 0);
		frontFloor->GetTransform()->SetLocalPosition(Vector3(0, 10, -25));
		frontFloor->GetTransform()->Scale(Vector3(26, 0.5, 26));
		frontFloor->GetPhysicsComponent()->ConstructCollisionCube();
		frontFloor->GetTransform()->rotate(Vector3(1, 0,0), 90);

		world->AddToChildren(frontFloor);
		

		////--------------------------SetUp bottomFloor--------------------------------//

		GameObj brickFloor = Instantiate();
		brickFloor->name = "brick floor";

		brickFloor->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH +  "cube.obj");
		brickFloor->GetMeshComponent()->SetMaterial(brickMaterial);
		brickFloor->GetMeshComponent()->AddTextureToMeshAt(Texture(PXG::config::PXG_INDEPENDENT_TEXTURES_PATH + "concrete.png", TextureType::DIFFUSE), 0);
		brickFloor->GetTransform()->SetLocalPosition(Vector3(0, 0, 0));
		brickFloor->GetTransform()->Scale(Vector3(26, 0.5, 26));
		brickFloor->GetPhysicsComponent()->ConstructCollisionCube();


		world->AddToChildren(brickFloor);

		//-----------------------------setup right wall---------------------------------//
		GameObj rightBrickWall = Instantiate();
		rightBrickWall->name = "rightBrickWall";

		rightBrickWall->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "cube.obj");
		rightBrickWall->GetMeshComponent()->SetMaterial(brickMaterial);
		rightBrickWall->GetMeshComponent()->AddTextureToMeshAt(Texture(PXG::config::PXG_INDEPENDENT_TEXTURES_PATH + "concrete.png", TextureType::DIFFUSE), 0);
		rightBrickWall->GetTransform()->SetLocalPosition(Vector3(25, 10, 0));
		rightBrickWall->GetTransform()->Scale(Vector3(20, 0.5, 25));
		rightBrickWall->GetTransform()->rotate(Vector3(0, 0, 1), 90);
		rightBrickWall->GetPhysicsComponent()->ConstructCollisionCube();


		world->AddToChildren(rightBrickWall);


		//-----------------------------setup left wall---------------------------------//
		
		GameObj leftBrickWall = Instantiate();
		leftBrickWall->name = "leftBrickWall";

		leftBrickWall->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "cube.obj");
		leftBrickWall->GetMeshComponent()->SetMaterial(brickMaterial);
		leftBrickWall->GetMeshComponent()->AddTextureToMeshAt(Texture(PXG::config::PXG_INDEPENDENT_TEXTURES_PATH + "concrete.png", TextureType::DIFFUSE), 0);
		leftBrickWall->GetTransform()->SetLocalPosition(Vector3(-25, 10, 0));
		leftBrickWall->GetTransform()->Scale(Vector3(20, 0.5, 25));
		leftBrickWall->GetTransform()->rotate(Vector3(0, 0, 1), 90);
		leftBrickWall->GetPhysicsComponent()->ConstructCollisionCube();


		world->AddToChildren(leftBrickWall);


		


		////-----------------------------setup left wall---------------------------------//


		////--------------------------SetUp middleCube--------------------------------//


		GameObj middleCube = Instantiate();
		middleCube->name = "midlle cube";
		middleCube->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "_cube.obj");
		middleCube->GetMeshComponent()->SetMaterial(diamondMaterial);
		middleCube->GetMeshComponent()->AddTextureToMeshAt(Texture(PXG::config::PXG_INDEPENDENT_TEXTURES_PATH + "diffuse3.jpg", TextureType::DIFFUSE), 0);
		middleCube->GetTransform()->SetLocalPosition(Vector3(0, 2.0, 0));
		middleCube->GetTransform()->Scale(Vector3(3, 3, 3));
		//middleCube->GetPhysicsComponent()->ConstructCollisionCube();
		world->AddToChildren(middleCube);
		middleCube->GetPhysicsComponent()->ConstructPhysicsRepresentationFromMeshComponent();

		//-----------------------Setup Diamond ------------------------------------//
		auto rotator = std::make_shared<RotatorComponent>(Vector3(0,1,0));

		GameObj diamond = Instantiate();
		diamond->name = "diamond";
		diamond->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "mineral.obj");
		diamond->GetMeshComponent()->SetMaterial(diamondMaterial);
		diamond->GetMeshComponent()->AddTextureToMeshAt(Texture(PXG::config::PXG_INDEPENDENT_TEXTURES_PATH + "bg-image.png", TextureType::DIFFUSE), 0);
		diamond->GetTransform()->SetLocalPosition(Vector3(0, 6.0, 0));
		diamond->GetTransform()->Scale(Vector3(1.5, 1.5, 1.5));
		diamond->GetPhysicsComponent()->ConstructPhysicsRepresentationFromMeshComponent();
	
		diamond->AddComponent(rotator);
		world->AddToChildren(diamond);

		

		GameObj smallDiamond = Instantiate();
		smallDiamond->name = "smallDiamond";
		smallDiamond->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "diamond.obj");
		smallDiamond->GetMeshComponent()->SetMaterial(diamondMaterial);
		smallDiamond->GetMeshComponent()->AddTextureToMeshAt(Texture(PXG::config::PXG_INDEPENDENT_TEXTURES_PATH + "bg-image.png", TextureType::DIFFUSE), 0);
		smallDiamond->GetTransform()->SetLocalPosition(Vector3(1.5, 0.0, 1.5));
		smallDiamond->GetTransform()->Scale(Vector3(0.3, 0.3, 0.3));
		smallDiamond->GetPhysicsComponent()->ConstructPhysicsRepresentationFromMeshComponent();

		diamond->AddToChildren(smallDiamond);



		//--------------------------setup lights----------------------------------------//
		//top spotlight

		auto lightComp = std::make_shared<LightComponent>();
		lightComp->SetIntensity(80.0f);
		lightComp->SetLightType(LightType::SPOTLIGHT);

		GameObj topSpotlight = Instantiate();
		topSpotlight->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "cube.obj");
		topSpotlight->GetMeshComponent()->SetMaterial(yellowColorMat);
		topSpotlight->name = "spotlight top";
		topSpotlight->AddComponent(lightComp);
		topSpotlight->GetTransform()->SetLocalPosition(Vector3(0.0, 23.0, 0.0));
		topSpotlight->GetTransform()->Scale(Vector3(0.5, 0.5, 0.5));
		world->AddToChildren(topSpotlight);

		auto leftlightComp = std::make_shared<LightComponent>();
		leftlightComp->SetIntensity(150.0f);
		GameObj leftPonintLight = Instantiate();
		leftPonintLight->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "cube.obj");
		leftPonintLight->GetMeshComponent()->SetMaterial(yellowColorMat);
		leftPonintLight->name = "leftPonintLight ";
		leftPonintLight->AddComponent(leftlightComp);
		leftPonintLight->GetTransform()->SetLocalPosition(Vector3(-12.0, 23.0, 0.0));
		leftPonintLight->GetTransform()->Scale(Vector3(0.5, 0.5, 0.5));
		world->AddToChildren(leftPonintLight);


		auto rightlightComp = std::make_shared<LightComponent>();
		rightlightComp->SetIntensity(150.0f);
		GameObj rightPonintLight = Instantiate();
		rightPonintLight->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "cube.obj");
		rightPonintLight->GetMeshComponent()->SetMaterial(yellowColorMat);
		rightPonintLight->name = "rightPonintLight";
		rightPonintLight->AddComponent(rightlightComp);
		rightPonintLight->GetTransform()->SetLocalPosition(Vector3(12.0, 23.0, 0.0));
		rightPonintLight->GetTransform()->Scale(Vector3(0.5, 0.5, 0.5));
		world->AddToChildren(rightPonintLight);










		*/




		//----------------------------------------------------------------- PHYSICS TEST -------------------------------------------------//

		//
		////--------------------Physics Test OBB EdgeToEdge --------------------------//


		
		GameObj yellowObject = Instantiate();
		yellowObject->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "_cube.obj");
		yellowObject->GetMeshComponent()->SetMaterial(yellowColorMat);
		yellowObject->GetPhysicsComponent()->ConstructPhysicsRepresentationFromMeshComponent();
		yellowObject->AddComponent(std::make_shared<Rigidbody>());
		yellowObject->name = "yellowObject";

		yellowObject->GetTransform()->SetLocalPosition(Vector3(0, 0, -6.2));
		yellowObject->GetTransform()->Scale(Vector3(1.0, 1.0, 1.0));

		world->AddToChildren(yellowObject);

		//yellowObject->GetTransform()->rotate(Vector3(0, 1, 0),45.0f);
		

		

		GameObj orangeObject = Instantiate();
		orangeObject->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "_cube.obj");
		orangeObject->GetMeshComponent()->SetMaterial(orangeColorMat);
		orangeObject->GetPhysicsComponent()->ConstructPhysicsRepresentationFromMeshComponent();
		orangeObject->name = "orangeObj";
		//orangeObject->AddComponent(std::make_shared<Rigidbody>());
		//orangeObject->AddComponent(std::make_shared<ForceAdderTestComponent>(PhysicsEngine::GetGravity() * -10.0f));

		orangeObject->GetTransform()->SetLocalPosition(Vector3(1.0, -2.0, -6.0));
		orangeObject->GetTransform()->Scale(Vector3(2.0, 1.0, 2.0));
		//orangeObject->GetTransform()->rotate(Vector3(0, 1, 0), 45.0f);
		//orangeObject->GetTransform()->rotate(Vector3(1, 0, 0), 90.0f);

		world->AddToChildren(orangeObject);

		//PhysicsEngine::SetGravity(Vector3());


		//*/

		////--------------------Physics Test OBB  --------------------------//
		//
		/*
		
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

		OBBTestObjectGreen->GetTransform()->SetLocalPosition(Vector3(3.0, -0.5, -2.0));
		OBBTestObjectGreen->GetTransform()->rotate(Vector3(1, 0, 0), 45.0f);
		OBBTestObjectGreen->GetTransform()->rotate(Vector3(0, 1, 0), 45.0f);
		OBBTestObjectGreen->GetTransform()->Scale(Vector3(1.0, 1.0, 1.0));
		world->AddToChildren(OBBTestObjectGreen);

		//*/

		////--------------------Physics Test AABB --------------------------//

		/*
		
		GameObj firstObj = Instantiate();
		firstObj->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "_cube.obj");
		firstObj->GetMeshComponent()->SetMaterial(redColorMat);
		firstObj->GetPhysicsComponent()->ConstructPhysicsRepresentationFromMeshComponent();
		firstObj->name = "redObj";
		
		firstObj->GetTransform()->SetLocalPosition(Vector3(1.0, 0.5, 1.5));
		firstObj->GetTransform()->Scale(Vector3(1.0, 1.0, 1.0));
		world->AddToChildren(firstObj);

		//*/

		/*
		GameObj secondObj = Instantiate();
		secondObj->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "_cube.obj");
		secondObj->GetMeshComponent()->SetMaterial(bluetColorMat);
		secondObj->GetPhysicsComponent()->ConstructPhysicsRepresentationFromMeshComponent();
		secondObj->name = "blueObj";

		secondObj->GetTransform()->SetLocalPosition(Vector3(3.0, 0.0, 2.0));
		secondObj->GetTransform()->Scale(Vector3(1.0, 1.0, 1.0));
		world->AddToChildren(secondObj);
		//*/

		//------------------------More Objects---------------------------------------//

		/*
		
		GameObj slategrayObject = Instantiate();
		slategrayObject->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "_cube.obj");
		slategrayObject->GetMeshComponent()->SetMaterial(slateGrayColorMat);
		slategrayObject->GetPhysicsComponent()->ConstructPhysicsRepresentationFromMeshComponent();
		slategrayObject->name = "slategrayObjectObj ";

		slategrayObject->GetTransform()->SetLocalPosition(Vector3(-0, 1.5, 0.0));
		slategrayObject->GetTransform()->Scale(Vector3(1.0, 1.0, 1.0));
		slategrayObject->GetTransform()->rotate(Vector3(1.0, 1.0, 0.0), 80.0f);
		world->AddToChildren(slategrayObject);

		GameObj cremeObj = Instantiate();
		cremeObj->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "_cube.obj");
		cremeObj->GetMeshComponent()->SetMaterial(cremeColorMat);
		cremeObj->GetPhysicsComponent()->ConstructPhysicsRepresentationFromMeshComponent();
		cremeObj->name = "cremeObj ";

		cremeObj->GetTransform()->SetLocalPosition(Vector3(2.0, 1.5, 0.0));
		cremeObj->GetTransform()->Scale(Vector3(1.0, 1.0, 1.0));
		cremeObj->GetTransform()->rotate(Vector3(1.0, 1.0, 1.0), 45.0f);
		world->AddToChildren(cremeObj);

		auto cremeWorldTransform = cremeObj->GetTransform()->GetWorldTransform();
		auto aabbTest = cremeObj->GetPhysicsComponent()->CreateAABBFromTransformedColliders(cremeWorldTransform);
		//world->GetDebugDrawingManager()->InstantiateAABBRepresentation(aabbTest.get(), Vector3(1, 0, 0), 30.0f);

		
		//*/
		//Vector3(0.77, 0.77, 0.77); Vector3(1, 0, 0); Vector3(0, 1, 0); Vector3(0, 0, 1); 
		//Vector3(0.77, 0.77, 0); Vector3(0, 0.77, 0.77); Vector3(0.8, 0, 0.6); Vector3(0, 0.6, 0.8); 
		
		std::vector<Vector3> positions;
		std::vector<Vector3> axis;
		std::vector<float> angles;
		
		/*1*/positions.push_back(Vector3(5.0, 5.0, -5.0)); axis.push_back(Vector3(0.77, 0.77, 0)); angles.push_back(30.0f);
		/*2*/positions.push_back(Vector3(5.0, -5.0, -5.0)); axis.push_back(Vector3(0.77, 0.77, 0.77)); angles.push_back(30.0f);
		/*3*/positions.push_back(Vector3(12.0, -7.0, 3.0)); axis.push_back(Vector3(0.77, 0.77, 0)); angles.push_back(20.0f);
		/*4*/positions.push_back(Vector3(20.0, -2.0, -12.0)); axis.push_back(Vector3(1.77, 0.77, 1)); angles.push_back(90.0f);
		/*5*/positions.push_back(Vector3(10.0, -2.0, 3.0)); axis.push_back(Vector3(2, 1, 0)); angles.push_back(120.0f);
		/*6*/positions.push_back(Vector3(8.0, 4.0, 3.0)); axis.push_back(Vector3(0, 1, 1)); angles.push_back(270.0f);
		/*7*/positions.push_back(Vector3(-8.0, -4.0, -3.0)); axis.push_back(Vector3(1, 1, 1)); angles.push_back(50.0f);
		/*8*/positions.push_back(Vector3(20.0, -7.0, 8.0)); axis.push_back(Vector3(2, 1, 0)); angles.push_back(100.0f);
		/*9*/positions.push_back(Vector3(1.0, -12.0, 6.0)); axis.push_back(Vector3(3, 1, 3)); angles.push_back(25.0f);
		/*10*/positions.push_back(Vector3(7.0, 7.0, 4.0));  axis.push_back(Vector3(0, 1, 0));  angles.push_back(1.0f);
		/*11*/positions.push_back(Vector3(13.0, -6.0, 8.0)); axis.push_back(Vector3(1, 1, 0)); angles.push_back(8.0f);
		/*12*/positions.push_back(Vector3(6.0, -4.0, 3.0)); axis.push_back(Vector3(0, 1, 1)); angles.push_back(5.0f);
		/*13*/positions.push_back(Vector3(-19.0, -19.0, -10.0));  axis.push_back(Vector3(1, 0, 1)); angles.push_back(2.0f);
		/*14*/positions.push_back(Vector3(10.0, -14.0, 13.0));  axis.push_back(Vector3(6, 0, 1)); angles.push_back(13.0f);
		/*15*/positions.push_back(Vector3(-11.0, -4.0, 4.0)); axis.push_back(Vector3(6,2, 4)); angles.push_back(60.0f);
		/*16*/positions.push_back(Vector3(12.0, -4.0, -3.0)); axis.push_back(Vector3(6, 2, 6)); angles.push_back(22.0f);
		/*17*/positions.push_back(Vector3(-12.0, -4.0, 3.0)); axis.push_back(Vector3(6, 2, 9)); angles.push_back(140.0f);
		/*18*/positions.push_back(Vector3(-9.0, 9.0, -9.0)); axis.push_back(Vector3(3, 2, 6)); angles.push_back(311.0f);
		/*19*/positions.push_back(Vector3(9.0, 9.0, 9.0)); axis.push_back(Vector3(1, 2, 6)); angles.push_back(28.0f);
		/*20*/positions.push_back(Vector3(8.0, -4.0, 20.0));  axis.push_back(Vector3(3, 8, 5)); angles.push_back(0.0f);

		//+20
		/*21*/positions.push_back(Vector3(25.0, 5.0, 5.0)); axis.push_back(Vector3(2, 3,4)); angles.push_back(2.0f);
		/*22*/positions.push_back(Vector3(21.0, 7.0, -9.0)); axis.push_back(Vector3(5, 4, 3)); angles.push_back(230.0f);
		/*23*/positions.push_back(Vector3(33.0, 17.0, -9.0)); axis.push_back(Vector3(2, 1, 2)); angles.push_back(330.0f);
		/*24*/positions.push_back(Vector3(27.0, -12.0, -11.0)); axis.push_back(Vector3(5, 2, 6)); angles.push_back(130.0f);
		/*25*/positions.push_back(Vector3(38.0, -3.0, 11.0)); axis.push_back(Vector3(10, 2, 10)); angles.push_back(20.0f);
		/*26*/positions.push_back(Vector3(38.0, 23.0, 11.0)); axis.push_back(Vector3(10, 1, 1)); angles.push_back(-50.0f);
		/*27*/positions.push_back(Vector3(40.0, -10.0, 11.0)); axis.push_back(Vector3(0,1, 0)); angles.push_back(20.0f);
		/*28*/positions.push_back(Vector3(40.0, -12.0, -3.0)); axis.push_back(Vector3(1, 0, 1)); angles.push_back(10.0f);
		/*29*/positions.push_back(Vector3(40.0, 0.0, 0)); axis.push_back(Vector3(2, 4, 2)); angles.push_back(70.0f);
		/*30*/positions.push_back(Vector3(40.0, 20.0,2)); axis.push_back(Vector3(1, 2, 6)); angles.push_back(90.0f);
		/*31*/positions.push_back(Vector3(30.0, 20.0, -20)); axis.push_back(Vector3(6, 2,1)); angles.push_back(110.0f);
		/*33*/positions.push_back(Vector3(33.0, 23.0, 5)); axis.push_back(Vector3(3,5, 3)); angles.push_back(140.0f);
		/*34*/positions.push_back(Vector3(37.0, 21.0, -5)); axis.push_back(Vector3(0, 0, 1)); angles.push_back(180.0f);
		/*35*/positions.push_back(Vector3(39.0,5.0, -5)); axis.push_back(Vector3(1, 0.77, 0)); angles.push_back(33.0f);
		/*36*/positions.push_back(Vector3(22.0, 5.0, 5)); axis.push_back(Vector3(1, 0.77, 0)); angles.push_back(10.0f);
		/*37*/positions.push_back(Vector3(22.0, 5.0, 15)); axis.push_back(Vector3(1, 4, 2)); angles.push_back(210.0f);
		/*38*/positions.push_back(Vector3(24.0, 7.0, 12)); axis.push_back(Vector3(3, 3, 4)); angles.push_back(160.0f);
		/*39*/positions.push_back(Vector3(26.0, -12.0, 4)); axis.push_back(Vector3(2, 9, 0)); angles.push_back(260.0f);
		/*40*/positions.push_back(Vector3(28.0, 17.0, 8)); axis.push_back(Vector3(6, 4, 5)); angles.push_back(280.0f);

		//z-20
		/*41*/positions.push_back(Vector3(23.0, 4.0, -25.0)); axis.push_back(Vector3(2, 3, 4)); angles.push_back(2.0f);
		/*42*/positions.push_back(Vector3(12.0, 2.0, -24.0)); axis.push_back(Vector3(4, 3, 2)); angles.push_back(20.0f);
		/*43*/positions.push_back(Vector3(5.0, 10.0, -29.0)); axis.push_back(Vector3(1, 1, 1)); angles.push_back(12.0f);
		/*44*/positions.push_back(Vector3(-5.0, 20.0, -35.0)); axis.push_back(Vector3(0, 3, 0)); angles.push_back(122.0f);
		/*45*/positions.push_back(Vector3(7.0, 17.0, -39.0)); axis.push_back(Vector3(8, 2, 2)); angles.push_back(52.0f);
		/*46*/positions.push_back(Vector3(-8.0, 15.0, -40.0)); axis.push_back(Vector3(6, 3, 6)); angles.push_back(92.0f);
		/*47*/positions.push_back(Vector3(0.0, -2.0, -20.0)); axis.push_back(Vector3(2, 1, 0)); angles.push_back(62.0f);
		/*49*/positions.push_back(Vector3(0.0, 0.0, -25.0)); axis.push_back(Vector3(2, 1, 0)); angles.push_back(222.0f);
		/*50*/positions.push_back(Vector3(-8.0, 2.0, -20.0)); axis.push_back(Vector3(2, 1, 2)); angles.push_back(42.0f);
		/*51*/positions.push_back(Vector3(9.0, -2.0, -23.0)); axis.push_back(Vector3(2, -1, 0)); angles.push_back(12.0f);
		/*52*/positions.push_back(Vector3(-8.0, 20.0, -28.0)); axis.push_back(Vector3(-2, 1, 0)); angles.push_back(32.0f);
		/*53*/positions.push_back(Vector3(2.0, -15.0, -38.0)); axis.push_back(Vector3(-2, 1, -1)); angles.push_back(82.0f);
		/*54*/positions.push_back(Vector3(-2.0, -15.0, -38.0)); axis.push_back(Vector3(-2, 34, -1)); angles.push_back(62.0f);
		/*55*/positions.push_back(Vector3(-20.0, 15.0, -34.0)); axis.push_back(Vector3(-2, 1, -1)); angles.push_back(122.0f);
		/*56*/positions.push_back(Vector3(2.0, -15.0, -33.0)); axis.push_back(Vector3(-2, 12, -5)); angles.push_back(222.0f);
		/*57*/positions.push_back(Vector3(12.0, -15.0, -31.0)); axis.push_back(Vector3(-2, 5, -1)); angles.push_back(342.0f);
		/*58*/positions.push_back(Vector3(-15.0, -15.0, -23.0)); axis.push_back(Vector3(-2, 1, -2)); angles.push_back(270.0f);
		/*59*/positions.push_back(Vector3(-7.0, -15.0, -33.0)); axis.push_back(Vector3(-2, 10, -1)); angles.push_back(45.0f);
		/*60*/positions.push_back(Vector3(-23.0, -15.0, -32.0)); axis.push_back(Vector3(-2, 7, -5)); angles.push_back(-45.0f);


		//Octree optimization test
		for(int i = 0; i < 30;i++)
		{
			GameObj Obj = Instantiate();
			Obj->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "_cube.obj");
			Obj->GetMeshComponent()->SetMaterial(cremeColorMat);
			Obj->GetPhysicsComponent()->ConstructPhysicsRepresentationFromMeshComponent();
			Obj->name = "Object " + std::to_string(i);

			Obj->GetTransform()->SetLocalPosition(positions.at(i));
			Obj->GetTransform()->Scale(Vector3(1.0, 1.0, 1.0));
			Obj->GetTransform()->rotate(axis.at(i),angles.at(i));
			world->AddToChildren(Obj);
		}


		//Octree optimization test

		

		Debug::Log("start  ");
		double finalTime = 0;
		float z = 1.0f;
		for (int i = 0; i < 0; i++)
		{
			GameObj slategrayObject = Instantiate();
			slategrayObject->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "_cube.obj");
			slategrayObject->GetMeshComponent()->SetMaterial(slateGrayColorMat);
			slategrayObject->GetPhysicsComponent()->ConstructPhysicsRepresentationFromMeshComponent();
			slategrayObject->name = "slategrayObjectObj " + std::to_string(i);

			slategrayObject->GetTransform()->SetLocalPosition(Vector3(-0, 1.5, z * i));
			slategrayObject->GetTransform()->Scale(Vector3(1.0, 1.0, 1.0));
			slategrayObject->GetTransform()->rotate(axis.at((int)Mathf::Map(i, 0, 500, 20, 40)), angles.at((int)Mathf::Map(i, 0, 500, 0, 20)));
			world->AddToChildren(slategrayObject);

			GameObj cremeObj = Instantiate();
			cremeObj->GetMeshComponent()->Load3DModel(PXG::config::PXG_MODEL_PATH + "_cube.obj");
			cremeObj->GetMeshComponent()->SetMaterial(cremeColorMat);
			cremeObj->GetPhysicsComponent()->ConstructPhysicsRepresentationFromMeshComponent();
			cremeObj->name = "cremeObj " + std::to_string(i);

			cremeObj->GetTransform()->SetLocalPosition(Vector3(2.0, 1.5, z * i));
			cremeObj->GetTransform()->Scale(Vector3(1.0, 1.0, 1.0));
			cremeObj->GetTransform()->rotate(axis.at((int)Mathf::Map(i,0,500,0,20)), angles.at((int)Mathf::Map(i, 0, 500, 20, 40)));
			world->AddToChildren(cremeObj);


			auto colliderA = slategrayObject->GetPhysicsComponent()->GetCollider();

			auto colliderB = cremeObj->GetPhysicsComponent()->GetCollider();

			Manifold m;
			m.physicsComponentA = slategrayObject->GetPhysicsComponent();
			m.physicsComponentB = cremeObj->GetPhysicsComponent();
			m.transformA = slategrayObject->GetTransform()->GetWorldTransform();
			m.transformB = cremeObj->GetTransform()->GetWorldTransform();

			double time = GetWorld()->GetTimeSystem()->GetTime();
			colliderA->CheckCollision(colliderB, m);
			finalTime += Mathf::Abs(GetWorld()->GetTimeSystem()->GetTime() - time);

			assert(GetWorld()->GetTimeSystem()->GetTime() > 0);
		}

		Debug::Log("it took {0} ", finalTime);


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

