#pragma once
#include <stdio.h>
#include <imgui.h>
#include <GL/glew.h>

#include <stdexcept>

class EngineViewport
{
public:
	EngineViewport()
	{
	}

	void initBuffers()
	{
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glViewport(0, 0, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);


		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

		// Check for framebuffer completeness
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw std::runtime_error("Framebuffer not complete");
		}

		// Render to the FBO
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	}
	void render()
	{
		bool t = true;
		ImGui::Begin("Viewport");
		ImVec2 startOfWindow = ImGui::GetCursorPos();
		ImVec2 rectMin = ImGui::GetItemRectMin();
		ImVec2 rectMax = ImGui::GetWindowSize();
		rectMax.y -= ImGui::GetTextLineHeight() * 3;
		ImTextureID texID = (void*)(intptr_t)tex;
		ImGui::Image(texID, rectMax, ImVec2(0, 1), ImVec2(1, 0));
		if (ImGui::IsWindowFocused())
		{
			hasFocus = true;
		}
		else
		{
			hasFocus = false;
		}
		ImGui::End();
	}
	void unbindBuffers() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
	void cleanup()
	{
		glDeleteTextures(1, &tex);
		glDeleteRenderbuffers(1, &rbo);
		glDeleteFramebuffers(1, &fbo);
	}

	static bool isFocused() { return hasFocus; }
private:
	GLuint fbo, tex, rbo;
	static bool hasFocus;
};