#pragma once
#include "Stage.h"
struct puyorensa
{
	void operator()(Stage& stg) 
	{
		bool nextflag = false;
		bool rensaflag = false;

		std::for_each(stg.player_.rbegin(), stg.player_.rend(), [&](auto&& puyo) {
			stg.ChangeSignal(puyo);
			if (stg.bitdata_._bits.down)
			{
				stg._dataBase_ptr[puyo->GetGrid().y + 1][puyo->GetGrid().x + 1].reset();
				rensaflag |= puyo->Update(stg.bitdata_);
			}
			else
			{
				stg._dataBase_ptr[puyo->GetGrid().y + 1][puyo->GetGrid().x + 1] = puyo;
			}
			});
		std::for_each(stg.player_.rbegin(), stg.player_.rend(), [&](auto&& puyo) {
			if (!rensaflag)
			{
				if (stg.ErasePuyo(puyo)) {
					rensaflag = true;
				}
			}
			});
		if (rensaflag) 
		{
			nextflag = true;
		}
		if (!nextflag)
		{
			stg.InsertPuyo();
			stg.stagemode_ = StageMode::DROP;
		}
	}
};