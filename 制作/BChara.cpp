//-----------------------------------------------------------------------------
//�L�����N�^�ėp�X�[�p�[�N���X
//-----------------------------------------------------------------------------
#include "BChara.h"
#include  "MyPG.h"



ML::Box2D BChara::OffsetBox() {
	return this->hitBase.OffsetCopy(this->pos);
}


ML::Vec2 BChara::MoveSet(int key, float spd)
{
					//0, 1, 2,  3,  4,  5,  6, 7, 8, 9,10,
	int table[] = { -1,90,270,-1,180,135,225,-1, 0,45,315 };
	//float spd = 10.5f;//�X�s�[�h
	ML::Vec2 est;
	//0�͓������
	if (key == 0)
	{
		est.x = 0.f;
		est.y = 0.f;
	}
	else
	{
		est.x = (float)cos(table[key] * D3DX_PI / 180.f) * spd;
		est.y = (float)-sin(table[key] * D3DX_PI / 180.f) * spd;
	}
	return est;
}

//�͂ݏo���h�~�ړ������i�͂ݏo������true��Ԃ��j
void BChara::CharaMove(ML::Vec2& e_) {
	while (e_.x != 0.0f) {
		float preX = pos.x;
		if (e_.x >= +1.0f) { pos.x += 1.0f, e_.x -= 1.0f; }
		else if (e_.x <= -1.0f) { pos.x -= 1.0f; e_.x += 1.0f; }
		else { pos.x += e_.x; e_.x = 0.0f; }
		//�����蔻��p�̋�`��p��
		ML::Box2D hit = hitBase.OffsetCopy(pos);
		if (this->CheckScreenOut(hit)) {
			//����������Q�[���I�[�o�[
			pos.x = preX;
			break;
		}
	}

	while (e_.y != 0.0f) {
		float preY = pos.y;
		if (e_.y >= +1.0f) { pos.y += 1.0f, e_.y -= 1.0f; }
		else if (e_.y <= -1.0f) { pos.y -= 1.0f; e_.y += 1.0f; }
		else { pos.y += e_.y; e_.y = 0.0f; }
		//�����蔻��p�̋�`��p��
		ML::Box2D hit = hitBase.OffsetCopy(pos);
		if (this->CheckScreenOut(hit)) {
			//����������Q�[���I�[�o�[
			pos.y = preY;
			break;
		}
	}
}

//��ʊO�ɂł���true��Ԃ�
bool BChara::CheckScreenOut(ML::Box2D hit_) {
	//�w�K�p����
	//Rect��Box2D�̕ϊ��ɂ���
	//ML::Rect o{ hit_.x,hit_.y ,hit_.x + hit_.w,hit_.y + hit_.h };

	ML::Rect s{ 0,0,960,static_cast<int>(ge->screenHeight) };
	ML::Rect o{ hit_.x,hit_.y ,hit_.x + hit_.w,hit_.y + hit_.h };

	//�ǂ������͂ݏo���Ă���Ή�ʂ���͂ݏo���Ă���
	if (o.left < s.left) { return true; }
	if (o.top < s.top) { return true; }
	if (o.right > s.right) { return true; }
	if (o.bottom > s.bottom) { return true; }

	return false;
}

//���S�ɉ�ʊO�ɏo����true��Ԃ�
bool BChara::CheckScreenFullOut() {
	ML::Box2D screen(0, 0, 960, static_cast<int>(ge->screenHeight));
	ML::Box2D chara = this->OffsetBox();
#if false
	return !screen.Hit(chara);
#else
	ML::Rect s{ 0,0,960,static_cast<int>(ge->screenHeight) };
	ML::Rect o{ chara.x,chara.y ,chara.x + chara.w,chara.y + chara.h };
	//if(chara)
	if (o.right < s.left) { return true; }
	if (o.bottom < s.top) { return true; }
	if (o.top > s.bottom) { return true; }
	if (o.left > s.right) { return true; }

	return false;
#endif

}

//-----------------------------------------------------------------------------
//�ڐG���̉�������
void BChara::Received(BChara* from_, AttackInfo at_) {
	ML::MsgBox("��������Ă��܂���");
}

//-----------------------------------------------------------------------------
//�ڐG����
bool BChara::CheckHit(const ML::Box2D& hit_) {
	ML::Box2D me = this->OffsetBox();
	return me.Hit(hit_);
}