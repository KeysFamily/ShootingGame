//-------------------------------------------------------------------
//�G���f�B���O
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ending.h"

#include  "easing.h"
#include  "randomLib.h"
#include  "MyAssistLib.h"
#include  "sound.h"

#include  "Task_Back.h"

#include  "Task_Title.h"
namespace  Ending
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imgStartLogo = DG::Image::Create("./data/image/StartKey.png");
		this->imgChar = DG::Image::Create("./data/image/font_number.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->imgStartLogo.reset();
		this->imgChar.reset();
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
		//ge->score = 99999;
		this->render2D_Priority[1] = 0.1f;
		this->effectCnt = 0;
		this->flashCnt = 0;
		
		//�C�[�W���O�̏�����
		easing::Init();
		easing::Create("ScoreEffect", easing::EASINGTYPE::EXPOINOUT, 0.0f, static_cast<float>(ge->score), 30);

		//�T�E���h
		
		se::LoadFile("poku", "./data/sound/poku.wav");
		se::LoadFile("ok", "./data/sound/ok.wav");
		se::LoadFile("bomb", "./data/sound/bom.wav");

		//���^�X�N�̐���

		return true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G(Back::defGroupName);

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			auto  nextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState();

		//�C�[�W���O�X�V����
		easing::UpDate();

		if (this->effectCnt >= 60) {
			//�����C�[�W���O�����삵�Ă��Ȃ����(�J�E���gup�̃C�[�W���O���J�n)
			if (easing::GetState("ScoreEffect") == easing::EQ_STATE::EQ_SET) {
				easing::Start("ScoreEffect");
			}
			if (this->effectCnt < 120 && this->effectCnt % 30 == 0) {
				//����炷
				se::Play("poku");
			}
			if (this->effectCnt == 120) {
				//�X�R�A�ɉ����Ĕ����G�t�F�N�g�𐶐��i�����̊֌W�ŏ����100�̂ɐݒ�j
				for (int i = 0; i < min(ge->score / 10, 100); ++i) {
					float x = 480.0f + GetRandom<float>(-450.0f, 450.0f);
					float y = ge->screenHeight / 2.0f + GetRandom<float>(-320.0f, 320.0f);
					ge->CreateEffect(9, ML::Vec2(x, y));
				}
				if (ge->score > 0) {
					//1�̈ȏア���甚������炷
					se::Play("bomb");
				}
			}
		}

		//ST�{�^���������ꂽ��
		if (inp.ST.down && ge->getCounterFlag("EndFade") != ge->ACTIVE) {
			se::Play("ok");
			ge->StartCounter("EndFade", 45); //�t�F�[�h��90�t���[���Ȃ̂Ŕ�����45�Ő؂�ւ�
			ge->CreateEffect(99, ML::Vec2(0, 0));
		}
		if (ge->getCounterFlag("EndFade") == ge->LIMIT) {
			this->Kill();
		}
		//�J�E���^�̃J�E���g�A�b�v
		if (this->effectCnt <= 120) {
			++this->effectCnt;
		}
		else {
			this->flashCnt = (flashCnt + 1) % 60;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		if (this->effectCnt >= 60) {
			//�X�R�A�\��
			ML::Box2D drawScoreLabel(250, 250, 224, 56);
			ML::Box2D srcScoreLabel(0, 66, 112, 28);
			this->res->imgChar->Draw(drawScoreLabel, srcScoreLabel);

			int digits = 5;
			//1�̈ʂ��珇�Ԃɕ`�悷��
			for (int i = digits - 1, score = static_cast<int>(easing::GetPos("ScoreEffect")); i >= 0; --i) {
				int num = score % 10;

				ML::Box2D drawScore(290, 320, 80, 112);
				drawScore.Offset(i * drawScore.w, 0);
				ML::Box2D srcScore(0, 34, 20, 28);
				srcScore.Offset(num * srcScore.w, 0);
				this->res->imgChar->Draw(drawScore, srcScore);

				score /= 10;
				//�X�R�A��0�Ȃ烋�[�v�𔲂���
				if (score == 0) { break; }
			}
		}
		if (this->effectCnt >= 60 + 60) {
			if (this->flashCnt / 30 == 0) {
				ML::Box2D drawStart = Assist::BoxCentering(ge->screenWidth * 3 / 8, ge->screenHeight * 3 / 4, 596 / 2, 91 / 2);
				ML::Box2D srcStart(0, 0, 596, 91);
				this->res->imgStartLogo->Draw(drawStart, srcStart);
			}
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