#pragma once
#include "Stage.h"
#include "../EffectMng.h"

struct puyojama
{
	bool operator()(Stage& stg) {
		int send = (stg.rensaMax_ / 2) * (stg.rensa_ - 1) * (stg.eraseCount_ / 8);
		// Ž©•ª‚ÉŒü‚¢‚Ä‚¢‚é‚¨‚¶‚á‚Ü‚ª‚ ‚ê‚Î‘ŠŽE‚ð‚·‚é
		if (stg.ojamalist_.size() >= 1) {
			for (; send >= 1 && stg.ojamalist_.size() >= 1; send--) {
				lpEffectMng.PlayEffect("Sumple2", Vector2(stg.offset_.x + stg.ojamalist_.back()->GetPos().x + stg.size_ / 2, 
					stg.offset_.y + stg.ojamalist_.back()->GetPos().y - stg.size_/2));
				stg.ojamalist_.pop_back();
			}
		}
		if (stg.ojamalist_.size() > 0 && !stg.ojamaflag_) {
			int count = 0;
			for (auto& ojama : stg.ojamalist_) {
				if (count < 6 * 5) {
					ojama->SetStayFlag(false);
					stg.player_.emplace_back(ojama);
					count++;
				}
				else
				{
					break;
				}
			}
			stg.ojamalist_.erase(stg.ojamalist_.begin(), std::next(stg.ojamalist_.begin(), count));
			//int no = 0;
			//for (auto& ojama : stg.ojamalist_) {
			//	ojama->SetPos(Vector2((no * stg.size_) % (stg.size_ * 6),
			//		(-stg.size_ * (no / 6))));
			//	no++;
			//}
			stg.stagemode_ = StageMode::FALL;
			stg.ojamaflag_ = true;
			return true;
		}
		stg.stagemode_ = StageMode::DROP;
		stg.holdflag_ = false;
		stg.ojamaflag_ = false;
		stg.coolflag_ = false;
		stg.InsertPuyo();
		auto data = [&](int x) 
		{
			for (auto& no : stg.num){
				if (no == x)
				{
					return true;
				}
			}
			return false;
		
		};
		for (int i = 0; i < send; i++)
		{
			if (i != 0 && i % 6 == 0)
			{
				stg.num.fill(-1);
			}
			int tmp = rand() % 6;
			while (data(tmp))
			{
				tmp = rand() % 6;
			}
			stg.num[i % 6] = tmp;

			stg.sendojama_.emplace_back(std::make_shared<OjamaPuyo>(Vector2((tmp * stg.size_) % (stg.size_ * 6),
				(-stg.size_ * (static_cast<int>(stg.sendojama_.size()) / 6))),
				stg.size_, std::move(stg._puyoimage), PUYO_TYPE::OJAMA));
		}
		if (stg.rensaMax_ < stg.rensa_)
		{
			stg.rensaMax_ = stg.rensa_;
		}
		stg.rensa_ = 0;
		stg.eraseCount_ = 0;
		if (stg.sendojama_.size() != 0)
		{
			stg.nextojama_ = true;
		}
		if (stg._dataBase_ptr[2][4]) {
			return false;
		}
		return true;
	}
};