#pragma once
#include "common/Vector2.h"

class Frame
{
public:
	Frame(int frameNo);
	~Frame();
	void Draw(Vector2 offset, double rad);

private:
	void Init(void);
	int screenID_;
	Vector2 pos_;
	Vector2 size_;
	int frameNo_;
	int hundle;
};

