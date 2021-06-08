#pragma once
#include <memory>
#include <vector>
#include "BaseScene.h"
#include "../common/Vector2.h"

#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
public:
	void Run(void);

	static SceneMng& GetInstance(void)
	{
		Create();
		return *sInstance;
	}
	static void Create(void)
	{
		if (sInstance == nullptr)
		{
			sInstance = new SceneMng();
		}
	}
	static void Destroy(void)
	{
		if (sInstance != nullptr)
		{
			delete sInstance;
		}
		sInstance = nullptr;
	}

	Vector2 GetScreenSize(void);
private:
	bool SysInit(void);

	const Vector2 _screenSize;
	std::unique_ptr<BaseScene> _sceneNow;

	SceneMng();
	~SceneMng();
	
	static SceneMng* sInstance;
};