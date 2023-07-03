//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"

#include  "randomLib.h"
#include  "sound.h"
#include  "easing.h"
#include  "Task_Effect00.h"
#include  "MyAssistLib.h"

#include  "Task_Game.h"


namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/titleback.png");
		this->TitleLogo = DG::Image::Create("./data/image/title.png");
		this->StartLogo = DG::Image::Create("./data/image/StartKey.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->img.reset();
		this->TitleLogo.reset();
		this->StartLogo.reset();
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
		this->controller = ge->in1;
		this->render2D_Priority[1] = 0.5f;
		int RenderTime = 0;

		ge->stage = 1;

		this->posTitle = ML::Vec2(ge->screenWidth / 2.0f, -40.0f);
		this->flashCnt = 0;

		//se
		se::LoadFile("ok","./data/sound/ok.wav");

		//�C�[�W���O����
		easing::Init();				//�C�[�W���O������

		//�C�[�W���O�쐬
		easing::Create(
			"titleYIn",					//�C�[�W���O��[string]
			easing::EASINGTYPE::BACKOUT,//�C�[�W���O�^�C�v	
			this->posTitle.y,			//�����l[float]
			ge->screenHeight / 2.0f,	//�I���l[float]
			2 * 60);					//���v����[�t���[��/int]

		easing::Start("titleYIn");//�C�[�W���O�J�n

		//���^�X�N�̐���
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto inp = this->controller->GetState();

		//�C�[�W���O�̍X�V����
		easing::UpDate();

		if (inp.ST.down && ge->getCounterFlag("StartFade") != ge->ACTIVE) {
			se::Play("ok");
			ge->StartCounter("StartFade", 45); //�t�F�[�h��90�t���[���Ȃ̂Ŕ�����45�Ő؂�ւ�
			ge->CreateEffect(98, ML::Vec2(0, 0));
		}
		if (ge->getCounterFlag("StartFade") == ge->LIMIT) {
			this->Kill();
		}

		this->posTitle.y = easing::GetPos("titleYIn");
		this->flashCnt = (this->flashCnt + 1) % 60;

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D drawBG(0, 0, ge->screenWidth, ge->screenHeight);
		ML::Box2D srcBG(0, 0, 1920, 1200);
		this->res->img->Draw(drawBG, srcBG);

		ML::Box2D drawTitle = Assist::BoxCentering(this->posTitle, 662, 78);
		ML::Box2D srcTitle(0, 0, 662, 78);
		this->res->TitleLogo->Draw(drawTitle, srcTitle);

		if (this->flashCnt / 30) {
			ML::Box2D drawStart = Assist::BoxCentering(ge->screenWidth / 2, ge->screenHeight * 3 / 4, 596, 91);
			ML::Box2D srcStart(0, 0, 596, 91);
			this->res->StartLogo->Draw(drawStart, srcStart);
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