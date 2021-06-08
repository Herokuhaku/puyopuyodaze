#include "Frame.h"
#include <DxLib.h>
Frame::Frame(int frameNo)
{
	frameNo_ = frameNo;
	Init();
}

Frame::~Frame()
{
}

void Frame::Draw(Vector2 offset, double rad)
{
	DrawRotaGraph(offset.x + pos_.x-45, offset.y + pos_.y-120,1.0f,rad, hundle,true);
}

void Frame::Init(void)
{
	switch (frameNo_)
	{
	case 0:
		hundle = LoadGraph("image/Frame.png");
		break;
	default:
		hundle = LoadGraph("image/Frame.png");
		break;
	}
	size_ = {388,824};
	pos_ = {size_.x/2,size_.y/2};
	screenID_ = MakeScreen(size_.x,size_.y);
}