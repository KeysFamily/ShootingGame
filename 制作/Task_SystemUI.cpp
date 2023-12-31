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
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		this->imgBack = DG::Image::Create("./data/image/BG0.png");
		this->imgChar = DG::Image::Create("./data/image/font_number.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		this->imgBack.reset();
		this->imgChar.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//uϊ»v^XNΆ¬ΙPρΎ―s€
	bool  Object::Initialize()
	{
		//X[p[NXϊ»
		__super::Initialize(defGroupName, defName, true);
		//\[XNXΆ¬or\[X€L
		this->res = Resource::Create();

		//f[^ϊ»
		this->render2D_Priority[1] = 0.2f;
		this->startCnt = 3 * 60;
		//^XNΜΆ¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ψ«p¬^XNΜΆ¬
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
	void  Object::UpDate()
	{
		//X^[g
		if (ge->GameStartFlag) {
			--this->startCnt;
			if (startCnt < 0) { ge->GameStartFlag = false; }
		}
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		//wi\¦
		ML::Box2D drawBG(960, 0, 480, 720);
		ML::Box2D srcBG(0, 0, 240, 360);
		this->res->imgBack->Draw(drawBG, srcBG);

		//JEg_E
		if (ge->GameStartFlag) {
			ML::Box2D drawScore = Assist::BoxCentering(ge->screenWidth * 3 / 8, ge->screenHeight / 2, 80, 112)/*(960, 400, 40, 56)*/;
			ML::Box2D srcScore(0, 34, 20, 28);
			srcScore.Offset((this->startCnt / 60 + 1) * srcScore.w, 0);
			this->res->imgChar->Draw(drawScore, srcScore);
		}

		//cθΜΝ\¦
		ML::Box2D draw(960, 100, 125, 28);
		ML::Box2D src(0, 100, 125, 28);
		this->res->imgChar->Draw(draw, src);

		if (ge->qa_Player != nullptr) {
			this->DrawChar(3, ge->qa_Player->hp, ML::Box2D(960, 130, 40, 56));
		}

		//XRA\¦
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

	//
	//ΘΊΝξ{IΙΟXsvΘ\bh
	//
	//-------------------------------------------------------------------
	//^XNΆ¬ϋ
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWΙo^
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYΙΈs΅½ηKill
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
	//\[XNXΜΆ¬
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