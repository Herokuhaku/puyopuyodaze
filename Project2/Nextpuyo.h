#pragma once
#include <memory>
#include <list>
#include "puyo/Player.h"
//#include "Vector2.h"

using nextPair = std::pair<std::shared_ptr<Player>, std::shared_ptr<Player>>;
using nextList = std::list<nextPair>;

class Nextpuyo
{
public:
	Nextpuyo(Vector2 && pos,int Generateno, int Drawno, std::map<PUYO_TYPE, std::vector<int>> image);
	~Nextpuyo();
	void Draw(Vector2 offset);
	nextPair PickUp(void);
private:
	void Init(void);
	void AddNext(int Generateno);

	Vector2 pos_;
	Vector2 offeset_;
	int GenerateNo_;
	int DrawNo_;
	int screenID_;

	nextList nextlist_;
	std::map<PUYO_TYPE, std::vector<int>> puyoimage_;	// ‰æ‘œ•Û‘¶
	int id_;
	static int count_;

	std::shared_ptr<Player> next[2];
	nextPair pick;
};

