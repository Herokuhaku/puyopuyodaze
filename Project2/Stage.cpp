#include <DxLib.h>
#include "Stage.h"
#include <functional>
#include "_debug/_DebugConOut.h"
#include "AllControl/XboxController.h"
#include "AllControl/KeyBoardCtl.h"
#include "AllControl/Mouse.h"
#include "playUnit.h"
#include "puyomove/puyodrop.h"
#include "puyomove/puyofall.h"
#include "puyomove/puyoerase.h"
#include "puyomove/puyopuyon.h"
#include "puyomove/puyomunyon.h"
#include "puyomove/puyowin.h"
#include "puyomove/puyolose.h"
#include "puyomove/puyocool.h"
#include "puyomove/puyojama.h"
#include "EffectMng.h"

int Stage::count_ = 0;

Stage::Stage():stageSize(288,624)
{
	Init();
}

Stage::Stage(const Vector2&& offset) : stageSize(288, 624)
{
	Init();
	offset_ = offset;

	player_.emplace_back(std::make_shared<Player>(Vector2(stageSize.x / 2, size_), size_,_puyoimage,static_cast<PUYO_TYPE>(rand() % 5+1)));
	player_.emplace_back(std::make_shared<Player>(Vector2(stageSize.x / 2,0), size_, _puyoimage, static_cast<PUYO_TYPE>(rand() % 5 + 1)));
}

Stage::~Stage()
{
	count_--;
}

void Stage::Update(void)
{
	(*controller_)();

	// リザルトをnowとoldにわけてDraw判定する
	// Drawの場合は両方LOSE
	result_.second = result_.first;
	// GameOverになったらステージをクリアしとく

	if (!stgupdate_[stagemode_](*this))
	{
#ifdef _DEBUG
		StageReset();
		return;
#else
		result_.first = ResultMode::LOSE;
#endif
	}
	SwitchStgMode();

	// ぷよをぷよんぷよんさせる
	if (CheckHitKey(KEY_INPUT_P))
	{
		for (auto& pl : player_) {
			pl->SetPuyonFlag(true);
		}
		puyonCnt_++;
	}
}

int Stage::GetScreen(void)
{
	return _stageScreen;
}

Vector2 Stage::GetOffset(void)
{
	return offset_;
}

void Stage::Draw(void)
{
	SetDrawScreen(_stageScreen);
	ClsDrawScreen();

	//DrawGraph(0, 0, _drawStage, true);
	DrawBox(0, 0, stageSize.x+1, stageSize.y+1, 0xaaaaaa,true);

	//for (int i = 0; i < 14; i++)
	//{
	//	DrawLine(0, i * size_, stageSize.x, i * size_, 0x000000, true);
	//}

	//for (int i = 0; i < 7; i++)
	//{
	//	DrawLine(i * size_, 0, i * size_, stageSize.y, 0x000000, true);
	//}

	// 操作中のぷよの描画
	// 予測位置のぷよの描画
	GhostDraw();
	for (auto&& pl : player_)
	{
		pl->Draw();
		pl->SetPuyon(puyonCnt_);
	}
}

void Stage::DrawMix(void)
{
	Vector2 off = offset_ + pos_;
	DrawRotaGraph(off.x+150, off.y+300,1.0f,rad_, _stageScreen, true);
	frame_->Draw(off,rad_);

	off = offset_;
	next_->Draw(off);
	hold_->Draw({ offset_.x, offset_.y });
	OjamaDraw();
}

bool Stage::GetOjamaFlag(void)
{
	return nextojama_;
}

int Stage::GetStgId(void)
{
	return id_;
}

std::list<std::shared_ptr<OjamaPuyo>> Stage::GetSendList(void)
{
	std::list<std::shared_ptr<OjamaPuyo>> tmp = sendojama_;
	sendojama_.clear();
	return tmp;
}

ControlType Stage::GetCntType(void)
{
	return ctype_;
}

