#pragma once
#include <vector>
#include <array>
#include "BaseScene.h"
#include "../Stage.h"
#include "../puyo/Player.h"

class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();
	std::unique_ptr<BaseScene> Update(std::unique_ptr<BaseScene> own) override;
	void Draw(void) override;
private:

	bool Init(void);
	void ChangeResult(std::shared_ptr<Stage>& stage);
	void Send(std::shared_ptr<Stage>& stage);
	std::unique_ptr<BaseScene> Next(std::unique_ptr<BaseScene> own);

	std::vector<std::shared_ptr<Stage>> _stageList;
	Vector2 offset_;

	void Change(void);
	std::pair<bool, bool> Pud[2];		// up down

	int countimage_[4];
	int frame_;

	bool start_;
};

