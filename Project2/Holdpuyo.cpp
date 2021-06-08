#include "Holdpuyo.h"
#include <DxLib.h>

int Holdpuyo::count_ = 0;

Holdpuyo::Holdpuyo()
{
	pos_ = { -48,0 };
	Init();
}

Holdpuyo::~Holdpuyo()
{
}

void Holdpuyo::Draw(Vector2 offset)
{
	auto Bscreen = GetDrawScreen();
	SetDrawScreen(screen_);
	ClsDrawScreen();

	DrawBox(0,0,52,96,0xffaaff,true);

	hold_.first->Draw();
	hold_.second->Draw();

	SetDrawScreen(Bscreen);
	int x = 288 + 60;
	if (id_ == 0) {
		x = -102;
	}
	//DrawGraph(offset.x + pos_.x, offset.y + pos_.y, screen_, true);
	DrawGraph(offset.x + x, offset.y + pos_.y, screen_, true);
}

nextPair& Holdpuyo::GetHold(void)
{
	return hold_;
}

void Holdpuyo::SetHold(nextPair hold)
{
	hold.first->SetPos({ 0,0 });
	hold.second->SetPos({ 0,48 });
	hold_ = hold;
}

void Holdpuyo::Init(void)
{
	screen_ = MakeScreen(72, 96, true);
	hold_.first = std::make_shared<Player>();
	hold_.second = std::make_shared<Player>();

	id_ = count_++;
}