bool Stage::Init(void)
{
	pos_ = { 0,0 };
	rad_ = 0.0f;
	_puyoimage[PUYO_TYPE::RED].resize(16);
	LoadDivGraph("image/redpuyo.png", 16, 16, 1, size_, size_, &_puyoimage[PUYO_TYPE::RED][0]);
	_puyoimage[PUYO_TYPE::BLUE].resize(16);
	LoadDivGraph("image/bluepuyo.png", 16, 16, 1, size_, size_, &_puyoimage[PUYO_TYPE::BLUE][0]);
	_puyoimage[PUYO_TYPE::YELLOW].resize(16);
	LoadDivGraph("image/yellowpuyo.png", 16, 16, 1, size_, size_, &_puyoimage[PUYO_TYPE::YELLOW][0]);
	_puyoimage[PUYO_TYPE::GREEN].resize(16);
	LoadDivGraph("image/greenpuyo.png", 16, 16, 1, size_, size_, &_puyoimage[PUYO_TYPE::GREEN][0]);
	_puyoimage[PUYO_TYPE::PURPLE].resize(16);
	LoadDivGraph("image/purplepuyo.png", 16, 16, 1, size_, size_, &_puyoimage[PUYO_TYPE::PURPLE][0]);
	_puyoimage[PUYO_TYPE::OJAMA].resize(16);
	LoadDivGraph("image/ojamapuyo.png", 16, 16, 1, size_, size_, &_puyoimage[PUYO_TYPE::OJAMA][0]);

	image_[ResultMode::LOSE] = LoadGraph("image/lose.png", true);
	image_[ResultMode::WIN] = LoadGraph("image/win.png", true);

	_stageScreen = MakeScreen(stageSize.x+1,stageSize.y+1); // DrawLineが描画できるように1を追加
	//_drawStage = LoadGraph("image/playerStage.png",true);

	id_ = count_;
	count_++;

	ctype_ = ControlType::KEY;
	if (id_ == 1) {
		ctype_ = ControlType::KEY;
	}

	SetCntrol(ctype_);

	stagemode_ = StageMode::DROP;
	nextmode_ = StageMode::MAX;

	_dataBase.resize(STAGE_CHIP_X * STAGE_CHIP_Y);
	_erasedata.resize(STAGE_CHIP_X * STAGE_CHIP_Y);

	for (size_t no = 0; no < STAGE_CHIP_Y; no++)
	{
		_dataBase_ptr.emplace_back(&_dataBase[no * STAGE_CHIP_X]);
		_erasedata_ptr.emplace_back(&_erasedata[no * STAGE_CHIP_X]);
	}
	// ステージにブロックを埋め込む
	for (int y = 0; y < STAGE_CHIP_Y; y++)
	{
		for (int x = 0; x < STAGE_CHIP_X; x++)
		{
			if(y == 14)
			{
				_dataBase_ptr[y][x] = std::make_shared<Player>(Vector2(-size_ + x * size_, -size_ + size_ * y), size_, _puyoimage, PUYO_TYPE::WALL);
			}
			if ((x == 0 || x == 7) && y != 14)
			{
				_dataBase_ptr[y][x] = std::make_shared<Player>(Vector2(-size_ + x * size_, -size_ + size_ * y), size_, _puyoimage, PUYO_TYPE::WALL);
			}
		}
	}

	num.fill(-1);

	playunit_ = std::make_unique<playUnit>(*this);
	next_ = std::make_unique<Nextpuyo>(Vector2(stageSize.x,100),3,2,_puyoimage);
	hold_ = std::make_unique<Holdpuyo>();
	frame_ = std::make_unique<Frame>(0);

	stgupdate_.try_emplace(StageMode::DROP,puyodrop());
	stgupdate_.try_emplace(StageMode::FALL, puyofall());
	stgupdate_.try_emplace(StageMode::ERASE, puyoerase());
	stgupdate_.try_emplace(StageMode::PUYON, puyopuyon());
	stgupdate_.try_emplace(StageMode::MUNYON, puyomunyon());
	stgupdate_.try_emplace(StageMode::WIN, puyowin());
	stgupdate_.try_emplace(StageMode::LOSE, puyolose());
	stgupdate_.try_emplace(StageMode::COOL, puyocool());
	stgupdate_.try_emplace(StageMode::JAMA, puyojama());
	result_ = { ResultMode::MAX,ResultMode::MAX };

	rensa_ = 0;
	rensaMax_ = 2;
	eraseCount_ = 0;
	puyonCnt_ = 15;
	ojamaflag_ = false;
	nextojama_ = false;
	holdflag_ = false;
	coolflag_ = false;
	cooldownflag_ = false;
	return true;
}

