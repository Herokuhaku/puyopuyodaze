#pragma once
#include "Stage.h"

struct puyolose
{
	bool operator()(Stage & stg)
	{
		SetDrawScreen(DX_SCREEN_BACK);
		std::for_each(stg.player_.rbegin(), stg.player_.rend(), [&](auto&& puyo) {
			stg.ChangeSignal(puyo);
			if (stg.bitdata_._bits.down)
			{
				if (puyo->GetGrid().y + 1 < STAGE_CHIP_Y && puyo->GetGrid().y + 1 >= 0 &&
					puyo->GetGrid().x + 1 < STAGE_CHIP_X && stg._dataBase_ptr[static_cast<size_t>(puyo->GetGrid().y) + 1][puyo->GetGrid().x + 1])
				{
					stg._dataBase_ptr[static_cast<size_t>(puyo->GetGrid().y) + 1][puyo->GetGrid().x + 1].reset();
				}
				puyo->SetDropSpeed(7);
				stg.rensaflag |= puyo->Update(stg.bitdata_);
			}
			});
		if (frame++ % rag == 0) {
			if (4 + (frame / rag) < STAGE_CHIP_X && 0 < 4 + (frame / rag))
			{
				stg._dataBase_ptr[STAGE_CHIP_Y - 1][4 + (frame / rag)].reset();
				stg._dataBase_ptr[STAGE_CHIP_Y - 1][4 + (-frame / rag)].reset();
			}
		}
		DrawRotaGraph(stg.offset_.x + stg.stageSize.x / 2 + 100,stg.offset_.y + stg.stageSize.y / 3 + abs((frame) % 150 - 75) / 2, 1.0f, 0.5f,stg.image_[ResultMode::LOSE], true);

		stg.pos_.y += 15;
		if (stg.rad_ <= 1.0f) {
			stg.rad_ += 0.05f;
		}
		return true;
	}
private:
	int frame = 0;
	static constexpr int rag = 15;
};