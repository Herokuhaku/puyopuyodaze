#include "Vector2.h"

Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::Vector2(int x, int y)
{
	Vector2::x = x;
	Vector2::y = y;
}

Vector2::~Vector2()
{
}

Vector2& Vector2::operator=(const Vector2& vec)
{
	x = vec.x;
	y = vec.y;

	return *this;
}

Vector2& operator+(const Vector2& vec, const Vector2& vec2)
{
	Vector2 tmp;
	
	tmp.x = vec.x + vec2.x;
	tmp.y = vec.y + vec2.y;

	return tmp;
}

Vector2& operator-(const Vector2& vec, const Vector2& vec2)
{
	Vector2 tmp;

	tmp.x = vec.x - vec2.x;
	tmp.y = vec.y - vec2.y;

	return tmp;
}
