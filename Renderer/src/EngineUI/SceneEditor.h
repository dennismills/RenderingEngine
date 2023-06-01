#pragma once
#include "../GameObject/GameObject.h"

#include <vector>
#include <imgui.h>

class SceneEditor
{
public:
	SceneEditor()
	{

	}
	void create()
	{
		ImGui::Begin("Scene Editor");
		ImVec2 startOfWindow = ImGui::GetCursorPos();
		ImVec2 rectMin = ImGui::GetItemRectMin();
		ImVec2 rectMax = ImGui::GetWindowSize();
		Mouse::addBoundingRect("SceneEditor", BoundingRect(rectMin.x, rectMin.y, rectMax.x, rectMax.y));

		int index = 0;
		for (auto& obj : objs)
		{
			ImGui::PushID(index);
			bool treeNode = ImGui::TreeNodeEx(obj->getName().c_str(),
				ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth,
				obj->getName().c_str()
			);
			ImGui::PopID();

			if (treeNode)
			{
				obj->populateUI();
				ImGui::TreePop();
			}
			++index;
		}
		ImGui::End();
	}
	void add(GameObject* obj) { objs.push_back(obj); }

private:
	
	// We do not delete the GameObjects because they are in use elsewhere
	// (like ModelManager)
	std::vector<GameObject*> objs;
};