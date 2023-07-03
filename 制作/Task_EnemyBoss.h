#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "BChara.h"

namespace  EnemyBoss
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�G");	//�O���[�v��
	const  string  defName("�{�X");		//�^�X�N��
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
	//�ύX������������������������������������������������������
		//���L����ϐ��͂����ɒǉ�����
		DG::Image::SP img;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:

		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate()		override;	//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()	override;	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	public:
	//�ύX������������������������������������������������������
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//BChara�Ɋ܂܂�Ȃ����m�݂̂����ɒǉ�����
		enum class MoveMode {
			Appearance,	//�o��
			Pattern0,	//�p�^�[��0
			Pattern1,	//�p�^�[��1
			Pattern2,	//�p�^�[��2
			Pattern3,	//�p�^�[��3

		};

		MoveMode mode;

		ML::Vec2 tgVec;//�ڕW�̍��W

		void Received(BChara* from_, AttackInfo at_) override;

		void AppearEnemyShot(ML::Vec2 pos, ML::Vec2 Vec);

		//���[�h����
		using ModeMap = std::map<MoveMode, std::function<void()>>;
		ModeMap modeMap;

		void MoveMap_Initialize();
		void MoveMap_Finalize();
		void MoveMap_UpDate(MoveMode mode_);

		//�e���[�h�̏���
		void Appearance_UpDate();
		void Pattern0_UpDate();
		void Pattern1_UpDate();
		void Pattern2_UpDate();
		void Pattern3_UpDate();

	};
}