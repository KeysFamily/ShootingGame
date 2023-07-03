//-------------------------------------------------------------------
//�G
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Enemy01.h"

#include  "Task_EnemyShot00.h"

#include  "randomLib.h"
#include  "sound.h"

namespace  Enemy01
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/Enemy.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->img.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->hitBase = Assist::BoxCentering(32, 32);
		this->shotCnt = GetRandom<int>(10, 30);


		se::LoadFile("bomb", "./data/sound/bom.wav");

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		this->pos += this->moveVec;

		if (this->shotCnt == 0 && this->pos.y < ge->qa_Player->pos.y) {
			//���@�_���e����
			float angle = atan2(ge->qa_Player->pos.y - this->pos.y, ge->qa_Player->pos.x - this->pos.x);
			auto e_shot = EnemyShot00::Object::Create(true);
			e_shot->pos = this->pos;
			e_shot->moveVec = ML::Vec2(
				4 * cos(angle),
				4 * sin(angle)
			);
		}
		
		//�����蔻��
		{
			ML::Box2D me = this->OffsetBox();

			if (ge->qa_Player == nullptr) { return; }
			
			if (ge->qa_Player->CheckHit(me)) {
				AttackInfo at = { 1 };
				ge->qa_Player->Received(this, at);
				this->Kill();
			}
		}

		if (this->CheckScreenFullOut()) { this->Kill(); }

		if (this->shotCnt >= 0) {
			--this->shotCnt;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->OffsetBox();
		ML::Box2D src(32, 0, 32, 32);
		this->res->img->Draw(draw, src);
	}

	//-------------------------------------------------------------------
	//�ڐG��̏���
	void Object::Received(BChara* from_, AttackInfo at_) {
		this->hp -= at_.power;
		++ge->score;
		se::Play("bomb");
		if (this->hp <= 0) {
			this->Kill();
			from_->Kill();

			ge->CreateEffect(8, this->pos);
		}
	}

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//���\�[�X�N���X�̐���
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}