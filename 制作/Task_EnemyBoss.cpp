//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_EnemyBoss.h"

#include  "randomLib.h"

#include  "Task_EnemyShot00.h"

namespace  EnemyBoss
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/shoot.png");
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
		this->hitBase = Assist::BoxCentering(128, 96);
		this->mode = MoveMode::Appearance;
		this->tgVec = ML::Vec2(480.0f, ge->screenHeight / 3.0f);
		this->hp = 100;

		MoveMap_Initialize();
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		MoveMap_Finalize();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		int max = 100;
		if (this->mode != MoveMode::Appearance) {
			if (this->hp > max * 3 / 4) {
				this->mode = MoveMode::Pattern1;

			}
			else if (this->hp > max / 2) {
				this->mode = MoveMode::Pattern3;

			}
			else if (this->hp > max / 4) {
				this->mode = MoveMode::Pattern0;

			}
			else {
				this->mode = MoveMode::Pattern2;
			}
		}
		MoveMap_UpDate(this->mode);
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->OffsetBox();
		ML::Box2D src(0, 96, 64, 48);
		this->res->img->Draw(draw, src);
	}

	void Object::AppearEnemyShot(ML::Vec2 pos_, ML::Vec2 moveVec_) {
		auto e_shot = EnemyShot00::Object::Create(true);
		e_shot->pos = pos_;
		e_shot->moveVec = moveVec_;
	}

	void Object::MoveMap_Initialize() {
		this->modeMap[MoveMode::Appearance] = std::bind(&Object::Appearance_UpDate, this);
		this->modeMap[MoveMode::Pattern0] = std::bind(&Object::Pattern0_UpDate, this);
		this->modeMap[MoveMode::Pattern1] = std::bind(&Object::Pattern1_UpDate, this);
		this->modeMap[MoveMode::Pattern2] = std::bind(&Object::Pattern2_UpDate, this);
		this->modeMap[MoveMode::Pattern3] = std::bind(&Object::Pattern3_UpDate, this);
	}

	void Object::MoveMap_Finalize() {
		this->modeMap.clear();
	}

	void Object::MoveMap_UpDate(MoveMode mode_) {
		if (this->modeMap.count(mode_) == 1) {
			this->modeMap[mode_]();
		}
	}

	void Object::Appearance_UpDate() {
		ML::Vec2 toVec = this->tgVec - this->pos;
		this->pos += toVec * 0.05f;

		if (this->pos.y >= ge->screenHeight / 3.0f - 1.0f) {
			this->mode = MoveMode::Pattern0;
		}
	}
	void Object::Pattern0_UpDate() {
		//---------------------------------------------------------------------------
		// Pattern1
		// ランダムな位置に敵が移動、左右から扇状の弾を撃つ
		//---------------------------------------------------------------------------


		++this->moveCnt;

#if false
		if (this->moveCnt < 18 && this->moveCnt % 2 == 0) {

			//時計回り
			AppearEnemyShot(
				this->pos + ML::Vec2(-32.0f, 32.0f),
				ML::Vec2(
					5.0f * cos(ML::ToRadian(this->moveCnt / 2 * 20.0f)),
					5.0f * sin(ML::ToRadian(this->moveCnt / 2 * 20.0f))
				)
			);

			//反時計回り
			AppearEnemyShot(
				this->pos + ML::Vec2(32.0f, 32.0f) ,
				ML::Vec2(
					5.0f * -cos(ML::ToRadian(this->moveCnt / 2 * 20.0f)),
					5.0f * sin(ML::ToRadian(this->moveCnt / 2 * 20.0f))
				)
			);
		}
		else if (moveCnt > 210) {
			moveCnt = 0;
		}
#else

		if (this->moveCnt < 18 && this->moveCnt % 2 == 0) {
			float angle = 20.0f * moveCnt / 2;
			//時計回り
			AppearEnemyShot(
				this->pos + ML::Vec2(-40.0f, 48.0f),
				ML::Vec2(
					5.0f * cos(ML::ToRadian(angle)),
					5.0f * sin(ML::ToRadian(angle))
				)
			);
		}
		else if (this->moveCnt >= 60 && this->moveCnt < 78 && this->moveCnt % 2 == 0) {
			float angle = 20.0f * (moveCnt - 60) / 2;
			
			//反時計回り
			AppearEnemyShot(
				this->pos + ML::Vec2(40.0f, 48.0f),
				ML::Vec2(
					5.0f * -cos(ML::ToRadian(angle)),
					5.0f * sin(ML::ToRadian(angle))
				)
			);
		}
		else if (moveCnt > 120) {
			moveCnt = 0;
			auto pl = ge->GetTask<Player::Object>(Player::defGroupName, Player::defName);

			//this->tgVec.x = GetRandom<float>(960.0f / 4, 960.0f * 3 / 4);
			//this->tgVec.x = pl->pos.x;
			if (pl->pos.x > 480) {
				this->tgVec.x = GetRandom<float>(this->hitBase.w / 2.0f, 480.0f);
			}
			else {
				this->tgVec.x = GetRandom<float>(480.0f, 960.0f - this->hitBase.w / 2.0f);
			}

			this->tgVec.y = GetRandom<float>(this->hitBase.h / 2.0f, ge->screenHeight / 3.0f - this->hitBase.h / 2.0f);
		}

		ML::Vec2 toVec = this->tgVec - this->pos;
		this->pos += toVec * 0.05f;
#endif
	}
	void Object::Pattern1_UpDate() {
		//---------------------------------------------------------------------------
		// Pattern1の動作
		// 12Way弾を一定間隔で撃つ
		//---------------------------------------------------------------------------

		if (this->moveCnt % 60 == 0) {
			float offsetAngle = GetRandom<float>(-10.0f, 10.0f);

			for (int i = 0; i < 18; ++i) {
				float angle = i * 20 + offsetAngle;
				this->AppearEnemyShot(
					this->pos,
					ML::Vec2(
						5.0f * cos(ML::ToRadian(angle)),
						5.0f * sin(ML::ToRadian(angle))
					)
				);
			}
			this->moveCnt = 0;
		}
		++this->moveCnt;
	}
	void Object::Pattern2_UpDate() {
		//---------------------------------------------------------------------------
		// Pattern2
		// Pattern0より、弾の角度が大きく、移動間隔が短い
		//---------------------------------------------------------------------------


		++this->moveCnt;

#if false
		if (this->moveCnt < 18 && this->moveCnt % 2 == 0) {

			//時計回り
			AppearEnemyShot(
				this->pos + ML::Vec2(-32.0f, 32.0f),
				ML::Vec2(
					5.0f * cos(ML::ToRadian(this->moveCnt / 2 * 20.0f)),
					5.0f * sin(ML::ToRadian(this->moveCnt / 2 * 20.0f))
				)
			);

			//反時計回り
			AppearEnemyShot(
				this->pos + ML::Vec2(32.0f, 32.0f),
				ML::Vec2(
					5.0f * -cos(ML::ToRadian(this->moveCnt / 2 * 20.0f)),
					5.0f * sin(ML::ToRadian(this->moveCnt / 2 * 20.0f))
				)
			);
		}
		else if (moveCnt > 210) {
			moveCnt = 0;
		}
#else

		if (this->moveCnt < 15 ) {
			float angle = moveCnt * 20.0f + 5.0f;
			//時計回り
			AppearEnemyShot(
				this->pos + ML::Vec2(40.0f, 48.0f),
				ML::Vec2(
					5.0f * cos(ML::ToRadian(angle)),
					5.0f * sin(ML::ToRadian(angle))
				)
			);
			//反時計回り
			AppearEnemyShot(
				this->pos + ML::Vec2(-40.0f, 48.0f),
				ML::Vec2(
					5.0f * -cos(ML::ToRadian(angle)),
					5.0f * sin(ML::ToRadian(angle))
				)
			);
		}
		else if (moveCnt > 60) {
			moveCnt = 0;
			auto pl = ge->GetTask<Player::Object>(Player::defGroupName, Player::defName);

			//this->tgVec.x = GetRandom<float>(960.0f / 4, 960.0f * 3 / 4);
			//this->tgVec.x = pl->pos.x;
			if (pl->pos.x > 480) {
				this->tgVec.x = GetRandom<float>(this->hitBase.w / 2.0f, 480.0f);
			}
			else {
				this->tgVec.x = GetRandom<float>(480.0f, 960.0f - this->hitBase.w / 2.0f);
			}

			this->tgVec.y = GetRandom<float>(this->hitBase.h / 2.0f, ge->screenHeight / 2.0f - this->hitBase.h / 2.0f);
		}

		ML::Vec2 toVec = this->tgVec - this->pos;
		this->pos += toVec * 0.05f;
#endif
	}
	void Object::Pattern3_UpDate() {
		//---------------------------------------------------------------------------
		// Pattern3
		// 60フレームに1回の自機狙い
		// 60フレームごとに方向をプレイヤとの方向±15度にする自機狙い1つ
		// 5フレームごとに方向をプレイヤとの方向±90度にする自機狙い2つ
		//---------------------------------------------------------------------------
		auto pl = ge->GetTask<Player::Object>(Player::defGroupName, Player::defName);
		if (pl == nullptr) { return; }

		ML::Vec2 targetVec = pl->pos - this->pos;
		float nowAngle = atan2(targetVec.y, targetVec.x);

		++this->moveCnt;

		if (moveCnt < 60 && moveCnt % 5 == 0) {
			AppearEnemyShot(
				this->pos,
				ML::Vec2(
					5.0f * cos(this->angle),
					5.0f * sin(this->angle)
				)
			);

			AppearEnemyShot(
				this->pos,
				ML::Vec2(
					5.0f * cos(nowAngle + ML::ToRadian(90.0f)),
					5.0f * sin(nowAngle + ML::ToRadian(90.0f))
				)
			);
			AppearEnemyShot(
				this->pos,
				ML::Vec2(
					5.0f * cos(nowAngle + ML::ToRadian(-90.0f)),
					5.0f * sin(nowAngle + ML::ToRadian(-90.0f))
				)
			);
		}
		else if (moveCnt % 30 == 0) {

			
			this->angle = nowAngle + ML::ToRadian(GetRandom<float>(-15.0f, 15.0f));

			float shotAngle;

			if (pl->pos.y > this->pos.y) {
				//右側
				targetVec = pl->pos - (this->pos + ML::Vec2(40.0f, 48.0f));
				shotAngle = atan2(targetVec.y, targetVec.x);

				AppearEnemyShot(
					this->pos + ML::Vec2(40.0f, 48.0f),
					ML::Vec2(
						5.0f * cos(shotAngle),
						5.0f * sin(shotAngle)
					)
				);

				//左側
				targetVec = pl->pos - (this->pos + ML::Vec2(-40.0f, 48.0f));
				shotAngle = atan2(targetVec.y, targetVec.x);

				AppearEnemyShot(
					this->pos + ML::Vec2(-40.0f, 48.0f),
					ML::Vec2(
						5.0f * cos(shotAngle),
						5.0f * sin(shotAngle)
					)
				);
			}
		}
		else if (this->moveCnt > 60) {
			this->moveCnt = 0;

		}
	}

	//-------------------------------------------------------------------
	//接触後の処理
	void Object::Received(BChara* from_, AttackInfo at_) {
		if (this->mode == MoveMode::Appearance) { return; }
		this->hp -= at_.power;
		++ge->score;
		//se::Play("bomb");
		if (this->hp <= 0) {
			this->Kill();

			ge->CreateEffect(8, this->pos);
			//ge->GameClearFlag = true;
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