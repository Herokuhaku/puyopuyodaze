#pragma once
class Vector2
{
public:
	Vector2();
	Vector2(int x,int y);
	~Vector2();
	int x;
	int y;

	Vector2& operator =(const Vector2& vec);
};

Vector2& operator +(const Vector2& vec, const Vector2& vec2);
Vector2& operator -(const Vector2& vec, const Vector2& vec2);