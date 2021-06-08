#include <DxLib.h>
#include "Nextpuyo.h"

int Nextpuyo::count_ = 0;

Nextpuyo::Nextpuyo(Vector2&& pos, int Generateno, int Drawno, std::map<PUYO_TYPE, std::vector<int>> image)
{
	pos_ = pos;
	GenerateNo_ = Generateno;
	DrawNo_ = Drawno;
	puyoimage_ = image;
	Init();
}

Nextpuyo::~Nextpuyo()
{

}

void Nextpuyo::Draw(Vector2 offset)
{
	int no = DrawNo_;
	auto Bscreen = GetDrawScreen();
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	DrawBox(0,0, 48 * (DrawNo_)-24, 96 * (DrawNo_)-48,0xaaddaa,true);
	for (auto& next : nextlist_)
	{
		next.first->Draw();
		next.second->Draw();
		if (--no <= 0)
		{
			break;
		}
	}
	SetDrawScreen(Bscreen);
	int x = 288+96;
	if (id_ == 1) {
		x = -96-48;
	}
	DrawGraph(offset.x + x ,offset.y+pos_.y,screenID_,true);
}

nextPair Nextpuyo::PickUp(void)
{
	pick = nextlist_.front();

	pick.first->SetPos(Vector2(48*3,0));
	pick.second->SetPos(Vector2(48*3, 48));
	nextlist_.erase(nextlist_.begin());

	AddNext(1);

	return pick;
}

void Nextpuyo::Init(void)
{
	screenID_ = MakeScreen(48*(DrawNo_)-24,96*(DrawNo_), true);
	next[0].reset();	
	next[1].reset();
	id_ = count_++;
	AddNext(GenerateNo_);
}

void Nextpuyo::AddNext(int Generateno)
{
	for (int i = 0; i < Generateno; i++)
	{
		next[0] = std::make_shared<Player>(Vector2(pos_.x, 48), 48, puyoimage_, static_cast<PUYO_TYPE>(rand() % 5 + 1));
		next[1] = std::make_shared<Player>(Vector2(pos_.x, 0), 48, puyoimage_, static_cast<PUYO_TYPE>(rand() % 5 + 1));
		if (next[0] && next[1]) {
			nextlist_.emplace_back(nextPair(next[0], next[1]));
		}
	}
	Vector2 pos[2] = { {0,0},{0,48} };
	Vector2 offset(48, 24);

	if (id_ == 1) {
		pos[0] = {48 * (DrawNo_)-72,0};
		pos[1] ={ 48 * (DrawNo_)-72,48 };
		offset = { -48, 24 };
	}

	for (auto& next : nextlist_)
	{
		next.first->SetPos(pos[0]);
		next.second->SetPos(pos[1]);
		pos[0] += offset;
		pos[1] += offset;
	}
}
