#pragma once
#include "Stage.h"
struct puyoerase
{
	bool operator()(Stage& stg) {
		int some = false;
		bool deleteflag = false;
		for (int i = 0; i < STAGE_CHIP_X * STAGE_CHIP_Y; i++)
		{
			if (stg._erasedata[i]) {
				stg._erasedata[i].reset();
			}
		}

		std::for_each(stg.player_.rbegin(), stg.player_.rend(), [&](auto&& puyo) {
			if (!stg.rensaflag || some)
			{
				deleteflag |= stg.ErasePuyo(puyo);
				if (deleteflag) {
					stg.rensaflag = true;
					some = true;
				}
			}
			});
		// ‚Õ‚æ‚ðÁ‚·
		if (deleteflag) {

			auto itr = std::remove_if(stg.player_.begin(), stg.player_.end(), [&](std::shared_ptr<Player>& pl) {
				if (!pl->GetAlive())stg.eraseCount_++;
				return !(pl->GetAlive()); });
			stg.player_.erase(itr, stg.player_.end());
			stg.DeletePuyo();
			stg.player_.shrink_to_fit();
		}
		if (stg.rensaflag)
		{
			stg.nextflag = true;
		}
		if (!stg.nextflag)
		{
			stg.stagemode_ = StageMode::JAMA;
			return true;
		}
		else {
			stg.stagemode_ = StageMode::MUNYON;
			stg.nextmode_ = StageMode::FALL;
			stg.rensa_++;
		}

		return true;
	}
private:
	int count = 0;
};