#pragma once

#include <chrono>
#include "Debug.h"
#include <string>

namespace PXG
{
	class BenchmarkTimer
	{

	public:

		BenchmarkTimer(std::string message = " This operation ")
		{
			this->message = message;
			startTimePoint = std::chrono::high_resolution_clock::now();
		}

		~BenchmarkTimer()
		{
			Stop();
		}

	private:


		std::string message;
		void Stop()
		{
			auto endPoint = std::chrono::high_resolution_clock::now();

			auto startTime = std::chrono::time_point_cast<std::chrono::microseconds>
				(startTimePoint).time_since_epoch().count();

			auto endTime = std::chrono::time_point_cast<std::chrono::microseconds>
				(endPoint).time_since_epoch().count();

			auto duration = endTime - startTime;


			double ms = duration * 0.001;

			Debug::Log("{0} took {1} ms", message,ms);

		}

		std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;

	};
}


