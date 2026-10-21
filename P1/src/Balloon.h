#pragma once

class Rectangle;
class Vector2D;
class Balloon
{
private:
	Rectangle* rect;
	Vector2D* vel;
	int color;

public: 
	Balloon();
	~Balloon();
	bool hit(const Rectangle& obj);
};

