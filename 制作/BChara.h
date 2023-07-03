#pragma once
#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//�L�����N�^�ėp�X�[�p�[�N���X
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"
#include "MyAssistLib.h"

class BChara : public BTask
{
	//�ύX�s����������������������������������������������������
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//�ύX������������������������������������������������������
	//�L�����N�^���ʃ����o�ϐ�
	ML::Vec2    pos;		//�L�����N�^�ʒu
	ML::Box2D   hitBase;	//�����蔻��͈�
	ML::Vec2	moveVec;	//�ړ��x�N�g��
	int			moveCnt;	//�s���J�E���^
	int			hp;			//�̗�
	//�����i2D���_�j
	float angle;

	//�U�����
	struct AttackInfo
	{
		int power;//�U���̈З�
	}; 

	//�����o�ϐ��ɍŒ���̏��������s��
	//���������o�ϐ���ǉ�������K�����������ǉ����鎖����
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
	//�ڐG���̉�������(�󂯐g�Ƃ��Ď���)�U�����󂯂�
	virtual void Received(BChara* from_, AttackInfo at_);
	//�ڐG����
	virtual bool CheckHit(const ML::Box2D& hit_);
protected:
	ML::Vec2 MoveSet(int key, float spd = 1.0f);
	void CharaMove(ML::Vec2& e_);
	bool CheckScreenOut(ML::Box2D hit_);

	bool CheckScreenFullOut();



};
