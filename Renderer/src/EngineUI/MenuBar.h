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
			if (ImGui::BeginMenu("View"))
			{
				std::string textEditorName = "Text Editor" + (TextEditor::isTextEditorOpen() ? std::string("\t[OPEN]") : std::string(""));
				if (ImGui::MenuItem(textEditorName.c_str()))
				{
					TextEditor::setTextEditorOpen(!TextEditor::isTextEditorOpen());
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

private:
};