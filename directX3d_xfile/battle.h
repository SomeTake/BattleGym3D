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
#include "input.h"

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
#define THROW_VALUE							(40.0f)						// �����̓�����͈�

#define INPUT_MAX							(60)						// �R�}���h���͌��m�p�z��̍ő吔
#define REC_TIME							(600)						// ���v���C�p�ɃL�[���͂�ۑ����Ă������ԁi10�b�ԁj
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �L�����N�^�[�̃f�[�^���Ǘ�����\����
typedef struct {
	D3DXVECTOR3			pos;				// ���f���̈ʒu
	D3DXVECTOR3			move;				// ���f���̈ړ���
	D3DXVECTOR3			rot;				// ���݂̌���
	D3DXVECTOR3			scl;				// ���f���̑傫��(�X�P�[��)
	int					HP;					// �̗�
	int					SP;					// SP�Q�[�W
	int					HPzan;				// �c��̗�
	D3DXANIMATION		*Animation;			// �A�j���[�V����
	BALL				HitBall[HIT_CHECK_NUM];	// �����蔻��p�{�[��
	bool				HitFrag;			// �U���������������ǂ���
	HADOU				HadouBullet;		// �g�����\����
	int					score;				// �X�R�A
	POP					Popup;				// 1Por2P�\���̃r���{�[�h
	int					framecount;			// �A�j���[�V�����̓r���œ����蔻��𔭐�������^�C�~���O�𑪂�J�E���^�i�A�j���[�V�������؂�ւ������0�ɖ߂��j
	int					damagecount;		// �_���[�W���񕜂���J�E���g

	// ���̓��[�h
	bool				CommandInput;		// �R�}���h���͂�ON�ɂ���t���O�itrue:CommandInput�Afalse:EasyInput�j
	// EasyInput
	int					gracetype;			// ���͗P�\�̎��
	int					graceframe;			// ���͗P�\�̎���
	bool				graceflag;			// ���͗P�\�̗L���t���O
	// CommandInput
	int					Input[INPUT_MAX];	// ���͏����p�z��

	int					ShadowIdx;			// �g�p����e�̔ԍ�

	// ���v���C�p
	D3DXVECTOR3			RecPos[REC_TIME];	// ���v���C���J�n����ꏊ��ۑ�����z��
	D3DXVECTOR3			ReplayPos;			// ���v���C���J�n����ꏊ
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
	AnimMax,			// �A�j���[�V�����̍ő吔
};

// �o�g���p�f�[�^�\����
typedef struct
{
	int Damage;					// ���̃��[�V�����ɂ���ė^����_���[�W�ʁiSP�Q�[�W�A�X�R�A�Ȃǂ��j
	float Spd;					// �A�j���[�V�������Đ�����X�s�[�h
	float ShiftTime;			// �A�j���[�V�����̐؂�ւ�����
	int CollisionStartTime;		// �U�������蔻��̔������鎞��
	int CollisionFinishTime;	// �U�������蔻��̏I�����鎞��
}BATTLEDATA;

// �o�g���p�f�[�^�\���̔z��
static BATTLEDATA Data[AnimMax] = {
	{0, 1.5f, 0.1f, 0, 0 },		// Idle
{ 0, 2.0f, 0.1f, 0, 0 },		// Frontwalk
{ 0, 2.0f, 0.1f, 0, 0 },		// Backwalk
{ 0, 2.0f, 0.1f, 0, 0 },		// Rightstep
{ 0, 2.0f, 0.1f, 0, 0 },		// Leftstep
{ 0, 1.0f, 0.1f, 0, 0 },		// Guard
{ 0, 1.5f, 0.1f, 0, 0 },		// Damage
{ 0, 1.5f, 0.1f, 0, 0 },		// Down
{ 0, 1.0f, 0.1f, 0, 0 },		// Downpose
{ 0, 1.5f, 0.1f, 0, 0 },		// Getup
{ 40, 2.5f, 0.1f, 10, 20 },		// Punchi
{ 50, 2.5f, 0.1f, 15, 30 },		// Kick
{ 100, 3.0f, 0.1f, 25, 0 },		// Hadou
{ 120, 2.0f, 0.1f, 5, 20 },		// Shoryu
{ 400, 1.5f, 0.1f, 30, 150 },		// SPattack
{ 150, 1.0f, 0.1f, 0, 0 },		// Throw
{ 0, 2.0f, 0.1f, 0, 0 },		// Win
{ 0, 1.5f, 0.1f, 0, 0 },		// Miss
{ 0, 1.0f, 0.1f, 0, 0 },		// Throwpose
};

// �R�}���h�\�i�D��x�������j
static const int
CMD_SPattack[] = { BUTTON_RIGHT, BUTTON_RIGHTDOWN, BUTTON_DOWN, BUTTON_LEFTDOWN, BUTTON_LEFT, BUTTON_A | BUTTON_B, INPUT_END | 20 },
CMD_Hadou[] = { BUTTON_DOWN, BUTTON_RIGHTDOWN, BUTTON_RIGHT, BUTTON_A, INPUT_END | 15 },
CMD_Hadou1[] = { BUTTON_DOWN, BUTTON_RIGHTDOWN, BUTTON_RIGHT | BUTTON_A, INPUT_END | 15 },
CMD_Shoryu[] = { BUTTON_RIGHT, BUTTON_DOWN, BUTTON_RIGHTDOWN, BUTTON_B, INPUT_END | 15 },
CMD_Shoryu1[] = { BUTTON_RIGHT, BUTTON_DOWN, BUTTON_RIGHTDOWN | BUTTON_B, INPUT_END | 15 },
CMD_Shoryu2[] = { BUTTON_RIGHTDOWN, BUTTON_DOWN, BUTTON_RIGHTDOWN, BUTTON_B, INPUT_END | 15 },
CMD_Shoryu3[] = { BUTTON_RIGHTDOWN, BUTTON_DOWN, BUTTON_RIGHTDOWN | BUTTON_B, INPUT_END | 15 },
CMD_Throw[] = { BUTTON_A | BUTTON_B, INPUT_END | 3 },
CMD_Punchi[] = { BUTTON_A, INPUT_END | 1 },
CMD_Kick[] = { BUTTON_B, INPUT_END | 1 },
CMD_Guard[] = { BUTTON_Y, INPUT_END | 1 },
CMD_Frontwalk[] = { BUTTON_RIGHT, INPUT_END | 1 },
CMD_Backwalk[] = { BUTTON_LEFT, INPUT_END | 1 },
CMD_Rightstep[] = { BUTTON_DOWN, BUTTON_DEFAULT, BUTTON_DOWN, INPUT_END | 15 },
CMD_Leftstep[] = { BUTTON_UP, BUTTON_DEFAULT, BUTTON_UP, INPUT_END | 15 };

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
	RightLeg,
};

// �����蔻��̔��a�i��̔����ꏊ�ƘA���j
static float HitRadius[] = 
{
	10.0f,
	10.0f,
	10.0f,
	5.0f,
	5.0f,
	5.0f,
	5.0f,
	7.0f,
	7.0f,
	5.0f,
	5.0f,
	7.0f,
	7.0f
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
bool HitBC(D3DXVECTOR3 AttackPos, D3DXVECTOR3 DefendPos, float AttackRange, float DefendRange);	// �o�E���f�B���O�T�[�N�������蔻��
bool HitCheckCToC(CHARA *AttackChara, CHARA *DefendChara);		// �L�����N�^�[���m�̓����蔻��
void HitAction(CHARA *AttackChara, CHARA *DefendChara);			// �U�������������Ƃ��̓���
void SubDamage(CHARA *Chara, int sub, bool AtkCharaInput);		// �_���[�W��^����
void AddSpGauge(CHARA *Chara, int add);							// SP�Q�[�W�̑���
void HitHadou(CHARA *AttackChara, CHARA *DefendChara);			// �g�����̓����蔻��
void AddScore(CHARA *Chara, int add);							// �X�R�A�̒ǉ�
void BattleAI(CHARA *AIChara, CHARA *AnotherChara);				// �o�g���pAI
void SetupTutorial(CHARA *Chara, REDGAUGE *Gauge);				// �`���[�g���A�����[�h�ł̃L�����N�^�[��HP�Ȃǂ̊Ǘ�

void EasyInput(CHARA *Chara, int ControllerNum);				// �ȒP����
void CommandInput(CHARA *Chara, int ControllerNum);				// �R�}���h����
bool CheckInput(int Input[], const int *command);				// ���͔���

#endif
