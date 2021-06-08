#pragma once
#include "Control.h"
#include "../common/Vector2.h"
//#include <array>

class Mouse:
	public Control
{
public:
	ControlType GetType(void)override;
	bool SetUp(int no)override;
	void Update(void)override;
private:
	Vector2 _center = {};
	Vector2 pos_ = {};
	Vector2 oldpos_ = {};
	std::array<char, 2> _keyData = {};
};

