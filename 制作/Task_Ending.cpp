//-------------------------------------------------------------------
//エンディング
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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imgStartLogo = DG::Image::Create("./data/image/StartKey.png");
		this->imgChar = DG::Image::Create("./data/image/font_number.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->imgStartLogo.reset();
		this->imgChar.reset();
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
		//ge->score = 99999;
		this->render2D_Priority[1] = 0.1f;
		this->effectCnt = 0;
		this->flashCnt = 0;
		
		//イージングの初期化
		easing::Init();
		easing::Create("ScoreEffect", easing::EASINGTYPE::EXPOINOUT, 0.0f, static_cast<float>(ge->score), 30);

		//サウンド
		
		se::LoadFile("poku", "./data/sound/poku.wav");
		se::LoadFile("ok", "./data/sound/ok.wav");
		se::LoadFile("bomb", "./data/sound/bom.wav");

		//★タスクの生成

		return true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G(Back::defGroupName);

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			auto  nextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState();

		//イージング更新処理
		easing::UpDate();

		if (this->effectCnt >= 60) {
			//もしイージングが動作していなければ(カウントupのイージングを開始)
			if (easing::GetState("ScoreEffect") == easing::EQ_STATE::EQ_SET) {
				easing::Start("ScoreEffect");
			}
			if (this->effectCnt < 120 && this->effectCnt % 30 == 0) {
				//音を鳴らす
				se::Play("poku");
			}
			if (this->effectCnt == 120) {
				//スコアに応じて爆発エフェクトを生成（処理の関係で上限を100体に設定）
				for (int i = 0; i < min(ge->score / 10, 100); ++i) {
					float x = 480.0f + GetRandom<float>(-450.0f, 450.0f);
					float y = ge->screenHeight / 2.0f + GetRandom<float>(-320.0f, 320.0f);
					ge->CreateEffect(9, ML::Vec2(x, y));
				}
				if (ge->score > 0) {
					//1体以上いたら爆発音を鳴らす
					se::Play("bomb");
				}
			}
		}

		//STボタンが押されたら
		if (inp.ST.down && ge->getCounterFlag("EndFade") != ge->ACTIVE) {
			se::Play("ok");
			ge->StartCounter("EndFade", 45); //フェードは90フレームなので半分の45で切り替え
			ge->CreateEffect(99, ML::Vec2(0, 0));
		}
		if (ge->getCounterFlag("EndFade") == ge->LIMIT) {
			this->Kill();
		}
		//カウンタのカウントアップ
		if (this->effectCnt <= 120) {
			++this->effectCnt;
		}
		else {
			this->flashCnt = (flashCnt + 1) % 60;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		if (this->effectCnt >= 60) {
			//スコア表示
			ML::Box2D drawScoreLabel(250, 250, 224, 56);
			ML::Box2D srcScoreLabel(0, 66, 112, 28);
			this->res->imgChar->Draw(drawScoreLabel, srcScoreLabel);

			int digits = 5;
			//1の位から順番に描画する
			for (int i = digits - 1, score = static_cast<int>(easing::GetPos("ScoreEffect")); i >= 0; --i) {
				int num = score % 10;

				ML::Box2D drawScore(290, 320, 80, 112);
				drawScore.Offset(i * drawScore.w, 0);
				ML::Box2D srcScore(0, 34, 20, 28);
				srcScore.Offset(num * srcScore.w, 0);
				this->res->imgChar->Draw(drawScore, srcScore);

				score /= 10;
				//スコアが0ならループを抜ける
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