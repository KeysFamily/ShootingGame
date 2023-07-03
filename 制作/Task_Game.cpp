//-------------------------------------------------------------------
//ゲーム本編
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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		
		//ゲームエンジン共有変数
		ge->GameStartFlag = true;
		ge->GameOverFlag = false;
		ge->GameClearFlag = false;
		ge->score = 0;

		//リソース常駐処理
		this->residentResource.push_back(Shot00::Resource::Create());
		
		//デバッグ用フォントの準備
		this->TestFont = DG::Font::Create("ＭＳ ゴシック", 30, 30);

		//サウンド
		//bgm::LoadFile("bgm","")

		//★タスクの生成
		Back::Object::Create(true);

		auto pl = Player::Object::Create(true);
		pl->pos = ML::Vec2(480.0f, 640.0f);

		EnemyManager::Object::Create(true);
		SystemUI::Object::Create(true);
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("本編");
		ge->KillAll_G(Player::defGroupName);
		ge->KillAll_G(EnemyManager::defGroupName);
		ge->KillAll_G(SystemUI::defGroupName);

		//リソース常駐解除
		this->residentResource.clear();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			auto next = Ending::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState( );
		ge->qa_Enemys = ge->GetTasks<BChara>("敵");
		ge->qa_Player = ge->GetTask<Player::Object>("プレイヤ");
		if (inp.B4.down) {
			ge->CreateEffect(50, ge->qa_Player->pos);
		}

		if (ge->GameClearFlag || ge->GameOverFlag) {
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		

	}

	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
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
	//リソースクラスの生成
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