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
		ImGui::Text(text.c_str());
		ImGui::SetScrollHereY(1.0);
		ImGui::End();
	}

	// TODO: Add better logging support with colors for the logging mode
	static void log(const std::string& msg)
	{
		text += msg + "\n";
	}

	static void clear()
	{
		text = "";
	}

private:
	static bool showLog;
	static std::string text;
};