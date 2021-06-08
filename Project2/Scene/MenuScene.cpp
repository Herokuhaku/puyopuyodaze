#include "MenuScene.h"
#include <DxLib.h>
#include "SceneMng.h"
#include "TitleScene.h"
MenuScene::MenuScene(std::unique_ptr<BaseScene> child)
{
	childScene_ = std::move(child);
}

MenuScene::~MenuScene()
{
}

std::unique_ptr<BaseScene> MenuScene::Update(std::unique_ptr<BaseScene> own)
{
	childScene_->Draw();
	Draw();
	if (CheckHitKey(KEY_INPUT_R))
	{
		return std::move(childScene_);
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		return std::make_unique<TitleScene>();
	}
	return own;
}

void MenuScene::Draw(void)
{
	DrawBox(lpSceneMng.GetScreenSize().x / 2 -200,lpSceneMng.GetScreenSize().y / 2 -100,
		lpSceneMng.GetScreenSize().x / 2 + 200 , lpSceneMng.GetScreenSize().y / 2 + 100, 0xaaaaaa,true);

	DrawLine(lpSceneMng.GetScreenSize().x / 2 - 200, lpSceneMng.GetScreenSize().y / 2 - 100, lpSceneMng.GetScreenSize().x / 2 + 200, lpSceneMng.GetScreenSize().y / 2 - 100, 0x000000, 5);
	DrawLine(lpSceneMng.GetScreenSize().x / 2 - 200, lpSceneMng.GetScreenSize().y / 2 - 100, lpSceneMng.GetScreenSize().x / 2 - 200, lpSceneMng.GetScreenSize().y / 2 + 100, 0x000000, 5);
	
	DrawLine(lpSceneMng.GetScreenSize().x / 2 + 200, lpSceneMng.GetScreenSize().y / 2 + 100, lpSceneMng.GetScreenSize().x / 2 - 200, lpSceneMng.GetScreenSize().y / 2 + 100, 0x000000, 5);
	DrawLine(lpSceneMng.GetScreenSize().x / 2 + 200, lpSceneMng.GetScreenSize().y / 2 - 100, lpSceneMng.GetScreenSize().x / 2 + 200, lpSceneMng.GetScreenSize().y / 2 + 100, 0x000000, 5);
	SetFontSize(25);
	DrawString(lpSceneMng.GetScreenSize().x / 2 - GetFontSize() * 4, lpSceneMng.GetScreenSize().y / 2, "タイトル　に戻る", 0x000000, true);
	SetFontSize(14);
}
