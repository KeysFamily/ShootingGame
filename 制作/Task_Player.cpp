//-------------------------------------------------------------------
// プレイヤー
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "sound.h"

#include  "Task_Shot00.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/player_sh.png");
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
		this->render2D_Priority[1] = 0.5f;
		this->hitBase = Assist::BoxCentering(64, 64);
		this->shotCnt = 0;
		this->hp = 10;
		this->unhitCnt = 0;

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
		auto inp = ge->in1->GetState();

		//Lスティックの入力があるか？
		if (inp.LStick.volume > 0) {
			int key = 0;
			if (inp.LStick.BU.on) { key += 1; }
			if (inp.LStick.BD.on) { key += 2; }
			if (inp.LStick.BL.on) { key += 4; }
			if (inp.LStick.BR.on) { key += 8; }
			
			if (key <= 10) {
				this->moveVec = this->MoveSet(key, 8.0f);
				this->CharaMove(this->moveVec);
			}
		}
		//プレイヤの弾（同時撃ちは不可能）
		if (inp.B1.on || inp.B2.on || inp.B3.on) {
			this->shotCnt = (this->shotCnt + 1) % 16;
		}
		//単発
		if (inp.B1.on && inp.B2.off && inp.B3.off) {
			if (this->shotCnt % 4 == 0) {
				auto shot = Shot00::Object::Create(true);
				shot->pos = this->pos + ML::Vec2(0, -36);
				shot->moveVec = ML::Vec2(0.0f, -20.0f);
			}
		}
		//3Way弾
		else if (inp.B1.off && inp.B2.on && inp.B3.off) {
			if (this->shotCnt % 8 == 0) {
				for (int i = -1; i <= 1; ++i) {
					auto shot = Shot00::Object::Create(true);
					shot->pos = this->pos + ML::Vec2(0, -36);

					shot->angle = ML::ToRadian(20.0f * i);
					shot->moveVec = ML::Vec2(
						22.5f * sin(shot->angle),
						22.5f * -cos(shot->angle)
					);
				}
			}
		}
		//変則5Way弾
		else if (inp.B1.off && inp.B2.off && inp.B3.on) {
			if (this->shotCnt % 16 == 0) {
				float angles[5] = { -160.0f, -50.0f ,0.0f ,50.0f ,160.0f };
				for (const float& angle : angles) {
					auto shot = Shot00::Object::Create(true);
					shot->pos = this->pos;

					shot->angle = ML::ToRadian(angle);
					shot->moveVec = ML::Vec2(
						17.50f * sin(shot->angle),
						17.50f * -cos(shot->angle)
					);
				}
			}
		}
		if (this->unhitCnt > 0) {
			--this->unhitCnt;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->OffsetBox();
		ML::Box2D src(64, 0, 32, 32);
		if (this->unhitCnt > 0) {
			this->res->img->Draw(draw, src, ML::Color(1.0f, 1.0f, 0.0f, 0.0f));
		}
		else {
			this->res->img->Draw(draw, src);
		}
	}


	//-------------------------------------------------------------------
	//接触後の処理
	void Object::Received(BChara* from_, AttackInfo at_) {
		if (this->unhitCnt > 0) { return; }
		this->unhitCnt = 30;
		this->hp -= at_.power;

		if (this->hp <= 0) {
			this->Kill();
			ge->GameOverFlag = true;

			ge->CreateEffect(9, this->pos);
			se::Play("bomb");
		}
		else if (at_.power > 0 && hp > 0) {
			ge->CreateEffect(8, this->pos);
			se::Play("bomb");
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