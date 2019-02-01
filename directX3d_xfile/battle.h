//=============================================================================
//
// �o�g�������i�v���C���[���G�l�~�[���Ăяo�������j [battle.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _BATTLE_H_
#define _BATTLE_H_

// �\���̂�v���g�^�C�v�錾�ɕK�v�ȃw�b�_�[
#include "D3DXAnimation.h"
#include "ball.h"
#include "hadou.h"
#include "pop.h"
#include "redgauge.h"
#include "effect.h"

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

#define MOVABLE_AREA	(385.0f)										// ��������t�B�[���h�̍L��

// �o�g���֌W
// ���̑����[�V�������Ƃ̏ڍ׃f�[�^�͉���Data�\���̔z��ŊǗ�
#define MAX_DISTANCE						(250.0f)					// �v���C���[�Ԃ̍ő勗���i����ȏ�͉�������Ȃ��j
#define MIN_DISTANCE						(30.0f)						// �v���C���[�Ԃ̍ŏ������i����ȏ�͋߂Â��Ȃ��j
#define FULL_HP								(1000)						// �Q�[�W�̍ő�l
#define FULL_SPGAUGE						(1000)						// �Q�[�W�̍ő�l
#define FULL_SCORE							(99999)						// �X�R�A�̍ő吔
#define FIRE_FRAME							(25)						// �g�����̔��˃^�C�~���O
#define THROW_FRAME							(180)						// �����A�j���[�V�����̓r���Ń_���[�W��^���違����̃A�j���[�V������ύX������^�C�~���O
#define GRACE_VALUE							(3)							// ���͗P�\�͈̔̓t���[����
#define HPHEAL								(10)						// �`���[�g���A�����[�h�ł�HP�̉񕜗�

// �����蔻��
#define BODY_RADIUS							(10.0f)						// �̂̓����蔻��̔��a
#define ARM_RADIUS							(5.0f)						// ��̓����蔻��̔��a
#define FOOT_RADIUS							(7.0f)						// ���̓����蔻��̔��a
#define THROW_VALUE							(40.0f)						// �����̓�����͈�

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
	POP					Popup;				// 1Por2P�\���̃r���{�[�h
	int					framecount;			// �A�j���[�V�����̓r���ŏ��������邽�߃^�C�~���O�𑪂�J�E���^
	int					gracetype;			// ���͗P�\�̎��
	int					graceframe;			// ���͗P�\�̎���
	bool				graceflag;			// ���͗P�\�̗L���t���O
	int					damagecount;		// �_���[�W���񕜂���J�E���g
	EFFECT				effect;				// �G�t�F�N�g�̃r���{�[�h
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
	"win",				// �K�b�c�|�[�Y�i�������j
	"miss",				// �����X�J��
	"throwedpose"		// �������Ă���Œ��̃|�[�Y
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
	Win,
	Miss,
	ThrowedPose,
	AnimMax,
};

// �o�g���p�f�[�^�\����
typedef struct
{
	int Damage;
	float Spd;
}BATTLEDATA;

// �o�g���p�f�[�^�\���̔z��
static BATTLEDATA Data[AnimMax] = {
	{0, 1.0f},	// Idle
{ 0, 2.0f },	// Frontwalk
{ 0, 2.0f },	// Backwalk
{ 0, 2.0f },	// Rightstep
{ 0, 2.0f },	// Leftstep
{ 0, 1.0f },	// Guard
{ 0, 1.0f },	// Damage
{ 0, 1.0f },	// Down
{ 0, 1.0f },	// Downpose
{ 0, 1.5f },	// Getup
{ 40, 1.5f },	// Punchi
{ 50, 1.5f },	// Kick
{ 100, 2.0f },	// Hadou
{ 120, 1.5f },	// Shoryu
{ 400, 1.5f },	// SPattack
{ 150, 1.0f },	// Throw
{ 0, 1.5f },	// Win
{ 0, 1.0f },	// Miss
{ 0, 1.0f },	// Throwpose
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
void SubDamage(CHARA *Chara, int sub);							// �_���[�W��^����
void AddSpGauge(CHARA *Chara, int add);							// SP�Q�[�W�̑���
void HitHadou(CHARA *AttackChara, CHARA *DefendChara);			// �g�����̓����蔻��
void EasyInput(CHARA *Chara, int ControllerNum);				// �ȒP����
void AddScore(CHARA *Chara, int add);							// �X�R�A�̒ǉ�
void BattleAI(CHARA *AIChara, CHARA *AnotherChara);				// �o�g���pAI
void SetupTutorial(CHARA *Chara, REDGAUGE *Gauge);				// �`���[�g���A�����[�h�ł̃L�����N�^�[��HP�Ȃǂ̊Ǘ�

#endif
