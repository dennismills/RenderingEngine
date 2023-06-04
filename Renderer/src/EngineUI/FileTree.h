#pragma once
#include <string>
#include <vector>
#include <experimental/filesystem>
#include <imgui.h>
#include <algorithm>

#include "../Mouse/Mouse.h"
#include "../Texture/Texture.h"

namespace fs = std::experimental::filesystem;

class FileTree
{
public:
    enum FileType
    {
        NO_EXT,
        TXT,
        CPP,
        HEADER,
        MODEL,
        IMG
    };

    struct FileNode
    {
        std::string name;
        bool isDir;
        std::vector<FileNode> children;
    };

    FileTree()
    {
        folderIconTextureID = Texture::load("src/assets/icons/folder.png");
        textIconTextureID = Texture::load("src/assets/icons/fileIcon.png");
        imageIconTextureID = Texture::load("src/assets/icons/imageIcon.png");
        modelIconTextureID = Texture::load("src/assets/icons/modelIcon.png");
        cppIconTextureID = Texture::load("src/assets/icons/cppIcon.png");
        headerIconTextureID = Texture::load("src/assets/icons/headerIcon.png");
    }

    void create(const std::string& rootDir)
    {
        int indexOfLastSlash = rootDir.rfind("/");
        rootPathPrefix = rootDir.substr(0, indexOfLastSlash + 1);
        fs::path rootPath = rootDir.c_str();
        treeRoot = new FileNode();
        treeRoot->name = rootPath.filename().string();
        treeRoot->isDir = true;
        walk(rootPath, treeRoot);
        createFileNode(treeRoot);
    }

private:
    FileNode* treeRoot;
    unsigned int folderIconTextureID;
    unsigned int textIconTextureID;
    unsigned int imageIconTextureID;
    unsigned int modelIconTextureID;
    unsigned int cppIconTextureID;
    unsigned int headerIconTextureID;
    std::string rootPathPrefix;

private:

    void walk(const fs::path& p, FileNode* parentNode)
    {
        for (const auto& entry : fs::directory_iterator(p))
        {
            const fs::path& path = entry.path();

            if (fs::is_directory(path))
            {
                FileNode dirNode;
                dirNode.name = path.filename().string();
                dirNode.isDir = true;
                parentNode->children.push_back(dirNode);
                walk(path, &(parentNode->children.back()));
            }
            else if (fs::is_regular_file(path))
            {
                FileNode fileNode;
                fileNode.name = path.filename().string();
                fileNode.isDir = false;
                parentNode->children.push_back(fileNode);
            }
        }
    }
    std::string findFile(FileNode* r, const std::string& targetFile) {
        std::string path = "";

        if (r == nullptr)
            return path;

        if (r->name == targetFile)
            return r->name;

        for (FileNode child : r->children) {
            std::string childPath = findFile(&child, targetFile);
            if (!childPath.empty()) {
                path = r->name + "/" + childPath;
                break;
            }
        }

        return path;
    }

    void createFileNode(FileNode* node)
    {
        auto getFileExtension = [](const std::string& s) -> FileType 
        {
            int index = s.find(".");
            if (index != std::string::npos)
            {
                std::string ext = s.substr(index + 1, s.size() - (index + 1));
                std::for_each(ext.begin(), ext.end(), [](char c) 
                    {
                        return std::tolower(c);
                    });

                if (ext == "cpp")
                {
                    return FileType::CPP;
                }
                else if (ext == "h" || ext == "hpp")
                {
                    return FileType::HEADER;
                }
                else if (ext == "png" || ext == "jpg" || ext == "jpeg" || ext == "svg")
                {
                    return FileType::IMG;
                }
                else if (ext == "obj" || ext == "3ds" || ext == "fbx")
                {
                    return FileType::MODEL;
                }
                else if (ext == "txt")
                {
                    return FileType::TXT;
                }
                else // Need to handle more cases as we grow to support more formats
                {
                    return FileType::TXT;
                }
            }
            return FileType::NO_EXT;
        };

        ImGui::Begin("File System Explorer");
        ImVec2 startOfWindow = ImGui::GetCursorPos();
        ImVec2 rectMin = ImGui::GetItemRectMin();
        ImVec2 rectMax = ImGui::GetWindowSize();

        // This next line is needed because the padding on the window isn't accounted
        // for by ImGui, so there are like 10-20 pixels at the left hand side that
        // allow the user to click on, and it will move the viewport. In order to account
        // for this, I subtract 10% of the window width just to be sure that there is no way that 
        // clicking on the padding will move the viewport and 5% on the rhs as well
        rectMin.x -= 0.1 * rectMax.x;
        rectMax.x += 0.05 * rectMax.x;
        Mouse::addBoundingRect("TextEditor", BoundingRect(rectMin.x, rectMin.y, rectMax.x, rectMax.y));
        if (node->isDir)
        {
            ImGui::Image((ImTextureID)folderIconTextureID, ImVec2(16, 16));
            ImGui::SameLine();
            bool nodeOpen = ImGui::TreeNode(node->name.c_str());
            if (nodeOpen)
            {
                for (auto& child : node->children)
                {
                    createFileNode(&child);
                }
                ImGui::TreePop();
            }
        }
        else
        {
            FileType fType = getFileExtension(node->name);
            switch (fType)
            {
            case TXT:
                ImGui::Image((ImTextureID)textIconTextureID, ImVec2(16, 16));
                break;
            case IMG:
                ImGui::Image((ImTextureID)imageIconTextureID, ImVec2(16, 16));
                break;
            case MODEL:
                ImGui::Image((ImTextureID)modelIconTextureID, ImVec2(16, 16));
                break;
            case CPP:
                ImGui::Image((ImTextureID)cppIconTextureID, ImVec2(16, 16));
                break;
            case HEADER:
                ImGui::Image((ImTextureID)headerIconTextureID, ImVec2(16, 16));
                break;
            case NO_EXT:
                ImGui::Image((ImTextureID)textIconTextureID, ImVec2(16, 16));
                break;
            }
            
            ImGui::SameLine();
            ImGui::Text("%s", node->name.c_str());
            if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(0))
            {
                TextEditor::setTextEditorOpen(true);
                std::string path = findFile(treeRoot, node->name);

                std::string fName = rootPathPrefix + path;
                std::ifstream file(fName);
                if (!file)
                {
                    printf("Cannot open the file: %s\n", (rootPathPrefix + fName).c_str());
                    assert(false);
                }
                else
                {
                    TextEditor::setOpenFileName(fName);
                    std::string contents = "";
                    std::string line = "";
                    while (std::getline(file, line))
                    {
                        contents += line + "\n";
                    }
                    TextEditor::setText(contents);
                    file.close();
                }
            }
        }
        ImGui::End();
    }
};