void Stage::ChangeSignal(ShareP& puyo)
{
	//当たり判定　leftright 右(1)    // 左(-1) 　x,yともに1ブロックの判定用のブロックを置いているためy,xに1を足して0に合わせている

	auto lrhit = [&](int off) {

		int y = ((puyo->GetPos().y + size_-1) / size_) + 1;
		int x = (puyo->GetGrid().x) + off + 1;
		if (y < STAGE_CHIP_Y && 0 < y &&
			x < STAGE_CHIP_X)
		{
			if (!_dataBase_ptr[y][x])
			{
				return true;
			}
		}
	return false;
	};

	// 下チェック updown 上(-1) 下(1)
	auto udhit = [&](int off) {
		int y = (((puyo->GetPos().y) / size_) + 1) + off;
		int x = puyo->GetGrid().x + 1;
		if (y < STAGE_CHIP_Y &&y > 0 &&
			x < STAGE_CHIP_X)
		{
			if (!_dataBase_ptr[y][x])
			{
				return true;
			}
		}else
		{
			return true;
		}
	return false;
	};

	bitdata_._bits.right = lrhit(1);
	bitdata_._bits.left = lrhit(-1);
	bitdata_._bits.down = udhit(1);
	bitdata_._bits.up = udhit(-1);

}

void Stage::InsertPuyo(void)
{
	if (player_.empty()) 
	{
		data = next_->PickUp();
		while (_dataBase_ptr[data.second->GetGrid().y + 1][data.second->GetGrid().x + 1])
		{
			data.first->SetPos(Vector2(data.first->GetPos().x, data.first->GetPos().y - 48));
			data.second->SetPos(Vector2(data.second->GetPos().x, data.second->GetPos().y - 48));
		}

		player_.emplace(player_.begin(), data.first);
		player_.emplace(player_.begin(), data.second);
	}
	else
	{
		// 座標がずれてる分整えて収納しておく
		if (player_.size() >= 2)
		{
			player_[0]->SetPos(player_[0]->GetGrid() * size_);
			player_[1]->SetPos(player_[1]->GetGrid() * size_);
		}
		auto data = next_->PickUp();
		while (_dataBase_ptr[data.second->GetGrid().y + 1][data.second->GetGrid().x + 1])
		{
			data.first->SetPos(Vector2(data.first->GetPos().x, data.first->GetPos().y - 48));
			data.second->SetPos(Vector2(data.second->GetPos().x, data.second->GetPos().y - 48));
		}

		player_.emplace(player_.begin(),data.first);
		player_.emplace(player_.begin(), data.second);

	}
}

