#pragma once
#include <string>

class TextEditor
{
public:
	TextEditor() {}

	void create()
	{
        ImGui::Begin("Text Editor");
        ImVec2 startOfWindow = ImGui::GetCursorPos();
        ImVec2 rectMin = ImGui::GetItemRectMin();
        ImVec2 rectMax = ImGui::GetWindowSize();
        Mouse::addBoundingRect("TextEditor", BoundingRect(rectMin.x, rectMin.y, rectMax.x, rectMax.y));

        static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
        struct Funcs
        {
            static int MyResizeCallback(ImGuiInputTextCallbackData* data)
            {
                if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
                {
                    ImVector<char>* my_str = (ImVector<char>*)data->UserData;
                    IM_ASSERT(my_str->begin() == data->Buf);
                    my_str->resize(data->BufSize); // NB: On resizing calls, generally data->BufSize == data->BufTextLen + 1
                    data->Buf = my_str->begin();
                }
                return 0;
            }
            // Note: Because ImGui:: is a namespace you would typically add your own function into the namespace.
            // For example, you code may declare a function 'ImGui::InputText(const char* label, MyString* my_str)'
            static bool MyInputTextMultiline(const char* label, ImVector<char>* my_str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0)
            {
                IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
                return ImGui::InputTextMultiline(label, my_str->begin(), (size_t)my_str->size(), size, flags | ImGuiInputTextFlags_CallbackResize, Funcs::MyResizeCallback, (void*)my_str);
            }
        };

        // For this demo we are using ImVector as a string container.
            // Note that because we need to store a terminating zero character, our size/capacity are 1 more
            // than usually reported by a typical string class.
        static ImVector<char> my_str;
        if (my_str.empty())
            my_str.push_back(0);
        Funcs::MyInputTextMultiline("##MyStr", &my_str, ImVec2(-FLT_MIN, ImGui::GetWindowSize()[1] - ImGui::GetTextLineHeight() * 3), flags);
        ImGui::End();
	}

private:


private:
    std::string src;
};