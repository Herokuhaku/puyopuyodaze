#pragma once
#include "BaseScene.h"
#include <map>

enum class RGB_TYPE
{
	RED,
	GREEN,
	BLUE
};
class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	std::unique_ptr<BaseScene> Update(std::unique_ptr<BaseScene> own) override;
private:
	void Draw(void);
	void ChangeColor(void);
	bool Init(void);

	int titleimage_;
	int frame_;
	char buf_[256];
	std::map<RGB_TYPE,int> color_;
	RGB_TYPE now_;
};

