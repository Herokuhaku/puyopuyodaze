#pragma once
#include "Stage.h"

class playUnit
{
public:
	playUnit(Stage& stage);
	~playUnit();
	bool Update(void);
private:
	bool Rota(bool Right);
	Stage* stage_;

	std::map < INPUT_ID, std::function<bool(TrgBool data)>> keymove_;
	int targetID;
	bool check;
};

