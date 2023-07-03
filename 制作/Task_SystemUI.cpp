//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_SystemUI.h"

#include  "MyAssistLib.h"

namespace  SystemUI
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imgBack = DG::Image::Create("./data/image/BG0.png");
		this->imgChar = DG::Image::Create("./data/image/font_number.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->imgBack.reset();
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
		this->render2D_Priority[1] = 0.2f;
		this->startCnt = 3 * 60;
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
		//スタート処理
		if (ge->GameStartFlag) {
			--this->startCnt;
			if (startCnt < 0) { ge->GameStartFlag = false; }
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//背景表示
		ML::Box2D drawBG(960, 0, 480, 720);
		ML::Box2D srcBG(0, 0, 240, 360);
		this->res->imgBack->Draw(drawBG, srcBG);

		//カウントダウン
		if (ge->GameStartFlag) {
			ML::Box2D drawScore = Assist::BoxCentering(ge->screenWidth * 3 / 8, ge->screenHeight / 2, 80, 112)/*(960, 400, 40, 56)*/;
			ML::Box2D srcScore(0, 34, 20, 28);
			srcScore.Offset((this->startCnt / 60 + 1) * srcScore.w, 0);
			this->res->imgChar->Draw(drawScore, srcScore);
		}

		//残り体力表示
		ML::Box2D draw(960, 100, 125, 28);
		ML::Box2D src(0, 100, 125, 28);
		this->res->imgChar->Draw(draw, src);

		if (ge->qa_Player != nullptr) {
			this->DrawChar(3, ge->qa_Player->hp, ML::Box2D(960, 130, 40, 56));
		}

		//スコア表示
		ML::Box2D drawScoreLabel(960, 350, 112, 28);
		ML::Box2D srcScoreLabel(0, 66, 112, 28);
		this->res->imgChar->Draw(drawScoreLabel, srcScoreLabel);

		this->DrawChar(5, ge->score, ML::Box2D(960, 400, 40, 56));
	}

	void Object::DrawChar(int digit_, int num_,const ML::Box2D& size_) {
		for (int i = digit_ - 1, value = num_; i >= 0; --i) {
			int num = value % 10;
			value /= 10;

			ML::Box2D drawScore = size_;
			drawScore.Offset(i * drawScore.w, 0);
			ML::Box2D srcScore(0, 34, 20, 28);
			srcScore.Offset(num * srcScore.w, 0);
			this->res->imgChar->Draw(drawScore, srcScore);
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