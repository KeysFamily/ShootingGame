//-------------------------------------------------------------------
//Q[{า
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"

#include  "randomLib.h"
#include  "sound.h"

#include  "Task_Back.h"
#include  "Task_Player.h"
#include  "Task_Shot00.h"
#include  "Task_EnemyManager.h"
#include  "Task_SystemUI.h"

#include  "Task_Ending.h"

namespace  Game
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[Xฬ๚ป
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//\[Xฬ๐๚
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//u๚ปv^XNถฌษP๑พฏsค
	bool  Object::Initialize()
	{
		//X[p[NX๚ป
		__super::Initialize(defGroupName, defName, true);
		//\[XNXถฌor\[XคL
		this->res = Resource::Create();

		//f[^๚ป
		
		//Q[GWคLฯ
		ge->GameStartFlag = true;
		ge->GameOverFlag = false;
		ge->GameClearFlag = false;
		ge->score = 0;

		//\[Xํ
		this->residentResource.push_back(Shot00::Resource::Create());
		
		//fobOptHgฬ๕
		this->TestFont = DG::Font::Create("lr SVbN", 30, 30);

		//TEh
		//bgm::LoadFile("bgm","")

		//^XNฬถฌ
		Back::Object::Create(true);

		auto pl = Player::Object::Create(true);
		pl->pos = ML::Vec2(480.0f, 640.0f);

		EnemyManager::Object::Create(true);
		SystemUI::Object::Create(true);
		return  true;
	}
	//-------------------------------------------------------------------
	//uIนv^XNมลษP๑พฏsค
	bool  Object::Finalize()
	{
		//f[^^XN๐๚
		ge->KillAll_G("{า");
		ge->KillAll_G(Player::defGroupName);
		ge->KillAll_G(EnemyManager::defGroupName);
		ge->KillAll_G(SystemUI::defGroupName);

		//\[Xํ๐
		this->residentResource.clear();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//๘ซpฌ^XNฬถฌ
			auto next = Ending::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[ษsค
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState( );
		ge->qa_Enemys = ge->GetTasks<BChara>("G");
		ge->qa_Player = ge->GetTask<Player::Object>("vC");
		if (inp.B4.down) {
			ge->CreateEffect(50, ge->qa_Player->pos);
		}

		if (ge->GameClearFlag || ge->GameOverFlag) {
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//uQc`ๆvPt[ษsค
	void  Object::Render2D_AF()
	{
		

	}

	//
	//ศบอ๎{IษฯXsvศ\bh
	//
	//-------------------------------------------------------------------
	//^XNถฌ๛
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWษo^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYษธsตฝ็Kill
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
	//\[XNXฬถฌ
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