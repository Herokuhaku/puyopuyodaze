#pragma once
#include "Stage.h"

struct puyofall
{
	bool changeSpd = false;
	bool operator()(Stage& stg) {
		stg.nextflag = false;
		stg.rensaflag = false;
		// スピード調節
		bool next = false;
		if (stg.player_.size() >= 2) {
			for (int i = 0; i < 2; i++) {
				stg.ChangeSignal(stg.player_[i]);
				if (!stg.bitdata_._bits.down) {
					changeSpd |= true;
				}
				next |= stg.bitdata_._bits.down == 0;
			}
			for (int i = 0; i < 2; i++) {
				if (changeSpd)stg.player_[i]->SetDropSpeed(5);
			}
		}
		if (next && !stg.coolflag_)
		{
			stg.coolflag_ = true;
			stg.cooldownflag_ = false;
			stg.nextmode_ = stg.stagemode_;
			stg.stagemode_ = StageMode::COOL;
			return true;
		}
		// シグナル見てOKだったらUpdateを呼ぶ elseはそこにぷよを設置
		int count = 0;
			std::for_each(stg.player_.rbegin(), stg.player_.rend(), [&](auto&& puyo) {
				stg.ChangeSignal(puyo);
				if (stg.bitdata_._bits.down)
				{
					if (puyo->GetGrid().y + 1 < STAGE_CHIP_Y && puyo->GetGrid().y + 1 >= 0 &&
						puyo->GetGrid().x + 1 < STAGE_CHIP_X) 
					{
						stg._dataBase_ptr[static_cast<size_t>(puyo->GetGrid().y) + 1][static_cast<size_t>(puyo->GetGrid().x) + 1].reset();
					}
					stg.rensaflag |= puyo->Update(stg.bitdata_);
				}
				else
				{
					if (puyo->GetGrid().y + 1 < STAGE_CHIP_Y && puyo->GetGrid().y + 1 >= 0 &&
						puyo->GetGrid().x + 1 < STAGE_CHIP_X)
					{
						stg._dataBase_ptr[static_cast<size_t>(puyo->GetGrid().y) + 1][static_cast<size_t>(puyo->GetGrid().x) + 1] = puyo;
						stg._dataBase_ptr[static_cast<size_t>(puyo->GetGrid().y) + 1][static_cast<size_t>(puyo->GetGrid().x) + 1]->SetPos(puyo->GetGrid() * stg.size_);
					}
				}
				});
			// どちらか片方が床についていればFALLに移行する
			if (stg.player_.size() >= 2)
			{
				bool next = false;
				for (int i = 0;i < 2;i++) {
					stg.ChangeSignal(stg.player_[i]);
					next |= !stg.bitdata_._bits.down;
				}
				if (next)
				{
					stg.stagemode_ = StageMode::FALL;
				}
			}
			// 連鎖が終わってたらぷよんに移行する
			std::for_each(stg.player_.rbegin(), stg.player_.rend(), [&](auto&& puyo) {
				if (StageMode::FALL == stg.stagemode_) {
					puyo->SetDropSpeed(5);
				}
				if (!stg.rensaflag)
				{
					changeSpd = false;
					stg.nextmode_ = StageMode::ERASE;
					stg.stagemode_ = StageMode::PUYON;
				}
				});

			// 操作ぷよが0の場合新しいぷよを生成する
			if (stg.player_.empty() && stg.result_.first == ResultMode::MAX) {
			changeSpd = false;
			stg.InsertPuyo();
			stg.stagemode_ = StageMode::DROP;
		}
		return true;
	}
private: 
};