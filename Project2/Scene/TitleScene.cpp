#include "TitleScene.h"
#include <DxLib.h>
#include "SceneMng.h"
#include "GameScene.h"

TitleScene::TitleScene()
{
	Init();
}

TitleScene::~TitleScene()
{
}

std::unique_ptr<BaseScene> TitleScene::Update(std::unique_ptr<BaseScene> own)
{
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		return std::make_unique<GameScene>();
	}

	Draw();

	return std::move(own);
}

void TitleScene::Draw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	ChangeColor();
	auto color = [&](RGB_TYPE type) {
		return abs((color_[type]) % (255) - 125);
	};
	DrawBox(0, 0, lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y,GetColor(color(RGB_TYPE::RED), color(RGB_TYPE::GREEN), color(RGB_TYPE::BLUE)), true);

	DrawRotaGraph(lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 3 + abs((frame_) % 150 - 75) / 2, 1.0f, 0.0f, titleimage_, true);

	if (!(frame_ / 30 % 2)) {
		DrawString(lpSceneMng.GetScreenSize().x / 2 - (GetFontSize() * 7), lpSceneMng.GetScreenSize().y / 2 + 100, "press the space key", 0xffffff, true);
	}
	frame_++;
	ScreenFlip();

}

void TitleScene::ChangeColor(void)
{
	if (frame_ % (255 / 2) == 0) {
		now_ = static_cast<RGB_TYPE>(rand() % 3);
	}
	color_[now_]++;
}

bool TitleScene::Init(void)
{
	titleimage_ = LoadGraph("image/title.png");
	frame_ = 75;
	color_[RGB_TYPE::RED] = 0xff;
	color_[RGB_TYPE::BLUE] = 0xff;
	color_[RGB_TYPE::GREEN] = 0xff; 
	
	now_ = static_cast<RGB_TYPE>(rand() % 3);
	return true;
}
