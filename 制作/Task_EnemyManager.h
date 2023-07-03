#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

namespace  EnemyManager
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�Ǘ�");	//�O���[�v��
	const  string  defName("�G");	//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()	override;
		bool  Finalize()	override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//���L����ϐ��͂����ɒǉ�����
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//�ύX�s����������������������������������������������������
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
		void  UpDate()			override;//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		int appearCnt;							//�o���J�E���^
		vector<BResource::SP> residentResource;	//���\�[�X�풓�z��

		bool isAppearBoss;						//�{�X���o���������H
		void AppearEnemy(int enemyNum_);		//�G�o���֐�

		enum class Mode {
			AppearNormalEnemy,	// �ʏ�G�o��
			CoolTimeUpToBoss,	// �{�X�o���܂ł̃N�[���^�C��
			BossAction,			// �{�X�s��
			EnemyAnnihilation,	// �G�S��
		} mode;

		using ModeMap = std::map<Mode, std::function<void()>>;
		ModeMap modeMap;

		void ModeMap_Initialize();
		void ModeMap_Finalize();
		void ModeMap_UpDate(Mode mode_);

		void AppearNormalEnemy_UpDate();
		void CoolTimeUpToBoss_UpDate();
		void BossAction_UpDate();
		void EnemyAnnihilation_UpDate();

	};
}