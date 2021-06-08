#pragma once
#include <memory>
#include "../EffectMng.h"

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual std::unique_ptr<BaseScene> Update(std::unique_ptr<BaseScene> own) = 0;
	virtual void Draw(void);
};

