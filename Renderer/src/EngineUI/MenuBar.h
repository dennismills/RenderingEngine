#pragma once
#include "../Mouse/Mouse.h"

#include <imgui.h>

class MenuBar
{
public:
	void create()
	{
		if (ImGui::BeginMainMenuBar())
		{
			ImVec2 startOfWindow = ImGui::GetCursorPos();
			ImVec2 rectMin = ImGui::GetItemRectMin();
			ImVec2 rectMax = ImGui::GetWindowSize();
			Mouse::addBoundingRect("MenuBar", BoundingRect(rectMin.x, rectMin.y, rectMax.x, rectMax.y));
			ImGui::EndMainMenuBar();
		}
	}
};