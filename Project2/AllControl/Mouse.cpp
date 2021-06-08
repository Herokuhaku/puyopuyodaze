#include "Mouse.h"
#include "../Scene/SceneMng.h"
ControlType Mouse::GetType(void)
{
	return ControlType::MOUSE;
}

bool Mouse::SetUp(int no)
{
	_keyList.try_emplace(INPUT_ID::UP, static_cast<int>(INPUT_ID::UP));
	_keyList.try_emplace(INPUT_ID::DOWN, static_cast<int>(INPUT_ID::DOWN));
	_keyList.try_emplace(INPUT_ID::LEFT, static_cast<int>(INPUT_ID::LEFT));
	_keyList.try_emplace(INPUT_ID::RIGHT, static_cast<int>(INPUT_ID::RIGHT));
	_keyList.try_emplace(INPUT_ID::L_ROTA, static_cast<int>(INPUT_ID::L_ROTA));
	_keyList.try_emplace(INPUT_ID::R_ROTA, static_cast<int>(INPUT_ID::R_ROTA));
	_keyList.try_emplace(INPUT_ID::HOLD, static_cast<int>(INPUT_ID::HOLD));

	Vector2 mouse;
	_center = lpSceneMng.GetScreenSize() / 2;
	GetMousePoint(&mouse.x, &mouse.y);
	pos_.x = mouse.x;
	pos_.y = mouse.y;
	
	return true;
}

void Mouse::Update(void)
{
	Vector2 mouse;
	GetMousePoint(&mouse.x, &mouse.y);

	for (auto data : INPUT_ID())
	{
		_data[data][static_cast<int>(Trg::Old)] = _data[data][static_cast<int>(Trg::Now)];
	}

	_data[INPUT_ID::L_ROTA][static_cast<int>(Trg::Now)] = GetMouseInput() == MOUSE_INPUT_LEFT;
	_data[INPUT_ID::R_ROTA][static_cast<int>(Trg::Now)] = GetMouseInput() == MOUSE_INPUT_RIGHT;
	_data[INPUT_ID::HOLD][static_cast<int>(Trg::Now)] = GetMouseInput() == MOUSE_INPUT_MIDDLE;

	if (_data[INPUT_ID::LEFT][static_cast<int>(Trg::Now)] == 0 && mouse.x - pos_.x <= -150)
	{
		_data[INPUT_ID::LEFT][static_cast<int>(Trg::Now)] = 1;
	}
	else if(_data[INPUT_ID::LEFT][static_cast<int>(Trg::Now)])
	{
		_data[INPUT_ID::LEFT][static_cast<int>(Trg::Now)] = 0;
		pos_.x = mouse.x;
	}
	if (_data[INPUT_ID::RIGHT][static_cast<int>(Trg::Now)] == 0 && pos_.x - mouse.x <= -150)
	{
		_data[INPUT_ID::RIGHT][static_cast<int>(Trg::Now)] = 1;
	}
	else if (_data[INPUT_ID::RIGHT][static_cast<int>(Trg::Now)])
	{
		_data[INPUT_ID::RIGHT][static_cast<int>(Trg::Now)] = 0;
		pos_.x = mouse.x;
	}
	//if (_data[INPUT_ID::RIGHT][static_cast<int>(Trg::Now)] == 0 && pos_.x - mouse.x <= 300)
	//{
	//	_data[INPUT_ID::RIGHT][static_cast<int>(Trg::Now)] = 1;
	//	oldpos_ = pos_;
	//}
	//else if (_data[INPUT_ID::RIGHT][static_cast<int>(Trg::Now)] == 1 &&
	//	abs(pos_.x - oldpos_.x) <= 300)
	//{
	//	_data[INPUT_ID::RIGHT][static_cast<int>(Trg::Now)] = 0;
	//	oldpos_ = pos_;
	//}
	/*if (_center.x - 300 >= mouse.x)
	{
		_data[INPUT_ID::LEFT][static_cast<int>(Trg::Now)] = 1;
	}
	else {
		_data[INPUT_ID::LEFT][static_cast<int>(Trg::Now)] = 0;
	}
	if (_center.x + 300 <= mouse.x)
	{
		_data[INPUT_ID::RIGHT][static_cast<int>(Trg::Now)] = 1;
	}
	else
	{
		_data[INPUT_ID::RIGHT][static_cast<int>(Trg::Now)] = 0;
	}*/
}
