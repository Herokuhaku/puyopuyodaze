#pragma once
#include "Stage.h"
#include "_debug/_DebugConOut.h"
struct puyopuyon
{
	bool operator()(Stage& stg)
	{
		int count[STAGE_CHIP_X];
		for (int x = 0;x < STAGE_CHIP_X;x++)
		{
			count[x] = 0;
		}

		std::function<void(int, int)> lam = [&](int y, int x) {
			int tmp = y;
			// ‚Õ‚æ‚ñƒtƒ‰ƒO‚ðtrue‚É‚·‚é
			for (int i = y; i < y + no; i++) 
			{
				if (i < STAGE_CHIP_Y)
				{
					if (stg._dataBase_ptr[i][x])
					{
						if (stg._dataBase_ptr[i][x]->GetType() != PUYO_TYPE::WALL &&
							stg._dataBase_ptr[i][x]->GetType() != PUYO_TYPE::OJAMA)
						{
							stg._dataBase_ptr[i][x]->SetPuyonFlag(true);
						}
						else
						{
							break;
						}
					}
				}
			}
			// •Ç‚©‚¨‚¶‚á‚Ü‚ª‚ ‚é‚©”»’è
			bool wallflag = false;
			int wall_y;
			for (int i = y+no; i > 0; i--)
			{
				if (i < STAGE_CHIP_Y-1)
				{
					if (stg._dataBase_ptr[i][x])
					{
						if (stg._dataBase_ptr[i][x]->GetType() == PUYO_TYPE::WALL ||
							stg._dataBase_ptr[i][x]->GetType() == PUYO_TYPE::OJAMA)
						{
							wallflag = true;
							wall_y = i;
						}
					}
				}
			}
			// ‚ ‚ê‚Î‚»‚Ì‰º‚Ì‚Õ‚æ‚ñ‚Í‰ðœ‚·‚é
			if (wallflag) {
				for (int i = y + (no+1); i > 0; i--)
				{
					if (i < STAGE_CHIP_Y-1  && wall_y < i)
					{
						if (stg._dataBase_ptr[i][x])
						{
							if (stg._dataBase_ptr[i][x]->GetType() != PUYO_TYPE::WALL ||
								stg._dataBase_ptr[i][x]->GetType() != PUYO_TYPE::OJAMA)
							{
								stg._dataBase_ptr[i][x]->SetPuyonFlag(false);
							}
							else
							{
								break;
							}
						}
					}
				}
			}


		for (int i = y; i < STAGE_CHIP_Y; i++)
			{
				if (i < STAGE_CHIP_Y && stg._dataBase_ptr[i][x])
				{
					if (stg._dataBase_ptr[i][x]->GetPuyonFlag())
					{
						count[x]++;
					}
					else {
						break;
					}
				}
			}

			for (int i = y; i < STAGE_CHIP_Y; i++) {
				if (count[x] > 0 && i < STAGE_CHIP_Y && stg._dataBase_ptr[i][x]) {
					stg._dataBase_ptr[i][x]->SetCountflag(count[x]--);
				}
			}
			//	else if(i < STAGE_CHIP_Y && stg._dataBase_ptr[i][x]){
			//		stg._dataBase_ptr[i][x]->SetPuyonFlag(false);
			//	}
			//}
		};

		bool old_[STAGE_CHIP_X] = { 0,0,0,0,0,0,0,0 };

		if (stg.puyonCnt_ == 15 && !puyon_)
		{
			for (int y = 0; y < STAGE_CHIP_Y; y++) {
				for (int x = 0; x < STAGE_CHIP_X; x++) {
					if (stg._dataBase_ptr[y][x] && stg._dataBase_ptr[y][x]->GetPuyonFlag() && !old_[x])
					{
						lam(y, x);
						old_[x] = true;
					}
				}
			}
			puyon_ = true;
		}	
		if (++stg.puyonCnt_ >= 45) {
			stg.stagemode_ = StageMode::MUNYON;
			stg.nextmode_ = StageMode::ERASE;
			stg.puyonCnt_ = 15;
			puyon_ = false;
			for (auto& pl : stg.player_) {
				pl->SetPuyonFlag(false);
			}
		}
		//for (auto& pl : stg.player_)
		//{
		//	bool tmpuyon = false;
		//	for (int i = pl->GetGrid().y + 1; i >= 0; i--) {
		//		if (stg._dataBase_ptr[i][pl->GetGrid().x + 1] && !stg._dataBase_ptr[i][pl->GetGrid().x + 1]->GetPuyonFlag())
		//		{
		//			tmpuyon = true;
		//		}
		//	}
		//	if (tmpuyon) {
		//		pl->SetPuyonFlag(false);
		//	}
		//}
	return true;
}
private:
	static constexpr int no = 4;
	bool puyon_ = false;
};