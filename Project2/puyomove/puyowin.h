#pragma once
#include "Stage.h"
#include "puyofall.h"

struct puyowin
{
	bool operator()(Stage& stg)
	{
		SetDrawScreen(DX_SCREEN_BACK);
		if (frame_ >= 150+75)
		{
			int tmpl = abs((count_+200) % 400 - 200) -100;
			int tmpr = abs((count_) % 400 - 200) -100;
			Vector2 s = stg.stageSize;
			Vector2 o = stg.offset_;
			DrawModiGraph(
				o.x + s.x / 2 - 100+200 + tmpl-100, o.y + s.y / 3 - 50,
				o.x + s.x / 2 + 100 + tmpr-100, o.y + s.y / 3 - 50,
				o.x + s.x / 2 + 100 + tmpr-100, o.y + s.y / 3 + 50,
				o.x + s.x / 2 - 100 +200+ tmpl-100, o.y + s.y / 3 + 50,
				stg.image_[ResultMode::WIN],true);
			count_++;
		}
		else
		{
			DrawRotaGraph(stg.offset_.x + stg.stageSize.x / 2, stg.offset_.y + stg.stageSize.y / 3 + abs((frame_) % 150 - 75) / 2, 1.0f, 0.0f, stg.image_[ResultMode::WIN], true);
		}
		frame_++;
		return true;
	}
private:
	int frame_ = 75;
	int count_ = 0;
};