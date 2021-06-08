#pragma once
#include "puyo/Player.h"

using nextPair = std::pair<std::shared_ptr<Player>, std::shared_ptr<Player>>;

class Holdpuyo
{
public:
	Holdpuyo();
	~Holdpuyo();
	void Draw(Vector2 offset);
	nextPair& GetHold(void);
	void SetHold(nextPair hold);
private:
	void Init(void);
	
	nextPair hold_;
	Vector2 pos_;
	int screen_;

	int id_;
	static int count_;
};

