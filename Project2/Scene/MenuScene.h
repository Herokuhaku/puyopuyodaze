#pragma once
#include "BaseScene.h"
class MenuScene :
	public BaseScene
{
public:
	MenuScene(std::unique_ptr<BaseScene> child);
	~MenuScene();
	std::unique_ptr<BaseScene> Update(std::unique_ptr<BaseScene> own) override;
	void Draw(void);
private:
	std::unique_ptr<BaseScene> childScene_;
};

