#include "OjamaPuyo.h"
#include <DxLib.h>
OjamaPuyo::OjamaPuyo(Vector2&& pos, int size, puyoMap&& puyo, PUYO_TYPE type)
{
	pos_ = pos;
	size_ = size;
	_puyoimage = puyo;
	puyotype_ = type;
	munyon_.flag = 0;
	stayflag_ = true;
	Init();
}

OjamaPuyo::~OjamaPuyo()
{
}

bool OjamaPuyo::Update(BitData flag)
{
	if (!stayflag_) 
	{
		if (flag._bits.down)
		{
			pos_.y += speed_;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}

void OjamaPuyo::Draw(void)
{
	DrawRotaGraph(pos_.x + size_ / 2, pos_.y + size_ / 2, 1.0f, 0.0, _puyoimage[puyotype_][munyon_.flag], true);
}

void OjamaPuyo::Draw(Vector2 offset)
{
	DrawRotaGraph(pos_.x + size_ / 2 + offset.x, pos_.y - size_ / 2+offset.y, 1.0f, 0.0, _puyoimage[puyotype_][munyon_.flag], true);
}

bool OjamaPuyo::GetStayFlag(void)
{
	return stayflag_;
}

void OjamaPuyo::SetStayFlag(bool flag)
{
	stayflag_ = flag;
}

void OjamaPuyo::SetMunyon(BitData munyon)
{
	munyon_.flag = 0;
}
