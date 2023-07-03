#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "BChara.h"

namespace  EnemyBoss
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("敵");	//グループ名
	const  string  defName("ボス");		//タスク名
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()		override;
		bool  Finalize()		override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		//共有する変数はここに追加する
		DG::Image::SP img;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:

		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()		override;	//「実行」１フレーム毎に行う処理
		void  Render2D_AF()	override;	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		//追加したい変数・メソッドはここに追加する
		//BCharaに含まれないモノのみここに追加する
		enum class MoveMode {
			Appearance,	//登場
			Pattern0,	//パターン0
			Pattern1,	//パターン1
			Pattern2,	//パターン2
			Pattern3,	//パターン3

		};

		MoveMode mode;

		ML::Vec2 tgVec;//目標の座標

		void Received(BChara* from_, AttackInfo at_) override;

		void AppearEnemyShot(ML::Vec2 pos, ML::Vec2 Vec);

		//モード処理
		using ModeMap = std::map<MoveMode, std::function<void()>>;
		ModeMap modeMap;

		void MoveMap_Initialize();
		void MoveMap_Finalize();
		void MoveMap_UpDate(MoveMode mode_);

		//各モードの処理
		void Appearance_UpDate();
		void Pattern0_UpDate();
		void Pattern1_UpDate();
		void Pattern2_UpDate();
		void Pattern3_UpDate();

	};
}