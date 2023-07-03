//-------------------------------------------------------------------
//敵
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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/Enemy.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->img.reset();
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
		this->hitBase = Assist::BoxCentering(32, 32);
		this->shotCnt = GetRandom<int>(10, 30);


		se::LoadFile("bomb", "./data/sound/bom.wav");

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		this->pos += this->moveVec;

		if (this->shotCnt == 0 && this->pos.y < ge->qa_Player->pos.y) {
			//自機狙い弾生成
			float angle = atan2(ge->qa_Player->pos.y - this->pos.y, ge->qa_Player->pos.x - this->pos.x);
			auto e_shot = EnemyShot00::Object::Create(true);
			e_shot->pos = this->pos;
			e_shot->moveVec = ML::Vec2(
				4 * cos(angle),
				4 * sin(angle)
			);
		}
		
		//当たり判定
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
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->OffsetBox();
		ML::Box2D src(32, 0, 32, 32);
		this->res->img->Draw(draw, src);
	}

	//-------------------------------------------------------------------
	//接触後の処理
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