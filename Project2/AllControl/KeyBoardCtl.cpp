#include "KeyBoardCtl.h"
#include <DxLib.h>

ControlType KeyBoard::GetType(void)
{
	return ControlType::KEY;
}

bool KeyBoard::SetUp(int no)
{
	if (no == 0) {
		_keyList.try_emplace(INPUT_ID::UP, KEY_INPUT_W);
		_keyList.try_emplace(INPUT_ID::DOWN, KEY_INPUT_S);
		_keyList.try_emplace(INPUT_ID::LEFT, KEY_INPUT_A);
		_keyList.try_emplace(INPUT_ID::RIGHT, KEY_INPUT_D);
		_keyList.try_emplace(INPUT_ID::L_ROTA, KEY_INPUT_Q);
		_keyList.try_emplace(INPUT_ID::R_ROTA, KEY_INPUT_E);
		_keyList.try_emplace(INPUT_ID::HOLD, KEY_INPUT_Z);
	}
	else {
		_keyList = {
			{INPUT_ID::UP ,KEY_INPUT_NUMPAD8},
			{INPUT_ID::DOWN,KEY_INPUT_NUMPAD5},
			{INPUT_ID::LEFT,KEY_INPUT_NUMPAD4},
			{INPUT_ID::RIGHT,KEY_INPUT_NUMPAD6},
			{INPUT_ID::L_ROTA,KEY_INPUT_NUMPAD7},
			{INPUT_ID::R_ROTA,KEY_INPUT_NUMPAD9},
			{INPUT_ID::HOLD,KEY_INPUT_NUMPAD1},
		};
	}
	_keyData.fill(0);
	return true;
}

void KeyBoard::Update(void)
{
	GetHitKeyStateAll(_keyData.data());
	for (auto id : INPUT_ID())
	{
		_data[id][static_cast<int>(Trg::Old)] = _data[id][static_cast<int>(Trg::Now)];
		_data[id][static_cast<int>(Trg::Now)] = _keyData[_keyList[id]];
	}
}
