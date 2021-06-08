#include "Control.h"
#include "Control.h"

Control::Control()
{
}

Control::~Control()
{
}

std::map<int, std::pair<int, int>> Control::GetState(void)
{
	return _keystate;
}

std::map<INPUT_ID, int> Control::GetkeyList(void)
{
	return _keyList;
}
