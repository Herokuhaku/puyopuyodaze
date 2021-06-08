#include <DxLib.h>
#include "../_debug/_DebugConOut.h"
#include "Player.h"
#include "../Stage.h"
#include <string>

int Player::count_ = 0;

Player::Player()
{
	pos_ = { 0,0 };
	puyotype_ = PUYO_TYPE::NON;
	size_ = 0;
	Init();
}

Player::Player(Vector2 && pos,int size,puyoMap puyo, PUYO_TYPE type)
{
	pos_ = pos;
	size_ = size;
	_puyoimage = puyo;
	puyotype_ = type;
	munyon_.flag = 0;
	Init();
}

Player::~Player()
{
	count_--;
	OutputDebugString(std::to_string(count_).c_str());
	OutputDebugString("\n");
}

bool Player::Update(BitData flag)
{
	if (flag._bits.down)
	{
		pos_.y += speed_;
	}
	else
	{
		return false;
	}
	puyonflag_ = true;
	return true;
}

void Player::Move(INPUT_ID id, BitData flag)
{
	switch (id)
	{
	case INPUT_ID::UP:
		if (flag._bits.up) {
			pos_.y -= size_;
		}
		break;
	case INPUT_ID::DOWN:
		if (flag._bits.down) {
			pos_.y += speed_*2;
		}
		break;
	case INPUT_ID::LEFT:
		if (flag._bits.left) {
			pos_.x -= size_;
		}
		break;
	case INPUT_ID::RIGHT:
		if (flag._bits.right) {
			pos_.x += size_;
		}
		break;
	case INPUT_ID::L_ROTA:
		break;
	case INPUT_ID::R_ROTA:
		break;
	default:
		break;
	}
}

void Player::Draw(void)
{
	if (puyonflag_ && puyotype_ > PUYO_TYPE::NON && puyotype_ < PUYO_TYPE::WALL) {
		int abs_ = abs((puyon_) % 30 - 15) / 2;
		DrawModiGraph(pos_.x - abs_, pos_.y + abs_ + (countflag * (abs_ * 2)),	// 左上
			pos_.x + size_ + abs_, pos_.y + abs_ + (countflag * (abs_ * 2)),				// 右上
			pos_.x + size_ + abs_, pos_.y + size_ + (countflag * (abs_ * 2)),				// 右下
			pos_.x - abs_, pos_.y + size_ + (countflag * (abs_ * 2)),						// 左下
			_puyoimage[puyotype_][0], true);
	}
	else if (puyotype_ > PUYO_TYPE::NON && puyotype_ < PUYO_TYPE::WALL)
	{
		DrawRotaGraph(pos_.x + size_ / 2, pos_.y + size_ / 2, 1.0f, 0.0, _puyoimage[puyotype_][munyon_.flag], true);
	}
}

void Player::TopDraw(void)
{
	int befor = GetDrawScreen();

	SetDrawScreen(buf_);
	ClsDrawScreen();
	if (puyonflag_ && puyotype_ > PUYO_TYPE::NON && puyotype_ < PUYO_TYPE::WALL) {
		int abs_ = abs((puyon_) % 30 - 15) / 2;
		DrawModiGraph(- abs_,abs_ + (countflag * (abs_ * 2)),	// 左上
			size_ + abs_, abs_ + (countflag * (abs_ * 2)),				// 右上
			size_ + abs_, size_ + (countflag * (abs_ * 2)),				// 右下
			- abs_,size_ + (countflag * (abs_ * 2)),						// 左下
			_puyoimage[PUYO_TYPE::OJAMA][0], true);
	}
	else if (puyotype_ > PUYO_TYPE::NON && puyotype_ < PUYO_TYPE::WALL)
	{
		DrawRotaGraph( size_ / 2,size_ / 2, 1.0f, 0.0, _puyoimage[PUYO_TYPE::OJAMA][munyon_.flag], true);
	}
	//GraphFilter(buf_, DX_GRAPH_FILTER_GAUSS, 5, 500);
	GraphFilter(buf_, DX_GRAPH_FILTER_HSB, 0, 0, 0, -255);

	SetDrawScreen(befor);
	DrawRotaGraph(pos_.x + size_ / 2, pos_.y + size_ / 2, 1.1f, 0.0,buf_, true);
}

void Player::GhostDraw(Vector2 pos)
{
	int befor = GetDrawScreen();

	SetDrawScreen(ghost_);
	ClsDrawScreen();	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 122);

	if (puyonflag_ && puyotype_ > PUYO_TYPE::NON && puyotype_ < PUYO_TYPE::WALL) {
		int abs_ = abs((puyon_) % 30 - 15) / 2;
		DrawModiGraph(-abs_, abs_ + (countflag * (abs_ * 2)),	// 左上
			size_ + abs_, abs_ + (countflag * (abs_ * 2)),				// 右上
			size_ + abs_, size_ + (countflag * (abs_ * 2)),				// 右下
			-abs_, size_ + (countflag * (abs_ * 2)),						// 左下
			_puyoimage[puyotype_][0], true);
	}
	else if (puyotype_ > PUYO_TYPE::NON && puyotype_ < PUYO_TYPE::WALL)
	{
		DrawRotaGraph(size_ / 2, size_ / 2, 1.0f, 0.0, _puyoimage[puyotype_][munyon_.flag], true);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetDrawScreen(befor);

	DrawRotaGraph(pos.x + size_ / 2, pos.y + size_ / 2, 1.0f, 0.0, ghost_, true);


}

const Vector2& Player::GetPos(void)
{
	return pos_;
}

void Player::SetPos(Vector2 pos)
{
	pos_ = pos;
}

const Vector2 Player::GetGrid(void)
{
	return pos_ / size_;
}

PUYO_TYPE Player::GetType(void)
{
	return puyotype_;
}

void Player::SetType(PUYO_TYPE type)
{
	puyotype_ = type;
}

bool Player::GetAlive(void)
{
	return alive_;
}

bool Player::GetPuyonFlag(void)
{
	return puyonflag_;
}

int Player::GetMunyon(void)
{
	return munyon_.flag;
}

void Player::SetAlive(bool alive)
{
	alive_ = alive;
}

void Player::SetPuyon(int puyon)
{
	if (puyonflag_) {
		puyon_ = puyon;
	}
}

void Player::SetPuyonFlag(bool puyon)
{
	puyonflag_ = puyon;
}

void Player::SetMunyon(BitData munyon)
{
	if (munyon_.flag >= 0 && munyon_.flag <= 16) {
		munyon_ = munyon;
	}
}

void Player::SetDropSpeed(int speed)
{
	speed_ = speed;
}

void Player::SetCountflag(int flag)
{
	countflag = flag;
}

void Player::Init(void)
{
	buf_ = MakeScreen(48, 48, true);
	ghost_ = MakeScreen(48,48,true);
	id_ = count_;
	alive_ = true;
	// 赤ぷよ
	//_puyoimage[PUYO_TYPE::RED].resize(16);
	//LoadDivGraph("image/redpuyo.png", 16, 16, 1,size_,size_, &_puyoimage[PUYO_TYPE::RED][0]);
	frame_ = 1;
	count_++;
	speed_ = 3;
	puyon_ = 15;
	puyonflag_ = false;
	countflag = 0;
}

