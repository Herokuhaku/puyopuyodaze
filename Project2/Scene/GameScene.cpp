#include <DxLib.h>
#include "../_debug/_DebugConOut.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "SceneMng.h"
#include "MenuScene.h"
#include "../puyo/Player.h"

GameScene::GameScene()
{
	Init();
}

GameScene::~GameScene()
{
	OutputDebugString("GameSceneが消えた");
}

std::unique_ptr<BaseScene> GameScene::Update(std::unique_ptr<BaseScene> own)
{
	Draw();
	if (CheckHitKey(KEY_INPUT_ESCAPE))
	{
		own = std::make_unique<MenuScene>(std::move(own));
	}
	if (start_)
	{
		for (auto& stage : _stageList)
		{
			// 勝敗を決める
			ChangeResult(stage);
			// おじゃまぷよを送る
			Send(stage);	
			stage->Update();
		}
	}	
	// 操作デバイス変更
	Change();
	// 勝負がついている状態でスペースを押すとタイトルに戻る
	own = Next(std::move(own));

	// エフェクシアを動かす
	lpEffectMng.Update();
	frame_++;
	return std::move(own);
}

void GameScene::Draw(void)
{
	DrawBox(0, 0, lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y, 0xafeeee, true);
	for (auto&& stage : _stageList)
	{
		stage->Draw();
		SetDrawScreen(DX_SCREEN_BACK);
		stage->DrawMix();
	};
	lpEffectMng.Draw();

	if (!start_)
	{
		DrawRotaGraph(lpSceneMng.GetScreenSize().x/2, lpSceneMng.GetScreenSize().y/2,1.0f,0.0f,countimage_[frame_/60],true);
	}
}

bool GameScene::Init(void)
{
	// Stage
	 offset_ = { 145, 48 };
	_stageList.emplace_back(std::make_shared<Stage>(std::move(offset_)));

	offset_ = { 865, 48 };
	_stageList.emplace_back(std::make_shared<Stage>(std::move(offset_)));

	Pud[0] = {0,0 };
	Pud[1] = { 0,0 };

	frame_ = 0;
	
	LoadDivGraph("image/count.png",4,4,1,150,150,&countimage_[0],true);

	return true;
}

void GameScene::ChangeResult(std::shared_ptr<Stage>& stage)
{
	if (stage->GetResult().second == ResultMode::LOSE)
	{
		std::vector<int> id;
		for (auto& stg : _stageList)
		{
			if (stg->GetResult().first == ResultMode::MAX)
			{
				id.emplace_back(stg->GetStgId());
			}
		}
		if (id.size() == 1)
		{
			_stageList[id[0]]->SetResult(ResultMode::WIN);
		}
	}
}

void GameScene::Send(std::shared_ptr<Stage>& stage)
{
	// おじゃま送る
	if (stage->GetOjamaFlag())
	{
		if (_stageList.size() - 1 >= static_cast<size_t>(stage->GetStgId()) + 1)
		{
			_stageList[static_cast<size_t>(stage->GetStgId()) + 1]->SetOjamaList(stage->GetSendList());
		}
		else
		{
			_stageList[0]->SetOjamaList(stage->GetSendList());
		}
		stage->SetOjamaFlag(false);
	}
}

std::unique_ptr<BaseScene> GameScene::Next(std::unique_ptr<BaseScene> own)
{
	bool fin = true;
	for (auto& stg : _stageList)
	{
		fin &= stg->GetResult().second != ResultMode::MAX;
	}
	if (fin)
	{
		if (frame_ / 30 % 2 == 0) {
			DrawString(lpSceneMng.GetScreenSize().x / 2 - GetFontSize() * 6, lpSceneMng.GetScreenSize().y / 3 * 2, "Ａキー　でタイトルに戻る", 0x000000, true);
		}
		if (CheckHitKey(KEY_INPUT_A))
		{
			return std::make_unique<TitleScene>();
		}
	}
	return own;
}

void GameScene::Change(void)
{
	Pud[0].second = Pud[0].first;
	Pud[0].first = CheckHitKey(KEY_INPUT_PGUP);

	Pud[1].second = Pud[1].first;
	Pud[1].first = CheckHitKey(KEY_INPUT_PGDN);
	auto change = [&](int no) {
		ControlType tmp = _stageList[no]->GetCntType();
		int tmpid = static_cast<int>(tmp) + 1;
		if (static_cast<ControlType>(tmpid) == ControlType::MAX)
		{
			tmpid = 0;
		}
		return static_cast<ControlType>(tmpid);
	};
	if (Pud[0].first && !Pud[0].second) {
		ControlType type = change(0);
		_stageList[0]->SetCntrol(type);
	}
	if (Pud[1].first && !Pud[1].second) {
		ControlType type = change(1);
		_stageList[1]->SetCntrol(type);
	}

	if ((frame_+45) / 60 == 4)
	{
		start_ = true;
	}
}
