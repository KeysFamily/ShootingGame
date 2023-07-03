//-------------------------------------------------------------------
//タイトル画面
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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/titleback.png");
		this->TitleLogo = DG::Image::Create("./data/image/title.png");
		this->StartLogo = DG::Image::Create("./data/image/StartKey.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->img.reset();
		this->TitleLogo.reset();
		this->StartLogo.reset();
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
		this->controller = ge->in1;
		this->render2D_Priority[1] = 0.5f;
		int RenderTime = 0;

		ge->stage = 1;

		this->posTitle = ML::Vec2(ge->screenWidth / 2.0f, -40.0f);
		this->flashCnt = 0;

		//se
		se::LoadFile("ok","./data/sound/ok.wav");

		//イージング処理
		easing::Init();				//イージング初期化

		//イージング作成
		easing::Create(
			"titleYIn",					//イージング名[string]
			easing::EASINGTYPE::BACKOUT,//イージングタイプ	
			this->posTitle.y,			//初期値[float]
			ge->screenHeight / 2.0f,	//終了値[float]
			2 * 60);					//所要時間[フレーム/int]

		easing::Start("titleYIn");//イージング開始

		//★タスクの生成
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto inp = this->controller->GetState();

		//イージングの更新処理
		easing::UpDate();

		if (inp.ST.down && ge->getCounterFlag("StartFade") != ge->ACTIVE) {
			se::Play("ok");
			ge->StartCounter("StartFade", 45); //フェードは90フレームなので半分の45で切り替え
			ge->CreateEffect(98, ML::Vec2(0, 0));
		}
		if (ge->getCounterFlag("StartFade") == ge->LIMIT) {
			this->Kill();
		}

		this->posTitle.y = easing::GetPos("titleYIn");
		this->flashCnt = (this->flashCnt + 1) % 60;

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
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