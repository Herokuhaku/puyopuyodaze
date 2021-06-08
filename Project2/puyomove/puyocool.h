#pragma once
#include "Stage.h"
#include "playUnit.h"
#include "puyofall.h"
struct puyocool
{
	bool operator()(Stage& stg) {
		change_ |= stg.playunit_->Update();   // ����
		
		if (cooltime_ > time_) {
			stg.stagemode_ = stg.nextmode_;
			if (stg.nextmode_ == StageMode::FALL) {
				stg.stagemode_ = StageMode::DROP;
			}
			cooltime_ = 0;
			time_ = 60;
			change_ = false;
		}
		cooltime_++;
		if (!change_ && cooltime_ > nontime_ || stg.cooldownflag_)
		{
			time_ = 0;
		}
		return true;
	}
private:
	int cooltime_ = 0;
	bool change_ = false;
	int time_ = 60;							// ��������̓������������ꍇ���ԕς��
	static int constexpr nontime_ =  10;		// �G���Ă��Ȃ����� x �t���[���܂ŋ��e
};