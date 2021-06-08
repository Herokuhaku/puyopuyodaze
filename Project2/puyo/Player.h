#pragma once
#include <map>
#include <vector>
#include <array>
#include "../common/Vector2.h"
#include "../AllControl/INPUT_ID.h"

enum class PUYO_TYPE
{
	NON,
	RED,
	GREEN,
	BLUE,
	YELLOW,
	PURPLE,
	OJAMA,
	WALL,
	MAX
};

struct CheckBit
{
	// :コロンのあとに使いたいbit数を記入
	unsigned int left : 1;
	unsigned int right : 1;
	unsigned int down : 1;
	unsigned int up : 1;
};

union BitData
{
	CheckBit _bits;
	unsigned int flag;
};

using puyoMap = std::map<PUYO_TYPE, std::vector<int>>;

class Player
{
public:
	Player();
	Player(Vector2&& pos,int size,puyoMap puyo,PUYO_TYPE type);
	~Player();

	virtual bool Update(BitData flag);
	void Move(INPUT_ID id, BitData flag);
	virtual void Draw(void);
	void TopDraw(void);				// 操作中のぷよを強調表現
	void GhostDraw(Vector2 pos);			// ゴーストを表示

	const Vector2& GetPos(void);	// 座標
	const Vector2 GetGrid(void);	// グリッド番号
	PUYO_TYPE GetType(void);		// 自分の種類
	bool GetAlive(void);			// 生きてるか
	bool GetPuyonFlag(void);		// ぷよん中かどうか
	int GetMunyon(void);			// むにょんの種類

	void SetPos(Vector2 pos);
	void SetType(PUYO_TYPE type);
	void SetAlive(bool alive);
	void SetPuyon(int puyon);
	void SetPuyonFlag(bool puyon);
	virtual void SetMunyon(BitData munyon);
	void SetDropSpeed(int speed);
	void SetCountflag(int flag);
protected:
	void Init(void);

	std::map<PUYO_TYPE, std::vector<int>> _puyoimage;	// 画像保存

	Vector2 pos_;			// 座標
	BitData munyon_;
	PUYO_TYPE puyotype_;

	static int count_;
	int frame_;				// 経過フレーム
	int id_;				// player番号
	int puyon_;
	int size_;
	int countflag;
	int speed_;

	bool alive_;
	bool puyonflag_;

	int buf_;
	int ghost_;
private:

	Vector2 offset_;		// オフセット

	std::map<INPUT_ID, int> _keyList;
	std::map<int,std::pair<int, int>> _keystate;		// old,now
};

