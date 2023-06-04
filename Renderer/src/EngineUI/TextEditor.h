#pragma once
#include <string>

class TextEditor
{
public:
	TextEditor() {}

	void create()
	{
        ImGui::Begin("Text Editor", &textEditorOpen);
        ImVec2 startOfWindow = ImGui::GetCursorPos();
        ImVec2 rectMin = ImGui::GetItemRectMin();
        ImVec2 rectMax = ImGui::GetWindowSize();
        Mouse::addBoundingRect("Text Editor", BoundingRect(rectMin.x, rectMin.y, rectMax.x, rectMax.y));

        static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;

        // For this demo we are using ImVector as a string container.
            // Note that because we need to store a terminating zero character, our size/capacity are 1 more
            // than usually reported by a typical string class.
        
        if (source.empty())
            source.push_back(0);
        Funcs::MyInputTextMultiline("##MyStr", &source, ImVec2(-FLT_MIN, ImGui::GetWindowSize()[1] - ImGui::GetTextLineHeight() * 3), flags);
        ImGui::End();

        if (ImGui::IsWindowFocused())
        {
            hasFocus = true;
        }
        else
        {
            hasFocus = false;
        }
	}

    void checkKeyPresses(GLFWwindow* window)
    {
        isCtrlPressed = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;
        isCtrlReleased = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_RELEASE;

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && isCtrlPressed && !singleFrameKeyCheck)
        {
            save();
            singleFrameKeyCheck = true;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE && isCtrlReleased)
        {
            singleFrameKeyCheck = false;
        }
    }

    static void setText(const std::string& s)
    {
        // Clear the existing content of my_str and copy the new text
       source.clear();

        for (char c : s)
        {
            source.push_back(c);
        }
        source.push_back(0);
    }

    static void setTextEditorOpen(bool v) { textEditorOpen = v; }
    static bool isTextEditorOpen() { return textEditorOpen; }
    static bool isFocused() { return hasFocus; }
    static void setOpenFileName(std::string fileName) { openFileName = fileName; }
private:
    struct Funcs
    {
        static int MyResizeCallback(ImGuiInputTextCallbackData* data)
        {
            if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
            {
                ImVector<char>* str = (ImVector<char>*)data->UserData;
                IM_ASSERT(str->begin() == data->Buf);
                str->resize(data->BufSize); // NB: On resizing calls, generally data->BufSize == data->BufTextLen + 1
                data->Buf = str->begin();
            }
            return 0;
        }
        // Note: Because ImGui:: is a namespace you would typically add your own function into the namespace.
        // For example, you code may declare a function 'ImGui::InputText(const char* label, MyString* my_str)'
        static bool MyInputTextMultiline(const char* label, ImVector<char>* str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0)
        {
            IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
            return ImGui::InputTextMultiline(label, str->begin(), (size_t)str->size(), size, flags | ImGuiInputTextFlags_CallbackResize, Funcs::MyResizeCallback, (void*)str);
        }
    };

    void save()
    {
        std::ofstream file(openFileName);
        if (!file)
        {
            printf("Cannot open the file: %s\n", openFileName.c_str());
            assert(false);
        }
        else
        {
            file << source.Data;
            file.close();
        }
    }
private:
    static ImVector<char> source;
    static std::string openFileName;
    static bool textEditorOpen;
    static bool hasFocus;

    bool isCtrlPressed;
    bool isCtrlReleased;
    bool singleFrameKeyCheck;
};