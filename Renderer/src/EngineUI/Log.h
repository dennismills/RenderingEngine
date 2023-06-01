#pragma once
#include <string>
#include <imgui.h>

// TODO: Add an export button that can export the log to a file (and on close)
class Log
{
public:

	static void create()
	{
		ImGui::Begin("Log", &showLog);
		ImVec2 startOfWindow = ImGui::GetCursorPos();
		ImVec2 rectMin = ImGui::GetItemRectMin();
		ImVec2 rectMax = ImGui::GetWindowSize();
		Mouse::addBoundingRect("Log", BoundingRect(rectMin.x, rectMin.y, rectMax.x, rectMax.y));
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		for (const auto& message : text) {
			ImGui::TextWrapped("%s", message.c_str());
		}

		// Set the scroll position to the bottom to display the latest log
		ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();
		ImGui::End();
	}

	// TODO: Add better logging support with colors for the logging mode
	static void log(const std::string& msg)
	{
		text.push_back(msg);;
	}

	static void clear()
	{
		text.clear();
	}

private:
	static bool showLog;
	static std::vector<std::string> text;
};