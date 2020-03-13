#include "GameObject.h"
#include "Debug.h"
#include "MeshComponent.h"
#include "PhysicsComponent.h"


namespace PXG
{
	GameObject::GameObject()
	{
		Debug::Log("init {0}", name);
		physicsComponent = std::make_shared<PhysicsComponent>();
		meshComponent = std::make_shared<MeshComponent>();

	}

	GameObject::~GameObject()
	{
		//Debug::Log(Verbosity::Info, "GameObject Destroyed");
	}

	void GameObject::Start()
	{
		InitializeComponentOwners();
		for (auto const& child : children)
		{
			child->Start();
		}

		for (auto const& component : components)
		{
			component->Start();
		}

	}
	void GameObject::Update()
	{
	}

	void GameObject::FixedUpdate(float tick)
	{

		for (auto const& child : children)
		{
			child->FixedUpdate(tick);
		}

		for (auto const& component : components)
		{
			if(component->IsActive())
			{
				component->FixedUpdate(tick);
			}
		}
	}

	void GameObject::AddToChildren(GOSharedPtr gameObj)
	{
		children.push_back(gameObj);
		gameObj->SetParent(shared_from_this());
	}

	void GameObject::SetParent(GOSharedPtr gameObj)
	{
		parent = gameObj;
		transform.SetParent(gameObj->GetTransform());
	}

	//TODO implement SetPosition


	void GameObject::SetLocalPosition(Vector3 newPosition)
	{
		transform.SetLocalPosition(newPosition);
	}

	int GameObject::GetImmediateChildrenCount() const
	{
		return static_cast<int>(children.size());
	}

	std::shared_ptr<MeshComponent> GameObject::GetMeshComponent() const
	{
		return meshComponent;
	}

	std::shared_ptr<PhysicsComponent> GameObject::GetPhysicsComponent() const
	{
		return physicsComponent;
	}

	std::vector<std::shared_ptr<GameObject>>& GameObject::GetChildren()
	{
		return children;
	}

	std::weak_ptr<World> GameObject::GetWorld() const
	{
		return world;
	}

	void GameObject::SetWorld(std::shared_ptr<World> world)
	{
		this->world = world;
	}

	Transform* GameObject::GetTransform()
	{
		return &transform;
	}

	void GameObject::RemoveChildren(std::shared_ptr<GameObject> obj)
	{
		for(auto & child : obj->children)
		{
			obj->RemoveChildren(child);
		}
		children.erase(std::remove(children.begin(), children.end(), obj), children.end());
	}

	void GameObject::InitializeComponentOwners()
	{
		physicsComponent->SetOwner(shared_from_this());
		meshComponent->SetOwner(shared_from_this());
	}

	

}
