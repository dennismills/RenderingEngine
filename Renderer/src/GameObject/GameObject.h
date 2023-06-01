#pragma once

class GameObject
{
public:
	GameObject() 
	{}

	virtual void serialize() = 0;
	virtual void populateUI() = 0;
	const std::string& getName() const { return name; }

protected:
	void setName(const std::string& s) { name = s; }

private:
	std::string name;
};