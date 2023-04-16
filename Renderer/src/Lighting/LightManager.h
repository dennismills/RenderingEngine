#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include <gl/glew.h>

#include "Light.h"

class LightManager
{
public:
	LightManager();
	const Light& operator[] (unsigned int index);
	const std::string& getName(const std::string name) const { return nameMap.at(name); }
	const unsigned int getLightCount() const { return lightCount; }
	const unsigned int getBindingIndex() const { return bindingIndex; }
	
	void addLight(const Light& light);
	void setNameMapping(const std::string& key, const std::string& value) { nameMap.insert(std::pair<std::string, std::string>(key, value)); }
	void fillLightBuffer();
private:
	std::unordered_map<std::string, std::string> nameMap;
	unsigned int lightCount;
	static const unsigned int MAX_LIGHTS = 128;
	Light lights[MAX_LIGHTS];

	unsigned int uboID;
	unsigned int bindingIndex;
};

