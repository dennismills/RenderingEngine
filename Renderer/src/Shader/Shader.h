#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Lighting/LightManager.h"

#define makeArray(str, index) (str + "[" + std::to_string(index) + "]")

class Shader
{
public:
	typedef GLuint glint;

	Shader() {}

	Shader(std::string vertFileName, std::string fragFileName)
	{
		typedef unsigned int uint;
		vertContents = readAllIntoStr(vertFileName);
		fragContents = readAllIntoStr(fragFileName);

		writePreprocessedShaderToFile(vertFileName, vertContents);
		writePreprocessedShaderToFile(fragFileName, fragContents);

		const char* vertContentsC = vertContents.c_str();
		const char* fragContentsC = fragContents.c_str();

		uint vertexID = createShader(GL_VERTEX_SHADER, vertContentsC);
		uint fragmentID = createShader(GL_FRAGMENT_SHADER, fragContentsC);

		programID = glCreateProgram();
		glAttachShader(programID, vertexID);
		glAttachShader(programID, fragmentID);
		glLinkProgram(programID);

		int success;
		const uint bufferSize = 512;
		char programLog[bufferSize + 1];
		memset(programLog, 0, bufferSize + 1);

		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(programID, bufferSize, NULL, programLog);
			std::cout << "============ Program Log Output ============" << std::endl;
			std::cout << programLog << std::endl;
		}

		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);
	}
	void use()
	{
		glUseProgram(programID);
	}
	static void unbind()
	{
		glUseProgram(0);
	}

public:
	void setUniformInt(const int& i, std::string name)
	{
		glint loc = getID(programID, name);
		glUniform1i(loc, i);
	}
	void setUniformFloat(const float& f, std::string name)
	{
		glint loc = getID(programID, name);
		glUniform1f(loc, f);
	}
	void setUniformVec3(const glm::vec3& vec, std::string name)
	{
		glint loc = getID(programID, name);
		glUniform3fv(loc, 1, glm::value_ptr(vec));
	}
	void setUniformMat4(const glm::mat4& mat, std::string name)
	{
		glint loc = getID(programID, name);
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
	}
	void setUBO(LightManager& manager, std::string name)
	{
		GLuint lightBlockIndex = glGetUniformBlockIndex(programID, name.c_str());
		if (lightBlockIndex < 0)
		{
			printf("Cannot create uniform block index\n");
			assert(false);
		}
		glUniformBlockBinding(programID, lightBlockIndex, manager.getBindingIndex());
	}
private:
	void writePreprocessedShaderToFile(std::string fileName, std::string source)
	{
		std::size_t rFindSlashIndex = fileName.rfind("/");
		std::string fName = fileName.substr(rFindSlashIndex + 1);
		std::string prefix = fileName.substr(0, rFindSlashIndex);
		std::string outputFileName = prefix + "/Preprocessed/" + fName;
		std::ofstream outputShader(outputFileName);
		
		if (!outputShader)
		{
			printf("Cannot create file: %s\n", outputFileName.c_str());
			assert(false);
		}
		outputShader << source;
		outputShader.close();
	}
	
	// TODO: Build graph from file names and make sure no cycles exist
	std::string readAllIntoStr(std::string fileName)
	{
		std::ifstream file(fileName);
		if (!file)
		{
			std::cout << "Cannot open the file: " << fileName << std::endl;
			assert(false);
		}

		std::string temp;
		std::string contents;
		while (std::getline(file, temp))
		{
			// If we see that we are including another file
			std::size_t includeIndex = temp.find("#include");
			if (includeIndex != std::string::npos)
			{
				// only supports format of `#include <fileName>`
				std::size_t startIndex = temp.find("<");
				std::size_t endIndex = temp.find(">");
				assert(startIndex != std::string::npos);
				assert(endIndex != std::string::npos);
				assert(startIndex < endIndex);
				std::string includeFileName = temp.substr(startIndex + 1, endIndex - (startIndex + 1));

				assert(includeFileName[0] == '"');
				assert(includeFileName[includeFileName.size() - 1] == '"');

				includeFileName = includeFileName.substr(1, includeFileName.size() - 2); // trims the string of the quotes 
				
				std::size_t totalLength = endIndex - includeIndex + 1; // Computes the length of the string
				temp.replace(includeIndex, totalLength, readAllIntoStr(includeFileName));
			}
			contents += temp + "\n";
		}
		file.close();
		return contents;
	}
	unsigned int createShader(int shaderType, const char* src)
	{
		const unsigned int bufferSize = 512;
		int success;

		char infoLog[bufferSize + 1];
		memset(infoLog, 0, sizeof(char) * bufferSize + 1);

		unsigned int shaderID = glCreateShader(shaderType);
		glShaderSource(shaderID, 1, &src, NULL);
		glCompileShader(shaderID);

		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderID, bufferSize, NULL, infoLog);
			if (shaderType == GL_VERTEX_SHADER)
			{
				std::cout << "============ Vertex Shader Log Output ============" << std::endl;
			}
			if (shaderType == GL_FRAGMENT_SHADER)
			{
				std::cout << "============ Fragment Shader Log Output ============" << std::endl;
			}
			std::cout << infoLog << std::endl;
		}

		return shaderID;
	}
	glint getID(glint prog, std::string name)
	{
		glint loc = glGetUniformLocation(prog, name.c_str());
		if (loc < 0)
		{
			std::cout << "Cannot find variable: " << name << " in the program. (getID -- Shader.h)" << std::endl;
		}

		return loc;
	}
private:
	std::string vertContents;
	std::string fragContents;

	unsigned int programID;
};