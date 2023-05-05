#pragma once
#include <imgui.h>

#include "../Timer/Timer.h"
#include "Log.h"

class EngineUI
{
public:
	EngineUI()
	{}

	void createLog()
	{
		Log::create();
		if (timer.elapsed() > 1000)
		{
			Log::log("[Engine Info Log] " + std::to_string(1000.0f / ImGui::GetIO().Framerate) + 
					 "ms / frame(" + std::to_string(ImGui::GetIO().Framerate) + " FPS)"
			);
			timer.reset();
		}
	}
	void createSceneEditor()
	{
		ImGui::Begin("Scene Editor");
		ImGui::Text("THIS IS THE SCENE EDITOR");
		ImGui::End();
	}

private:
	Timer timer;
};