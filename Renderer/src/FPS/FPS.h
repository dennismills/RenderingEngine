#pragma once
#include <iostream>
#include <chrono>
#include <ctime> 

class FPS
{
public:
	FPS() = delete;
	static void start()
	{
		startTime = std::chrono::system_clock::now();
	}

	static void update()
	{
		currentTime = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsedTime = currentTime - startTime;
		counter += 1;
		if (elapsedTime.count() >= 1.0)
		{
			std::cout << "FPS: " << counter << "\tTime Per Frame: " << elapsedTime.count() / (float)counter << std::endl;
			counter = 0;
			FPS::start();
			currentTime = startTime;
		}
	}

private:
	static std::chrono::system_clock::time_point startTime;
	static std::chrono::system_clock::time_point currentTime;
	static long counter;
};
