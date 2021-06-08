#pragma once
#include "Stage.h"

struct puyomunyon
{
	void FallCheck(Stage& stg) {
		if (stg.nextmode_ == StageMode::FALL)
		{
			std::for_each(stg.player_.rbegin(), stg.player_.rend(), [&](auto&& puyo) {
				stg.ChangeSignal(puyo);
				if (stg.bitdata_._bits.down)
				{
					if (puyo->GetGrid().y + 1 < STAGE_CHIP_Y && puyo->GetGrid().y + 1 >= 0 &&
						puyo->GetGrid().x < STAGE_CHIP_X && puyo->GetGrid().x >= 0) {
						stg._dataBase_ptr[static_cast<size_t>(puyo->GetGrid().y) + 1][static_cast<size_t>(puyo->GetGrid().x)+ 1].reset();
					}
				}
				});
		}
	}
	bool operator()(Stage& stg)
	{
		// ‚Þ‚É‚å‚ñ‚ªˆê“xŠO‚ê‚é‚½‚ß
		FallCheck(stg);

		std::for_each(stg.player_.rbegin(), stg.player_.rend(), [&](auto&& pl) {
			BitData munyon_;
			munyon_.flag = 0;
			if (pl->GetGrid().y + 1 < STAGE_CHIP_Y && pl->GetGrid().y + 1 >= 0 &&
				pl->GetGrid().x + 1< STAGE_CHIP_X && pl->GetGrid().x  + 1>= 0) 
			{
				if (stg._dataBase_ptr[static_cast<size_t>(pl->GetGrid().y) + 1][static_cast<size_t>(pl->GetGrid().x) + 1])
				{
					PUYO_TYPE tmptype = stg._dataBase_ptr[static_cast<size_t>(pl->GetGrid().y) + 1][static_cast<size_t>(pl->GetGrid().x )+ 1]->GetType();
					if (tmptype != PUYO_TYPE::OJAMA)
					{
						if (stg._dataBase_ptr[static_cast<size_t>(pl->GetGrid().y)][static_cast<size_t>(pl->GetGrid().x) + 1])
						{
							munyon_._bits.up = (stg._dataBase_ptr[static_cast<size_t>(pl->GetGrid().y)][static_cast<size_t>(pl->GetGrid().x) + 1]->GetType() == tmptype);
						}

						if (stg._dataBase_ptr[static_cast<size_t>(pl->GetGrid().y) + 2][static_cast<size_t>(pl->GetGrid().x) + 1])
						{
							munyon_._bits.down = (stg._dataBase_ptr[static_cast<size_t>(pl->GetGrid().y) + 2][static_cast<size_t>(pl->GetGrid().x) + 1]->GetType() == tmptype);
						}

						if (stg._dataBase_ptr[static_cast<size_t>(pl->GetGrid().y) + 1][pl->GetGrid().x])
						{
							munyon_._bits.left = (stg._dataBase_ptr[static_cast<size_t>(pl->GetGrid().y) + 1][pl->GetGrid().x]->GetType() == tmptype);
						}

						if (stg._dataBase_ptr[static_cast<size_t>(pl->GetGrid().y) + 1][pl->GetGrid().x + 2])
						{
							munyon_._bits.right = (stg._dataBase_ptr[static_cast<size_t>(pl->GetGrid().y) + 1][static_cast<size_t>(pl->GetGrid().x) + 2]->GetType() == tmptype);
						}
						pl->SetMunyon(munyon_);
					}
				}
			}
			});
	//	stg.stagemode_ = StageMode::ERASE;
		stg.stagemode_ = stg.nextmode_;
		return true;
	}
};