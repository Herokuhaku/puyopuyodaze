#include "playUnit.h"

playUnit::playUnit(Stage& stage)
{
	stage_ = &stage;
	targetID = 0;
	check = true;
	keymove_.try_emplace(INPUT_ID::UP, [&](TrgBool data) {
		/*if (data[static_cast<int>(Trg::Now)] && !data[static_cast<int>(Trg::Old)])
		{
			for (int i = 0; i < 2; i++) {
				(*stage_).ChangeSignal((*stage_).player_[i]);
				check &= (*stage_).bitdata_._bits.up;
			}
			if (check) {
				(*stage_).player_[1]->Move(INPUT_ID::UP, (*stage_).bitdata_);
				(*stage_).player_[0]->Move(INPUT_ID::UP, (*stage_).bitdata_);
			}
		}*/
		return false;
		});
	keymove_.try_emplace(INPUT_ID::LEFT, [&](TrgBool data) {
		if (data[static_cast<int>(Trg::Now)] && !data[static_cast<int>(Trg::Old)])
		{
			for (int i = 0; i < 2; i++) {
				(*stage_)._dataBase_ptr[static_cast<size_t>((*stage_).player_[i]->GetGrid().y)+1][static_cast<size_t>((*stage_).player_[i]->GetGrid().x) + 1].reset();
				(*stage_).ChangeSignal((*stage_).player_[i]);
				check &= (*stage_).bitdata_._bits.left;
			}
				check &= (*stage_).player_[0]->GetPos().y == (*stage_).player_[1]->GetPos().y ||
					(*stage_).player_[0]->GetPos().x == (*stage_).player_[1]->GetPos().x;

			if (check) {
				(*stage_).player_[1]->Move(INPUT_ID::LEFT, (*stage_).bitdata_);
				(*stage_).player_[0]->Move(INPUT_ID::LEFT, (*stage_).bitdata_);
				return true;
			}
		}
		return false;
		});	
	keymove_.try_emplace(INPUT_ID::RIGHT, [&](TrgBool data) {
			if (data[static_cast<int>(Trg::Now)] && !data[static_cast<int>(Trg::Old)]){
				for (int i = 0; i < 2; i++) {
					(*stage_)._dataBase_ptr[(*stage_).player_[i]->GetGrid().y + 1][(*stage_).player_[i]->GetGrid().x + 1].reset();
					(*stage_).ChangeSignal((*stage_).player_[i]);
					check &= (*stage_).bitdata_._bits.right;
				}

				check &= (*stage_).player_[0]->GetPos().y == (*stage_).player_[1]->GetPos().y ||
					(*stage_).player_[0]->GetPos().x == (*stage_).player_[1]->GetPos().x;
				if (check) {
					(*stage_).player_[1]->Move(INPUT_ID::RIGHT, (*stage_).bitdata_);
					(*stage_).player_[0]->Move(INPUT_ID::RIGHT, (*stage_).bitdata_);
					return true;
				}
			}
			return false;
			});
	keymove_.try_emplace(INPUT_ID::DOWN, [&](TrgBool data) {
	if (data[static_cast<int>(Trg::Now)] && data[static_cast<int>(Trg::Old)]){
		for (int i = 0; i < 2; i++) {
			(*stage_).ChangeSignal((*stage_).player_[i]);
			check &= (*stage_).bitdata_._bits.down;
		}
		if (check) {
			(*stage_).player_[0]->Move(INPUT_ID::DOWN, (*stage_).bitdata_);
			(*stage_).ChangeSignal((*stage_).player_[1]);
			(*stage_).player_[1]->Move(INPUT_ID::DOWN, (*stage_).bitdata_);
			(*stage_).cooldownflag_ = true;
			return true;
		}
		//(*stage_).player_[0]->Move(INPUT_ID::DOWN, (*stage_).bitdata_);
		//(*stage_).ChangeSignal((*stage_).player_[1]);
		//(*stage_).player_[1]->Move(INPUT_ID::DOWN, (*stage_).bitdata_);
		}
	return false;
	});
	keymove_.try_emplace(INPUT_ID::L_ROTA, [&](TrgBool data) {
			if (data[static_cast<int>(Trg::Now)] && !data[static_cast<int>(Trg::Old)])
			{
				return Rota(false);
			}
			return false;
			});
	keymove_.try_emplace(INPUT_ID::R_ROTA, [&](TrgBool data) {
			if (data[static_cast<int>(Trg::Now)] && !data[static_cast<int>(Trg::Old)])
			{
				return Rota(true);
			}
			return false;
		});
	keymove_.try_emplace(INPUT_ID::HOLD, [&](TrgBool data) {

		if (data[static_cast<int>(Trg::Now)] && !data[static_cast<int>(Trg::Old)]) {
			if ((*stage_).stagemode_ == StageMode::DROP || (*stage_).stagemode_ == StageMode::COOL)
			{
				if (!(*stage_).holdflag_) {
					if ((*stage_).hold_->GetHold().first->GetType() == PUYO_TYPE::NON)
					{
						(*stage_).hold_->SetHold({ (*stage_).player_[0],(*stage_).player_[1] });
						int count = 0;
						auto itr = std::remove_if((*stage_).player_.begin(), (*stage_).player_.end(), [&](auto&& pl) {return count++ < 2; });
						(*stage_).player_.erase(itr, (*stage_).player_.end());
						(*stage_).InsertPuyo();
					}
					else
					{
						nextPair tmp = { (*stage_).player_[0], (*stage_).player_[1] };

						(*stage_).player_[0] = (*stage_).hold_->GetHold().first;
						(*stage_).player_[1] = (*stage_).hold_->GetHold().second;

						(*stage_).player_[0]->SetPos(Vector2(48 * 3, 0));
						(*stage_).player_[1]->SetPos(Vector2(48 * 3, 48));

						(*stage_).hold_->SetHold(tmp);

					}
					(*stage_).holdflag_ = true;
				}
			}
		}
		return (*stage_).holdflag_;
		});
}

