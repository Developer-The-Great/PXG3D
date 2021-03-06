#pragma once

#include <list>


namespace PXG
{
	class Time
	{
	public:

		Time(int ,float );

		static float GetTime();

		float GetAverageDeltaTime() const;

		void UpdateTimePassed();

		float GetCurrentDeltaTime() const;

		void UpdateAverageTime();

		float GetFPS() const;

	private:

		const float maxTimePassed = 1.0f;

		float averageDeltaTick;

		float timePassed;

		std::list<float> averageTimes;

		int frameTickStoredCount;

		void calculateAverageDeltaTick();
	};

}

