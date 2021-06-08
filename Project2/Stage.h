#pragma once
#include<functional>
//#include <memory>
//#include <list>
//#include "common/Vector2.h"
//#include "puyo/Player.h"
#include "Nextpuyo.h"
#include "puyo/OjamaPuyo.h"
#include "AllControl/Control.h"
#include "Frame.h"
#include "Holdpuyo.h"
class playUnit;

#define STAGE_CHIP_X 8
#define STAGE_CHIP_Y 15

enum class StageMode
{
	DROP,
	FALL,
	ERASE,
	PUYON,
	MUNYON,
	WIN,
	LOSE,
	DRAW,
	COOL,
	JAMA,
	MAX
};

enum class ResultMode
{
	WIN,
	LOSE,
	MAX
};

using ShareP = std::shared_ptr<Player>;

class Stage
{
public:
	Stage();
	Stage(const Vector2&& offset);
	~Stage();
	void Update(void);
	int GetScreen(void);
	Vector2 GetOffset(void);
	void Draw(void);
	void DrawMix(void);
	void GhostDraw(void);
	void OjamaDraw(void);

	bool GetOjamaFlag(void);
	int GetStgId(void);
	std::list<std::shared_ptr<OjamaPuyo>> GetSendList(void);
	ControlType GetCntType(void);
	void SetOjamaList(std::list<std::shared_ptr<OjamaPuyo>> ojama);
	void SetOjamaFlag(bool flag);
	void SwitchStgMode(void);
	std::pair<ResultMode, ResultMode> GetResult(void);
	void SetResult(ResultMode result);
	void SetCntrol(ControlType& control);


private:
	bool Init(void);
	void ChangeSignal(ShareP& puyo);
	void InsertPuyo(void);
	bool ErasePuyo(ShareP& puyo);
	void DeletePuyo(void);
	void StageReset(void);


	std::vector<ShareP> player_;
	std::unique_ptr<Control>controller_;
	StageMode stagemode_;

	const Vector2 stageSize;

	std::vector<ShareP> _dataBase;
	std::vector<ShareP*> _dataBase_ptr;

	std::vector<ShareP> _erasedata;
	std::vector<ShareP*> _erasedata_ptr;

	std::list<std::shared_ptr<OjamaPuyo>> ojamalist_;
	std::unique_ptr<Nextpuyo> next_;
	std::unique_ptr<Frame> frame_;
	std::unique_ptr<Holdpuyo> hold_;
	bool holdflag_;

	std::map<StageMode, std::function<bool(Stage&)>> stgupdate_;
	static constexpr int size_ = 48;

	bool nextflag;
	bool rensaflag;
	bool ojamaflag_;

	StageMode nextmode_;

	Vector2 pos_;
	Vector2 offset_;
	double rad_;
	int _stageScreen;
	int _drawStage;

	//bool delflag;
	int id_;
	static int count_;
	int cool_;			// ‚Õ‚æ’…’n‚µ‚½uŠÔ‚©‚ç‚Ìframe‹L˜^
	bool coolflag_;
	bool cooldownflag_;
	BitData bitdata_;
	std::unique_ptr<playUnit> playunit_;
	std::map<PUYO_TYPE, std::vector<int>> _puyoimage;	// ‰æ‘œ•Û‘¶
	std::map<ResultMode, int> image_;
	int puyonCnt_;

	int rensaMax_;			// Å‘å˜A½”
	int rensa_;				// ¡‰ñ‚Ì˜A½”
	int eraseCount_;		// Á‚µ‚½ŒÂ”
	bool nextojama_;
	std::list<std::shared_ptr<OjamaPuyo>> sendojama_;	// “G‚É‘—‚é‚¨‚¶‚á‚Ü

	std::array<int, 6> num;					// numbox —ñ‚ğŒ©‚Ä‚¨‚¶‚á‚Ü‚ÌêŠ‚ª‚©‚Ô‚Á‚Ä‚È‚¢‚©

	std::pair<ResultMode,ResultMode> result_;
	ControlType ctype_;

	nextPair data;
	// —F’Bˆê——
	friend class playUnit;
	friend struct puyodrop;
	friend struct puyorensa;
	friend struct puyofall;
	friend struct puyoerase;
	friend struct puyopuyon;
	friend struct puyomunyon;
	friend struct puyowin;
	friend struct puyolose;
	friend struct puyocool;
	friend struct puyojama;
};