playUnit::~playUnit()
{
}

bool playUnit::Update(void)
{
	bool flag = false;
	if ((*stage_).player_.size() >= 2) {

		for (auto data : (*stage_).controller_->GetCntData())
		{
			check = true;
			flag |= keymove_[data.first](data.second);
		}
	}
	return flag;
}

bool playUnit::Rota(bool Right)
{
	int tmpsize_ = -(*stage_).size_;
	if (Right) {
		tmpsize_ = -tmpsize_;
	}

	Vector2 p0 = (*stage_).player_[0]->GetPos();
	Vector2 p1 = (*stage_).player_[1]->GetPos();

	Vector2 tmppos[2] = { {-1,-1},{-1,-1} };

	auto data = (*stage_)._dataBase_ptr;
	Vector2 cpos[2];	// checkPos
	auto ccpos = [&](int i) {
		cpos[i] = { tmppos[i].x / (*stage_).size_ + 1,(tmppos[i].y + (*stage_).size_ - 1) / (*stage_).size_ + 1};
	};

	auto cInit = [&]() {
		for (int i = 0;i < 2;i++) {
			ccpos(i);
	}

		};
	// 0が下の場合
	if (p0.y > p1.y)
	{
		tmppos[1] = Vector2(p0.x + tmpsize_, p0.y);
		tmppos[0] = p0;
		cInit();

		// ずれる前に
		if (data[cpos[1].y][cpos[1].x] && !data[cpos[1].y-1][cpos[1].x])
		{
			tmpsize_ = -(*stage_).size_;
			tmppos[0].y += tmpsize_;
			tmppos[1].y += tmpsize_;
			if (Right) {
				tmpsize_ = -tmpsize_;
			}
		}
		else if (Right)
		{
			if (data[cpos[1].y][cpos[1].x] && data[cpos[1].y - 1][cpos[1].x] && !data[cpos[1].y - 1][cpos[1].x - 1] && !data[cpos[1].y - 1][cpos[1].x - 2])
			{
				tmppos[0].y += -tmpsize_;
				tmppos[1].y += -tmpsize_;

				tmppos[0].x += -tmpsize_;
				tmppos[1].x += -tmpsize_;
			}
		}
		else if (!Right)
		{
			if (data[cpos[1].y][cpos[1].x] && data[cpos[1].y - 1][cpos[1].x] && !data[cpos[1].y - 1][cpos[1].x + 1] && !data[cpos[1].y - 1][cpos[1].x + 2])
			{
				tmppos[0].y += tmpsize_;
				tmppos[1].y += tmpsize_;
				
				tmpsize_ = -tmpsize_;

				tmppos[0].x += tmpsize_;
				tmppos[1].x += tmpsize_;
			}
		}
		cInit();
		// ずれる
		if (data[cpos[1].y][cpos[1].x] || data[cpos[0].y][cpos[0].x])
		{
			tmppos[0].x += -tmpsize_;
			tmppos[1].x += -tmpsize_;
		}
		cInit();

		// それでも埋まっていたら上と下の位置を戻した後上下を入れ替える
		if (data[cpos[1].y][cpos[1].x] || data[cpos[0].y][cpos[0].x])
		{
			tmppos[1] = p1;
			tmppos[0] = p0;
			PUYO_TYPE type = (*stage_).player_[0]->GetType();
			(*stage_).player_[0]->SetType((*stage_).player_[1]->GetType());
			(*stage_).player_[1]->SetType(type);
		}
	}
	// 0が右の場合
	else if (p0.x > p1.x)
	{
		tmppos[0] = p0;
		tmppos[1] = Vector2(p0.x, p1.y - tmpsize_);

		cInit();
		// 上にずれる
		if (data[cpos[1].y][cpos[1].x] || data[cpos[0].y][cpos[0].x])
		{
			tmppos[0].y += tmpsize_;
			tmppos[1].y += tmpsize_;

			cInit();
		}

		if (data[cpos[1].y][cpos[1].x] || data[cpos[0].y][cpos[0].x])
		{
			tmppos[1] = p1;
			tmppos[0] = p0;
		}
	}
	// 0が上の場合
	else if (p0.y < p1.y)
	{
		tmppos[0] = p0;
		tmppos[1] = Vector2(p0.x - tmpsize_, p0.y);

		cInit();

		if (data[cpos[1].y][cpos[1].x] && !data[cpos[1].y - 1][cpos[1].x])
		{
			tmpsize_ = -(*stage_).size_;
			tmppos[0].y += tmpsize_;
			tmppos[1].y += tmpsize_;
			if (Right) {
				tmpsize_ = -tmpsize_;
			}
		}
		else if (Right)
		{
			if (data[cpos[1].y][cpos[1].x] && data[cpos[1].y - 1][cpos[1].x] && !data[cpos[1].y - 1][cpos[1].x + 1] && !data[cpos[1].y - 1][cpos[1].x + 2])
			{
				tmppos[0].x += tmpsize_;
				tmppos[1].x += tmpsize_;
			}
		}
		else if (!Right)
		{
			if (data[cpos[1].y][cpos[1].x] && data[cpos[1].y - 1][cpos[1].x] && !data[cpos[1].y - 1][cpos[1].x - 1] && !data[cpos[1].y - 1][cpos[1].x - 2])
			{
				tmppos[0].x += tmpsize_;
				tmppos[1].x += tmpsize_;
			}
		}
		cInit();

		// 左にずれる
		if (data[cpos[1].y][cpos[1].x] || data[cpos[0].y][cpos[0].x])
		{
			tmppos[0].x += tmpsize_;
			tmppos[1].x += tmpsize_;
		}

		cInit();
		// それでも埋まっていたら上と下の位置を戻した後上下を入れ替える
		if (data[cpos[1].y][cpos[1].x] || data[cpos[0].y][cpos[0].x])
		{
			tmppos[1] = p1;
			tmppos[0] = p0;
			PUYO_TYPE type = (*stage_).player_[0]->GetType();
			(*stage_).player_[0]->SetType((*stage_).player_[1]->GetType());
			(*stage_).player_[1]->SetType(type);
		}
	}
	// 0が左の場合
	else if (p0.x < p1.x)
	{
		tmppos[0] = p0;
		tmppos[1] = Vector2(p0.x, p1.y + tmpsize_);
	
		cInit();

		if (data[cpos[1].y][cpos[1].x] || data[cpos[0].y][cpos[0].x])
		{
			tmpsize_ = -(*stage_).size_;
			tmppos[0].y += tmpsize_;
			tmppos[1].y += tmpsize_;

			cInit();
		}

		if (data[cpos[1].y][cpos[1].x] || data[cpos[0].y][cpos[0].x])
		{
			tmppos[1] = p1;
			tmppos[0] = p0;
		}
	}

	if (tmppos[0] != Vector2(-1, -1))
	{
		(*stage_).player_[0]->SetPos(tmppos[0]);
		(*stage_).player_[1]->SetPos(tmppos[1]);
		return true;
	}

	//else if (!data[y][x+1] || !data[(p0.y + (*stage_).size_ - 1) / (*stage_).size_ + 1][p0.x / (-tmpsize_) + 2])
	//{
	//	tmppos.y += tmpsize_;
	//	tmppos.x += (-tmpsize_);
	//	(*stage_).player_[1]->SetPos(tmppos);
	//	(*stage_).player_[0]->SetPos({ p0.x,p0.y + tmpsize_ });
	//	return true;
	//}
	// それすらできないなら上と下を入れ替える
	//else if(p0.y != p1.y)
	//{
	//	PUYO_TYPE type = (*stage_).player_[0]->GetType();
	//	(*stage_).player_[0]->SetType((*stage_).player_[1]->GetType());
	//	(*stage_).player_[1]->SetType(type);
	//	return true;
	//}

	return false;
}
