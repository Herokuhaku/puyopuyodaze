#pragma once
#include "Stage.h"
#include "playUnit.h"
#include "puyofall.h"
struct puyodrop
{
	bool operator()(Stage& stg) {

			//	stg.ChangeSignal(stg.player_[0]);

			for (auto& pl : stg.player_) {
				pl->SetDropSpeed(2);
			}
			stg.playunit_->Update();   // ‘€ì
		
			puyofall()(stg);
			
		return true;
	}
};