#pragma once
#include "Component.h"
#include <vector>
#include <future>
#include "HitInfo.h"
#include "Subject.h"
namespace PXG
{
	class RayCastShooter :public Component, public subject_base
	{
	public:

		enum events : event_t
		{
			ON_RAYCAST_HIT
		};

		virtual void Start() override;

		virtual void FixedUpdate(float tick) override;

		virtual void Update() override;

		void SetActive(bool condition) { isActive = condition; };
		virtual const HitInfo& GetLastHit() const
		{
			return lastHit;
		}

	private:

		std::shared_ptr<GameObject> possessedGameObject;
		float speed = 0.05f;



		HitInfo lastHit;
		bool isActive = false;
	};
}


