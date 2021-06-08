#pragma once
#include <map>
#include <list>
#include <string>
#include "common/Vector2.h"

#define lpEffectMng EffectMng::GetInstance()

class EffectMng
{
public:
	static EffectMng& GetInstance()
	{
		Create();
		return *sInstance;
	}
	static void Create(void)
	{
		if (sInstance == nullptr)
		{
			sInstance = new EffectMng();
		}
	}
	bool Init(Vector2&& size);
	void Update(void);
	void Draw(void);
	bool StopEffectAll(void);

	bool PlayEffect(std::string key, const Vector2& pos);
private:
	int GetImage(std::string key);
	std::map<std::string, int> images_;
	std::list<int> playlist_;

	EffectMng();
	~EffectMng();
	static EffectMng *sInstance;
};

