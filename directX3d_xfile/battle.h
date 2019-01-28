//=============================================================================
//
// �o�g�������i�v���C���[���G�l�~�[���Ăяo�������j [battle.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _BATTLE_H_
#define _BATTLE_H_

// �\���̂ɕK�v�ȃw�b�_�[
#include "D3DXAnimation.h"
#include "ball.h"
#include "hadou.h"
#include "pop.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	CHARA_XFILE		"data/MODEL/Boy.x"				// �ǂݍ��ރ��f����(�t�@�C���p�X��)

#define	VALUE_MOVE		(1.00f)											// �ړ���
#define VALUE_FRONTWALK	(1.50f)											// �O�����ړ���
#define VALUE_BACKWALK	(1.00f)											// �������ړ���
#define	VALUE_ROTATE	(D3DX_PI * 0.80f)								// ��]��
#define VALUE_ENLARGEMENT	(0.1f)										// �g��E�k����
#define	VALUE_ROTATE_MODEL	(D3DX_PI * 0.05f)							// ��]���x
#define VALUE_HALF		(0.5f)											// ���炩�̒l�𔼕��ɂ��邽�߂Ɋ|����

#define	RATE_MOVE_MODEL		(0.20f)										// �ړ������W��
#define	RATE_ROTATE_MODEL	(0.20f)										// ��]�����W��

// �o�g���֌W
#define MAX_DISTANCE						(250.0f)					// �v���C���[�Ԃ̍ő勗���i����ȏ�͉�������Ȃ��j
#define MIN_DISTANCE						(30.0f)						// �v���C���[�Ԃ̍ŏ������i����ȏ�͋߂Â��Ȃ��j
#define FULL_HP								(1000)						// �Q�[�W�̍ő�l
#define FULL_SPGUAGE						(1000)						// �Q�[�W�̍ő�l
#define FULL_SCORE							(99999)						// �X�R�A�̍ő吔
#define DAMAGE_PUNCHI						(40)						// �_���[�W��
#define DAMAGE_KICK							(50)						// �_���[�W��
#define DAMAGE_HADOU						(100)						// �_���[�W��
#define DAMAGE_SHORYU						(120)						// �_���[�W��
#define DAMAGE_SPATTACK						(400)						// �_���[�W��
#define DAMAGE_THROW						(150)						// �_���[�W��
#define FIRE_FRAME							(25)						// �g�����̔��˃^�C�~���O

// �����蔻��
#define BODY_RADIUS							(10.0f)						// �̂̓����蔻��̔��a
#define ARM_RADIUS							(5.0f)						// ��̓����蔻��̔��a
#define FOOT_RADIUS							(7.0f)						// ���̓����蔻��̔��a

// �A�j���[�V�����X�s�[�h
#define ANIM_SPD_05							(0.5f)
#define ANIM_SPD_1							(1.0f)
#define ANIM_SPD_15							(1.5f)
#define ANIM_SPD_2							(2.0f)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �L�����N�^�[�̃f�[�^���Ǘ�����\����
typedef struct {
	D3DXVECTOR3			pos;				// ���f���̈ʒu
	D3DXVECTOR3			move;				// ���f���̈ړ���
	D3DXVECTOR3			rot;				// ���݂̌���
	D3DXVECTOR3			scl;				// ���f���̑傫��(�X�P�[��)
	int					IdxShadow;			// �eID
	float				SizeShadow;			// �e�̃T�C�Y
	D3DXCOLOR			ColShadow;			// �e�̐F
	int					HP;					// �̗�
	int					SP;					// SP�Q�[�W
	int					HPzan;				// �c��̗�
	D3DXANIMATION		*Animation;			// �A�j���[�V����
	BALL				HitBall[HIT_CHECK_NUM];	// �����蔻��p�{�[��
	bool				HitFrag;			// �U���������������ǂ���
	HADOU				HadouBullet;		// �g�����\����
	int					score;				// �X�R�A
	POP					Popup;				// 1Por1P�\���̃r���{�[�h
}CHARA;

// �L�����N�^�[�̃A�j���[�V�����ԍ�
static const char* CharaStateAnim[] =
{
	"idle",				// �ҋ@
	"frontwalk",		// �O����
	"backwalk",			// ������
	"rightstep",		// ���ړ�
	"leftstep",			// ���ړ�
	"guard",			// �K�[�h �߂����ዹ����
	"damage",			// �_���[�W�󂯂�
	"down",				// �_�E��
	"downpose",			// �_�E�����
	"getup",			// �N���オ��
	"punchi",			// �p���`
	"kick",				// �L�b�N
	"hadou",			// �g�����B�₽�甭�����x��
	"shoryu",			// �������B�o�b�N�t���b�v
	"SPattack",			// SP�Z�B�߂������]����
	"throw",			// �����B�͂�ŕG����ăA�b�p�[
	"win"				// �K�b�c�|�[�Y�i�������j
};

// �L�����N�^�[�̃A�j���[�V�����ԍ��ƘA���iCharaStateAnim�j
enum CharaStateNum
{
	Idle,
	Frontwalk,
	Backwalk,
	Rightstep,
	Leftstep,
	Guard,
	Damage,
	Down,
	Downpose,
	Getup,
	Punchi,
	Kick,
	Hadou,
	Shoryu,
	SPattack,
	Throw,
	Win
};

// �����蔻��𔭐�������ꏊ
static const char* CharaHitPos[] =
{
	"Hips",				// �K
	"Neck",				// ��
	"Head",				// ��
	"LeftShoulder",		// ����
	"RightShoulder",	// �E��
	"LeftHand",			// ����
	"RightHand",		// �E��
	"LeftFoot",			// ����
	"RightFoot",		// �E��
	"LeftForeArm",		// ���I
	"RightForeArm",		// �E�I
	"LeftLeg",			// ���G
	"RightLeg"			// �E�G
};

// �����蔻�蔭���ꏊ�ƘA���iCharaHitPos�j
enum CharaHitNum
{
	Hips,
	Neck,
	Head,
	LeftShoulder,
	RightShoulder,
	LeftHand,
	RightHand,
	LeftFoot,
	RightFoot,
	LeftForeArm,
	RightForeArm,
	LeftLeg,
	RightLeg
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
bool HitBC(D3DXVECTOR3 AttackPos, D3DXVECTOR3 DefendPos, float AttackRange, float DefendRange);	// �o�E���f�B���O�T�[�N�������蔻��
bool HitCheckCToC(CHARA *AttackChara, CHARA *DefendChara);		// �L�����N�^�[���m�̓����蔻��
void HitAction(CHARA *AttackChara, CHARA *DefendChara);			// �U�������������Ƃ��̓���
void SubDamage(CHARA *Chara, int add);							// �_���[�W��^����
void AddSpGauge(CHARA *Chara, int add);							// SP�Q�[�W�̑���
void HitHadou(CHARA *AttackChara, CHARA *DefendChara);			// �g�����̓����蔻��
void EasyInput(CHARA *Chara, int ControllerNum);				// �ȒP����
void AddScore(CHARA *Chara, int add);							// �X�R�A�̒ǉ�

#endif
