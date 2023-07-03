#pragma once
#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//キャラクタ汎用スーパークラス
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"
#include "MyAssistLib.h"

class BChara : public BTask
{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	//キャラクタ共通メンバ変数
	ML::Vec2    pos;		//キャラクタ位置
	ML::Box2D   hitBase;	//あたり判定範囲
	ML::Vec2	moveVec;	//移動ベクトル
	int			moveCnt;	//行動カウンタ
	int			hp;			//体力
	//向き（2D視点）
	float angle;

	//攻撃情報
	struct AttackInfo
	{
		int power;//攻撃の威力
	}; 

	//メンバ変数に最低限の初期化を行う
	//★★メンバ変数を追加したら必ず初期化も追加する事★★
	BChara()
		: pos(0, 0)
		, hitBase(0, 0, 0, 0)
		, moveVec(0, 0)
		, moveCnt(0)
		, angle(0.f)
		, hp(1)
	{
	}
	virtual  ~BChara() {}

	ML::Box2D OffsetBox();
	//接触時の応答処理(受け身として実装)攻撃を受ける
	virtual void Received(BChara* from_, AttackInfo at_);
	//接触判定
	virtual bool CheckHit(const ML::Box2D& hit_);
protected:
	ML::Vec2 MoveSet(int key, float spd = 1.0f);
	void CharaMove(ML::Vec2& e_);
	bool CheckScreenOut(ML::Box2D hit_);

	bool CheckScreenFullOut();



};
