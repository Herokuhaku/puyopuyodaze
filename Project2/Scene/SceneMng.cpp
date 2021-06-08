#include <DxLib.h>
#include "SceneMng.h"
#include "GameScene.h"
#include "../_debug/_DebugDispOut.h"
#include "TitleScene.h"
#include <time.h>
 SceneMng* SceneMng::sInstance = nullptr;

void SceneMng::Run(void)
{
	SysInit();
	_sceneNow = std::make_unique<TitleScene>();
	while (ProcessMessage() == 0)
	{
		SetDrawScreen(DX_SCREEN_BACK);
		ClsDrawScreen();

		_sceneNow = _sceneNow->Update(std::move(_sceneNow));

		ScreenFlip();
	}
}

Vector2 SceneMng::GetScreenSize(void)
{
	return _screenSize;
}

bool SceneMng::SysInit(void)
{
	bool rtnFlag = true;

	SetWindowText("puyopuyo");
	SetGraphMode(_screenSize.x, _screenSize.y, 16);
	ChangeWindowMode(true);
	if (DxLib::DxLib_Init() == -1)
	{
		return false;
	}
	srand((unsigned int)time(NULL));

	SetDrawScreen(DX_SCREEN_BACK);
	lpEffectMng.Init(Vector2(_screenSize));
	return true;
}

SceneMng::SceneMng() :_screenSize(1280, 720)
{
}

SceneMng::~SceneMng()
{
}
