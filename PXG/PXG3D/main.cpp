#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <soloud/soloud.h>


#define IS_EDITOR 0

#if defined(IS_EDITOR) && IS_EDITOR == 1
#define GAME_INSTANCE PXGEditor
#else
#define GAME_INSTANCE PXGGame
#endif



#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#include "PXGWindow.h"
#include <iostream>
#include <memory>

#include "Mathf.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Debug.h"
#include "Input.h"
#include "Transform.h"

#include "PXGGame.h"
#include "RenderingEngine.h"
#include "Time.h"
#include "World.h"
#include "Canvas.h"
#include "KeyCode.h"


#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_glfw.h"
#include "imgui/examples/imgui_impl_opengl3.h"
#include "ScreenSize.h"

#include "DebugDrawingManager.h"

constexpr int width = PXG::ScreenSize::WIDTH;
constexpr int height = PXG::ScreenSize::HEIGHT;

#include "AABBBox.h"
#include "DebugDrawingManager.h"

#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;
int main()
{


	Debug::SetDebugState(true);

	Debug::Log(Verbosity::Info, "PXG is running");

	//--------------------------- Initializing GLFW Stuff--------------------//

	//TODO refactor glfw stuff

	PXG::PXGWindow::Init();

	GLFWwindow* window = PXG::PXGWindow::MakeGLFWWindow(width, height, "PXG3D Game", NULL, NULL);

	if (window == nullptr)
	{
		Debug::Log(Verbosity::Error, "Failed to create window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Debug::Log(Verbosity::Error, "Failed to initialize glad");
		return -1;
	}

	

	//tell OpenGL the size of the rendering window
	glViewport(0, 0, width, height);
	//TODO LOW PRIORITY : refactor callbacks
	glfwSetKeyCallback(window, PXG::PXGWindow::key_callback);
	glfwSetMouseButtonCallback(window, PXG::PXGWindow::mouse_button_callback);
	glfwSetCursorPosCallback(window, PXG::PXGWindow::mouse_position_callback);

	//--------------------Initialize Game-----------------------//

	auto gamePtr = std::make_shared<PXG:: GAME_INSTANCE >();
	

	//--------------------Initialize Rendering Engine-----------------------//

	std::unique_ptr<PXG::RenderingEngine> renderingEngine = std::make_unique<PXG::RenderingEngine>();
	renderingEngine->SetWorld(gamePtr->GetWorld());
	renderingEngine->SetCanvas(gamePtr->GetCanvas());


	//--------------------Initialize Physics Engine -------------------------//

	std::unique_ptr<PXG::PhysicsEngine> physicsEngine = std::make_unique<PXG::PhysicsEngine>();
	physicsEngine->SetWorld(gamePtr->GetWorld());

	//------------------- Initialize World -----------------------------------//

	int frameTickStored = 6;
	int estimatedInitialFPS = 60;

	std::shared_ptr<PXG::Time> time = std::make_shared<PXG::Time>(frameTickStored, estimatedInitialFPS);

	gamePtr->frender = &renderingEngine->GetFontRenderer();
	gamePtr->Initialize();
	gamePtr->Start();

	Debug::Log("children in world {0}", gamePtr->GetWorld()->GetImmediateChildrenCount());
	Debug::Log("children in Canvas {0}", gamePtr->GetCanvas()->GetImmediateChildrenCount());

	//--------------------------------- Initialize Debug Mesh Object------------------------------//

	std::shared_ptr<PXG::DebugDrawingManager> debugDrawingManager = std::make_shared<PXG::DebugDrawingManager>();
	debugDrawingManager->SetWorld(gamePtr->GetWorld());
	gamePtr->GetWorld()->SetDebugDrawingManager(debugDrawingManager);




	gamePtr->GetWorld()->SetTimeSystem(time);
	
	auto aabbTest = std::make_shared<PXG::AABBBox>(PXG::Vector3(0.0, 0.0, 0.0), PXG::Vector3(0.5, 0.5, 0.5));

	gamePtr->GetWorld()->GetDebugDrawingManager()->InstantiateCube(aabbTest->position, PXG::Vector3(0.0, -0.0, 0.0), aabbTest->halfWidths * 2.0f, PXG::Vector3(1, 0, 1), 30.0f, -aabbTest->halfWidths);

	std::vector<std::shared_ptr<PXG::AABBBox>> splitBoxes;
	aabbTest->OctreeBoxSplit(splitBoxes);

	auto aabbTest2 = std::make_shared<PXG::AABBBox>(PXG::Vector3(2.75, 0.5, 0.5), PXG::Vector3(0.5, 0.5, 0.5));
	gamePtr->GetWorld()->GetDebugDrawingManager()->InstantiateCube(aabbTest2->position, PXG::Vector3(), aabbTest2->halfWidths * 2.0f, PXG::Vector3(1, 0, 0), 30.0f, -aabbTest2->halfWidths);

	if (aabbTest->CheckCollisionWithAABB(aabbTest2.get()))
	{
		Debug::Log("Collision");
	}

	/*for(const auto& box : splitBoxes)
	{
		gamePtr->GetWorld()->GetDebugDrawingManager()->InstantiateCube(box->position, PXG::Vector3(0.0, -0.0, 0.0), box->halfWidths * 2.0f, PXG::Vector3(1, 1, 1), 30.0f, -box->halfWidths);
	}*/


	//-------------------------------------- GAME LOOP ------------------------------------//
	glEnable(GL_DEPTH_TEST);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	
	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0.4f, 0.5f, 0.8f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//track current time
		time->UpdateTimePassed();

		Input::PollEvents();

		physicsEngine->SetTickRemaining(time->GetAverageDeltaTime());

		gamePtr->Update();

		while (physicsEngine->IsTicking())
		{
			
			float tick = physicsEngine->DecreaseRemainingTickTime();

			//fixed update on game
			gamePtr->FixedUpdate(tick);

			float currentTime = time->GetTime();

		}

		float beforeCol = time->GetTime();
		physicsEngine->CheckCollisions();
		Debug::Log("Collision check time {0}", time->GetTime() - beforeCol);



		//gamePtr->Update();
		
		renderingEngine->RenderCurrentlySetWorld();



		glDisable(GL_DEPTH_TEST);
		debugDrawingManager->DrawDebugObjects();

		debugDrawingManager->DecreaseLifespan(time->GetAverageDeltaTime());
		debugDrawingManager->RemoveDeadDebugMeshes();

		renderingEngine->RenderCanvas();


		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(window);

		Input::LateUpdateTrackedKeyStates();

		Debug::Log("FPS :{0}", time->GetFPS());

		time->UpdateAverageTime();


	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	return 0;
}