bool Stage::ErasePuyo(ShareP& puyo)
{
	if (puyo->GetGrid().y + 1 < STAGE_CHIP_Y && puyo->GetGrid().y + 1 >= 0 &&
		puyo->GetGrid().x + 1 < STAGE_CHIP_X && puyo->GetGrid().x + 1>= 0) {
		if (player_.size() >= 2) {
			if (player_[0]->GetPos().x == player_[1]->GetPos().x && player_[0]->GetPos().y > player_[1]->GetPos().y)
			{
				std::swap(player_[0], player_[1]);
			}
		}
	_dataBase_ptr[puyo->GetGrid().y + 1][puyo->GetGrid().x + 1] = puyo;
	}	
	//for (int i = 0; i < STAGE_CHIP_X * STAGE_CHIP_Y; i++)
	//{
	//	_erasedata[i].reset();
	//}

	int count = 0;

	std::vector<ShareP> tmperase;
	std::vector<ShareP*> tmperase_ptr;

	Vector2 tmppos = { puyo->GetGrid().x + 1,puyo->GetGrid().y + 1 };

	tmperase.resize(STAGE_CHIP_X * STAGE_CHIP_Y);

	for (size_t no = 0; no < STAGE_CHIP_Y; no++)
	{
		tmperase_ptr.emplace_back(&tmperase[no * STAGE_CHIP_X]);
	}
	std::function<void(Vector2, PUYO_TYPE)>checktmp = [&](Vector2 pos, PUYO_TYPE id) {
		if (pos.y < STAGE_CHIP_Y && pos.y >= 0 &&
			pos.x < STAGE_CHIP_X && pos.x >= 0) {
			if (!tmperase_ptr[pos.y][pos.x]) {
				if (_dataBase_ptr[pos.y][pos.x])
				{
					if (_dataBase_ptr[pos.y][pos.x]->GetType() == id && id != PUYO_TYPE::OJAMA)
					{
						count++;
						tmperase_ptr[pos.y][pos.x] = _dataBase_ptr[pos.y][pos.x];
						checktmp({ pos.x - 1,pos.y }, id);
						checktmp({ pos.x + 1,pos.y }, id);
						checktmp({ pos.x ,pos.y - 1 }, id);
						checktmp({ pos.x ,pos.y + 1 }, id);
					}
					else if (_dataBase_ptr[pos.y][pos.x]->GetType() == PUYO_TYPE::OJAMA)
					{
						tmperase_ptr[pos.y][pos.x] = _dataBase_ptr[pos.y][pos.x];
					}
				}
			}
		}
	};

	std::function<void(Vector2, PUYO_TYPE)>lamu = [&](Vector2 pos, PUYO_TYPE id) {
		if (pos.y < STAGE_CHIP_Y && pos.y >= 0 &&
			pos.x < STAGE_CHIP_X && pos.x >= 0) {
			if (!_erasedata_ptr[pos.y][pos.x]) {
				if (_dataBase_ptr[pos.y][pos.x])
				{
					if (_dataBase_ptr[pos.y][pos.x]->GetType() == id && id != PUYO_TYPE::OJAMA)
					{
						_erasedata_ptr[pos.y][pos.x] = _dataBase_ptr[pos.y][pos.x];
						lamu({ pos.x - 1,pos.y }, id);
						lamu({ pos.x + 1,pos.y }, id);
						lamu({ pos.x ,pos.y - 1 }, id);
						lamu({ pos.x ,pos.y + 1 }, id);
					}
					else if (_dataBase_ptr[pos.y][pos.x]->GetType() == PUYO_TYPE::OJAMA)
					{
						_erasedata_ptr[pos.y][pos.x] = _dataBase_ptr[pos.y][pos.x];
					}
				}
			}
		}
	};


	checktmp(tmppos,puyo->GetType());
	if (count >= 4)
	{
		lamu(tmppos, puyo->GetType());
	}

	if (count >= 4)
	{
		for (auto&& pl : player_)
		{
			if (pl->GetGrid().y + 1 < STAGE_CHIP_Y && pl->GetGrid().y + 1 >= 0 &&
				pl->GetGrid().x + 1 < STAGE_CHIP_X && pl->GetGrid().x + 1 >= 0) {
				if (_erasedata_ptr[pl->GetGrid().y + 1][pl->GetGrid().x + 1]) {
					if (_erasedata_ptr[pl->GetGrid().y + 1][pl->GetGrid().x + 1]->GetType() == puyo->GetType() ||
						_erasedata_ptr[pl->GetGrid().y + 1][pl->GetGrid().x + 1]->GetType() == PUYO_TYPE::OJAMA)
					{
						pl->SetAlive(false);
						Vector2 vec;
						vec.x = static_cast<double>(offset_.x + (pl->GetPos().x + size_ / 2));
						vec.y = static_cast<double>(offset_.y + (pl->GetPos().y + size_ / 2));
						lpEffectMng.PlayEffect("Sumple2", vec);
					}
				}
			}
		}
		return true;
	}
	return false;
}

