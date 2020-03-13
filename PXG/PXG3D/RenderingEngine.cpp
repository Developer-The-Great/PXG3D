#include "RenderingEngine.h"
#include "World.h"
#include "Canvas.h"
#include "PhysicsComponent.h"
#include "CameraComponent.h"
#include "Mat4.h"
#include "Mesh.h"
#include "RasterizationMode.h"
namespace PXG
{
	RenderingEngine::RenderingEngine()
	{

	}
	void RenderingEngine::AddRenderingComponent(std::shared_ptr<MeshComponent> meshComponent)
	{

	}
	//TODO [Medium Priority] refactor RenderCurrentlySetWorld() and RenderDebugDrawingForSetWorld()
	void RenderingEngine::RenderCurrentlySetWorld() const
	{

		//check if the world exists
		if(!world) return;

		auto camera = world->GetCamera();

		//check if the camera exists
		if (!camera) return;
		glEnable(GL_DEPTH_TEST);
		Mesh::SetRasterizationMode(RasterizationMode::FILL);
		auto meshComponent = world->GetMeshComponent();

		//render mesh if it exists
		if (meshComponent)
		{
			meshComponent->Draw(Mat4(), camera->GetView(), camera->GetProjection());
		}
		glDisable(GL_DEPTH_TEST);
		Mesh::SetRasterizationMode(RasterizationMode::LINE);
		//render physics components if necessary
		if (world->IsDrawPhysicsComponentMeshNeeded() )
		{
			world->GetPhysicsComponent()->DrawPhysicsRepresentation(Mat4(), camera->GetView(), camera->GetProjection());
		}
	}

	void RenderingEngine::RenderCanvas() const
	{
		Mesh::SetRasterizationMode(RasterizationMode::FILL);
		const glm::vec3 red = { 0.2f,0.2f,0.2f };

		std::shared_ptr<CameraComponent>  camera = canvas->GetCamera();

		if (camera)
		{

			std::shared_ptr<MeshComponent> meshComponent = canvas->GetMeshComponent();

			if (meshComponent)
			{
				meshComponent->Draw(Mat4(), camera->GetView(), camera->GetProjection());
			}

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			if (fontRenderer)
				fontRenderer->draw(camera->GetProjection().ToGLM(), red);
		}
	}
	void RenderingEngine::SetWorld(std::shared_ptr<World> world)
	{
		this->world = world;
	}
	void RenderingEngine::SetCanvas(std::shared_ptr<Canvas> canvas)
	{
		this->canvas = canvas;
	}
}

