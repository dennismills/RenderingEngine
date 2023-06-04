#pragma once
#include <imgui.h>

#include "../Timer/Timer.h"
#include "Log.h"
#include "SceneEditor.h"
#include "TextEditor.h"
#include "MenuBar.h"
#include "FileTree.h"

class EngineUI
{
public:
	EngineUI()
	{}

	void createMenuBar()
	{
		menuBar.create();
	}
	void createFileTreeExplorer()
	{
		fileTree.create("D:/OpenGL/Renderer/Renderer/src");
	}
	void createTextEditor()
	{
		if (textEditor.isTextEditorOpen())
		{
			textEditor.create();
		}
	}
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
		sceneEditor.create();
	}
	void addToSceneEditor(GameObject* obj)
	{
		obj->serialize();
		sceneEditor.add(obj);
	}

	void relayKeyPresses(GLFWwindow* window)
	{
		if (textEditor.isTextEditorOpen())
		{
			textEditor.checkKeyPresses(window);
		}
	}
private:
	Timer timer;
	SceneEditor sceneEditor;
	TextEditor textEditor;
	FileTree fileTree;
	MenuBar menuBar;
};