void Stage::DeletePuyo(void)
{
	for (int i = 0; i < STAGE_CHIP_X * STAGE_CHIP_Y; i++)
	{
		if (_erasedata[i])
		{
			_dataBase[i].reset();
			_erasedata[i].reset();
		}
	}
}

void Stage::StageReset(void)
{	
	player_.clear();
	player_.shrink_to_fit();

	for (int i = 0; i < STAGE_CHIP_X * STAGE_CHIP_Y; i++)
	{
		if (_dataBase[i]) {
			if (_dataBase[i]->GetType() != PUYO_TYPE::WALL) 
			{
				_dataBase[i].reset();
			}
		}
		_erasedata[i].reset();
	}

	InsertPuyo();
	stagemode_ = StageMode::DROP;
}

void Stage::SetOjamaList(std::list<std::shared_ptr<OjamaPuyo>> ojama)
{
	for (auto list : ojama)
	{
		ojamalist_.emplace_back(list);
	}
	std::array<int, 6>n;
	n.fill(-1);
	auto data = [&](int x)
	{
		for (auto& no : n) {
			if (no == x)
			{
				return true;
			}
		}
		return false;

	};
	int num = 0;
	for (auto& i : ojamalist_)
	{
		if (num != 0 && num % 6 == 0)
		{
			n.fill(-1);
		}
		int tmp = rand() % 6;
		while (data(tmp))
		{
			tmp = rand() % 6;
		}
		n[num % 6] = tmp;
		i->SetPos(Vector2((tmp * size_) % (size_ * 6),
			(-size_ * (num / 6))
			));
			
		num++;
	}
}

void Stage::SetOjamaFlag(bool flag)
{
	nextojama_ = flag;
}

void Stage::SwitchStgMode(void)
{
	if (result_.second != ResultMode::MAX)
	{
		switch (result_.first)
		{
		case ResultMode::WIN:
			stagemode_ = StageMode::WIN;
			break;
		case ResultMode::LOSE:
			stagemode_ = StageMode::LOSE;
			break;
		default:
			break;
		}
	}
}

std::pair<ResultMode, ResultMode> Stage::GetResult(void)
{
	return result_;
}

void Stage::SetResult(ResultMode result)
{
	result_.first = result;
}

void Stage::GhostDraw(void)
{
	if (stagemode_ == StageMode::DROP && !coolflag_)
	{
		if (!player_.empty())
		{
			player_[0]->TopDraw();
		}
		if (player_.size() >= 2)
		{
			Vector2 pos[2];
			// 床までゴーストを落とす
			for (int i = 0;i < 2;i++) {
				pos[i].x = player_[i]->GetPos().x;
				for (int y = player_[i]->GetGrid().y + 1;y < STAGE_CHIP_Y;y++)
				{
					if (y >= 0) {
						if (_dataBase_ptr[y][player_[i]->GetGrid().x + 1])
						{
							pos[i].y = (y - 1) * size_ - size_;
							break;
						}
					}
				}
				pos[1].x = player_[1]->GetPos().x;
			}
		// 同じところの場合
			if (pos[0] == pos[1])
			{
				if (player_[0]->GetPos().y < player_[1]->GetPos().y)
				{
					pos[0].y -= size_;
				}
				else
				{
					pos[1].y -= size_;
				}
			}
			player_[0]->GhostDraw(pos[0]);
			player_[1]->GhostDraw(pos[1]);
		}
	}
}

void Stage::OjamaDraw(void)
{
	int no = 0;
	for (auto& jama : ojamalist_)
	{
		jama->Draw(offset_);
		no++;
	}
}

void Stage::SetCntrol(ControlType& type)
{
	ctype_ = type;
	switch (ctype_)
	{
	case ControlType::KEY:
		controller_ = std::make_unique<KeyBoard>();
		break;
	case ControlType::XBOX_CONTROLLER:
		controller_ = std::make_unique<XboxController>();
		break;
	case ControlType::MOUSE:
		controller_ = std::make_unique<Mouse>();
		break;
	}
	controller_->SetUp(id_);
}


