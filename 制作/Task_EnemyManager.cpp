//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_EnemyManager.h"

#include  "randomLib.h"

#include  "Task_Enemy00.h"
#include  "Task_Enemy01.h"
#include  "Task_EnemyBoss.h"
#include  "Task_EnemyShot00.h"

namespace  EnemyManager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
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
		this->appearCnt = 0;
		this->isAppearBoss = false;

		this->residentResource.push_back(Enemy00::Resource::Create());
		this->residentResource.push_back(Enemy01::Resource::Create());
		this->residentResource.push_back(EnemyBoss::Resource::Create());
		this->residentResource.push_back(EnemyShot00::Resource::Create());

		//���^�X�N�̐���
#if false
		auto boss = EnemyBoss::Object::Create(true);

		boss->pos = ML::Vec2(480.0f, 200.0f);
		boss->mode = EnemyBoss::Object::MoveMode::Pattern3;
#else
		//this->AppearEnemy(10);
		this->ModeMap_Initialize();
		this->mode = Mode::AppearNormalEnemy;
#endif
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G(Enemy00::defGroupName);
		ge->KillAll_G(Enemy01::defGroupName);
		ge->KillAll_G(EnemyBoss::defGroupName);
		ge->KillAll_G(EnemyShot00::defGroupName);

		this->residentResource.clear();
		this->ModeMap_Finalize();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		if (ge->GameStartFlag) { return; }
#if true
		this->ModeMap_UpDate(this->mode);

#else
		if (!this->isAppearBoss) {
			if (ge->score > 100) {
				this->AppearEnemy(10);
				this->isAppearBoss = true;
			}
			else {
				this->appearCnt = (this->appearCnt + 1) % 60;
			}
#if true
			if (this->appearCnt % 10 == 0) {
				this->AppearEnemy(0);
			}
			if (this->appearCnt % 30 == 0) {
				this->AppearEnemy(1);
			}
#endif
		}

#endif
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
	}

	void  Object::AppearEnemy(int enemyNum_) {
		BChara::SP enemy;
		switch (enemyNum_)
		{
		case 0:
			enemy = Enemy00::Object::Create(true);
			enemy->pos = ML::Vec2(
				GetRandom<float>(enemy->hitBase.w / 2.0f, 960.0f - enemy->hitBase.w / 2.0f),
				-enemy->hitBase.h / 2.0f
			);
			enemy->moveVec = ML::Vec2(0.0f, GetRandom<float>(4.0f, 6.0f));
			break;
		case 1:
			enemy = Enemy01::Object::Create(true);
			enemy->pos = ML::Vec2(
				GetRandom<float>(enemy->hitBase.w / 2.0f, 960.0f - enemy->hitBase.w / 2.0f),
				-enemy->hitBase.h / 2.0f
			);
			enemy->moveVec = ML::Vec2(0.0f, GetRandom<float>(4.0f, 6.0f));
			break;

		case 10:
			enemy = EnemyBoss::Object::Create(true);
			enemy->pos = ML::Vec2(
				480.0f,
				-enemy->hitBase.h / 2.0f
			);
		default:
			break;
		}
	}
	
	void Object::ModeMap_Initialize() {
		this->modeMap[Mode::AppearNormalEnemy] = std::bind(&Object::AppearNormalEnemy_UpDate, this);
		this->modeMap[Mode::CoolTimeUpToBoss] = std::bind(&Object::CoolTimeUpToBoss_UpDate, this);
		this->modeMap[Mode::BossAction] = std::bind(&Object::BossAction_UpDate, this);
		this->modeMap[Mode::EnemyAnnihilation] = std::bind(&Object::EnemyAnnihilation_UpDate, this);
	}
	void Object::ModeMap_Finalize() {
		this->modeMap.clear();
	}
	void Object::ModeMap_UpDate(Mode mode_) {
		if (this->modeMap.count(mode_) == 1) {
			this->modeMap[mode_]();
		}
	}

	void Object::AppearNormalEnemy_UpDate() {
		if (ge->score > 100) {
			this->mode = Mode::CoolTimeUpToBoss;
		}
		else {
			this->appearCnt = (this->appearCnt + 1) % 60;
		}
#if true
		if (this->appearCnt % 10 == 0) {
			this->AppearEnemy(0);
		}
		if (this->appearCnt % 30 == 0) {
			this->AppearEnemy(1);
		}
#endif
	}
	void Object::CoolTimeUpToBoss_UpDate() {
		this->appearCnt = (this->appearCnt + 1) % 150;
		if (this->appearCnt % 30 == 0) {
			ge->CreateEffect(20, ML::Vec2(500, 500));
		}
		if (this->appearCnt >= 120) {
			this->appearCnt = 0;
			//ge->CreateEffect(10, ML::Vec2(500, 500));
			this->mode = Mode::BossAction;
			this->AppearEnemy(10);
		}
	}
	void Object::BossAction_UpDate() {
		// ���݂͏����Ȃ�
		if (ge->qa_Enemys->empty()) { 
			this->mode = Mode::EnemyAnnihilation;
		}
	}
	void Object::EnemyAnnihilation_UpDate() {
		ge->CreateEffect(50, ge->qa_Player->pos);
		ge->GameClearFlag = true;
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