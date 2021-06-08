#pragma once
#include "Player.h"
class OjamaPuyo :
	public Player
{
public:
	OjamaPuyo(Vector2&& pos, int size, puyoMap&& puyo, PUYO_TYPE type);
	~OjamaPuyo();
	bool Update(BitData flag);
	void Draw(void);
	void Draw(Vector2 offset);
	bool GetStayFlag(void);

	void SetStayFlag(bool flag);
	void SetMunyon(BitData munyon);
private:

	bool stayflag_;
};

