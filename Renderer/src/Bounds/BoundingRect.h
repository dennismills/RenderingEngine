#pragma once

class BoundingRect
{
public:
	BoundingRect() {}
	BoundingRect(float x, float y, float w, float h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	bool isInside(float xPos, float yPos)
	{
		return (xPos > x && xPos < x + w && yPos > y && yPos < y + h);
	}

private:
	float x, y, w, h;
};
