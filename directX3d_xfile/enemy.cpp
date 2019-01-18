//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#include "main.h"
#include "enemy.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "D3DXAnimation.h"
#include "debugproc.h"
#include "player.h"
#include "HitCheck.h"
#include "meshwall.h"
#include "particle.h"
#include "CapsuleMesh.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
ENEMY enemyWk;

// �L�����N�^�[�̃A�j���[�V�����ԍ�
const char* EnemyAnimNum[] =
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

enum EnemyStateNum
{
	Idle_E,
	Frontwalk_E,
	Backwalk_E,
	Rightstep_E,
	Leftstep_E,
	Guard_E,
	Damage_E,
	Down_E,
	Downpose_E,
	Getup_E,
	Punchi_E,
	Kick_E,
	Hadou_E,
	Shoryu_E,
	SPattack_E,
	Throw_E,
	Win_E
};


//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ModelIndex = 0;

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	enemyWk.pos = FIRST_ENEMY_POS;
	enemyWk.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	enemyWk.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	enemyWk.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	enemyWk.HP = FULL_HP;
	enemyWk.HPzan = enemyWk.HP;

	if (type == 0)
	{
		//�I�u�W�F�N�g�̏�����
		enemyWk.Animation = CreateAnimationObject();

		// xFile�̓ǂݍ���
		if (FAILED(Load_xFile(enemyWk.Animation, ENEMY_XFILE, "enemy")))
		{
			return E_FAIL;
		}

		// ���ړ�
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[Rightstep_E])))
		{
			return E_FAIL;
		}
		// ���ړ�
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[Leftstep_E])))
		{
			return E_FAIL;
		}
		// �_���[�W
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[Damage_E])))
		{
			return E_FAIL;
		}
		// �_�E��
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[Down_E])))
		{
			return E_FAIL;
		}
		// �_�E�����
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[Downpose_E])))
		{
			return E_FAIL;
		}
		// �N���オ��
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[Getup_E])))
		{
			return E_FAIL;
		}
		// �p���`
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[Punchi_E])))
		{
			return E_FAIL;
		}
		// �L�b�N
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[Kick_E])))
		{
			return E_FAIL;
		}
		// �g��
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[Hadou_E])))
		{
			return E_FAIL;
		}
		// ����
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[Shoryu_E])))
		{
			return E_FAIL;
		}
		// SP�Z
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[SPattack_E])))
		{
			return E_FAIL;
		}
		// ����
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[Throw_E])))
		{
			return E_FAIL;
		}

		// AnimationSet������������
		for (int i = 0; i < enemyWk.Animation->AnimSetNum; i++)
		{
			if (FAILED(enemyWk.Animation->InitAnimation(enemyWk.Animation, EnemyAnimNum[i], i)))
			{
				return E_FAIL;
			}
		}
		enemyWk.Animation->CurrentAnimID = Idle_E;
		
		// �A�j���[�V�����Ԃ̕⊮��ݒ�
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Idle_E, 0.5f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Frontwalk_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Backwalk_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Rightstep_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Leftstep_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Guard_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Damage_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Down_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Downpose_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Getup_E, 1.0f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Punchi_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Kick_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Hadou_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Shoryu_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, SPattack_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Throw_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Win_E, 0.1f);

		// �����蔻��J�v�Z���𐶐�
		if (FAILED(
			CreateCapsule(&enemyWk.HitCapsule, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), 60.0f, 15.0f, true)))
		{
			return E_FAIL;
		}

		// �e�̐���
		enemyWk.IdxShadow = CreateShadow(enemyWk.pos, SHADOW_SIZE_X, SHADOW_SIZE_Z);
		enemyWk.SizeShadow = 25.0f;
		enemyWk.ColShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	// �A�j���[�V�����������[�X
	enemyWk.Animation->UninitAnimation(enemyWk.Animation);

	// �J�v�Z���������[�X
	UninitCapsule(&enemyWk.HitCapsule);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	int *Phase = GetPhase();
	PLAYER *playerWk = GetPlayer();

#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("�G�l�~�[���W X:%f Y:%f Z:%f\n", enemyWk.pos.x, enemyWk.pos.y, enemyWk.pos.z);
	PrintDebugProc("�G�l�~�[�p�x X:%f Y:%f Z:%f\n", enemyWk.rot.x, enemyWk.rot.y, enemyWk.rot.z);
#endif

	// �ȒP����
	EasyInputEnemy();

	// �{�i����

	// �A�j���[�V�������X�V
	enemyWk.Animation->UpdateAnimation(enemyWk.Animation, TIME_PER_FRAME);

	// ������
	if (*Phase == PhaseFinish && enemyWk.HPzan > playerWk->HPzan && enemyWk.Animation->CurrentAnimID == Idle_E)
	{
		enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Win_E, 1.5f);
	}
	// �s�k��HP0�ɂȂ�����_�E��
	if (enemyWk.HPzan <= 0)
	{
		enemyWk.HPzan = 0;
		enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Down_E, 0.5f);
		SetPhase(PhaseFinish);
	}

	// ���W�ړ�
	MoveEnemy();

	// �e�̈ʒu�ݒ�
	SetPositionShadow(enemyWk.IdxShadow, D3DXVECTOR3(enemyWk.pos.x, 0.1f, enemyWk.pos.z));
	SetVertexShadow(enemyWk.IdxShadow, enemyWk.SizeShadow, enemyWk.SizeShadow);
	SetColorShadow(enemyWk.IdxShadow, enemyWk.ColShadow);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DMATERIAL9 matDef;
	MATRIX matrix = *GetMatrix();
	D3DXMATRIX CapsuleMatrix;
	CAMERA *cameraWk = GetCamera(0);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&matrix.world);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&matrix.scale, enemyWk.scl.x, enemyWk.scl.y, enemyWk.scl.z);
	D3DXMatrixMultiply(&matrix.world, &matrix.world, &matrix.scale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&matrix.scale, enemyWk.rot.y, enemyWk.rot.x, enemyWk.rot.z);
	D3DXMatrixMultiply(&matrix.world, &matrix.world, &matrix.scale);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&matrix.translation, enemyWk.pos.x, enemyWk.pos.y, enemyWk.pos.z);
	D3DXMatrixMultiply(&matrix.world, &matrix.world, &matrix.translation);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &matrix.world);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �����_�����O
	enemyWk.Animation->DrawAnimation(enemyWk.Animation, &matrix.world);

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

	// �G�l�~�[�̃J�v�Z����`�悷��
	CapsuleMatrix = GetBoneMatrix(enemyWk.Animation, "Hips");
	DrawCapsule(&enemyWk.HitCapsule, &CapsuleMatrix);
}

//=============================================================================
//�G�l�~�[�̏����擾����
//=============================================================================
ENEMY *GetEnemy(void)
{
	return &enemyWk;
}

//=============================================================================
//�ȒP���́i�A�j���[�V�����̊Ǘ����j
//=============================================================================
void EasyInputEnemy(void)
{
	CAMERA *camera = GetCamera(0);

	switch (enemyWk.Animation->CurrentAnimID)
	{
	case Idle_E:
		// �ړ�
		// �O
		if (GetKeyboardTrigger(DIK_A) || IsButtonTriggered(1, BUTTON_RIGHT) || IsButtonTriggered(1, STICK_RIGHT))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Frontwalk_E, ANIM_SPD_2);
		}
		// ���
		else if (GetKeyboardTrigger(DIK_D) || IsButtonTriggered(1, BUTTON_LEFT) || IsButtonTriggered(1, STICK_LEFT))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Backwalk_E, ANIM_SPD_2);
		}
		// ��O
		else if (GetKeyboardTrigger(DIK_S) || IsButtonTriggered(1, BUTTON_DOWN) || IsButtonTriggered(1, STICK_DOWN))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Rightstep_E, ANIM_SPD_2);
		}
		// ��
		else if (GetKeyboardTrigger(DIK_W) || IsButtonTriggered(1, BUTTON_UP) || IsButtonTriggered(1, STICK_UP))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Leftstep_E, ANIM_SPD_2);
		}

		// �U��
		// �p���`
		else if (GetKeyboardTrigger(DIK_O) || IsButtonTriggered(1, BUTTON_A))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Punchi_E, ANIM_SPD_15);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_K) || IsButtonTriggered(1, BUTTON_B))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Kick_E, ANIM_SPD_15);
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_L) || IsButtonTriggered(1, BUTTON_X))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Hadou_E, ANIM_SPD_2);
		}
		// ������
		else if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(1, BUTTON_C))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Shoryu_E, ANIM_SPD_15);
		}
		// �K�[�h
		else if (GetKeyboardTrigger(DIK_I) || IsButtonTriggered(1, BUTTON_Y))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Guard_E, ANIM_SPD_1);
		}
		else
		{
		}
		break;
	case Frontwalk_E:
		// �O
		if (GetKeyboardPress(DIK_A) || IsButtonPressed(1, BUTTON_RIGHT) || IsButtonPressed(1, STICK_RIGHT))
		{

		}
		// �{�^�������[�X�őҋ@�ɖ߂�
		else
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle_E, ANIM_SPD_1);
		}

		// �U��
		// �p���`
		if (GetKeyboardTrigger(DIK_O) || IsButtonTriggered(1, BUTTON_A))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Punchi_E, ANIM_SPD_15);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_K) || IsButtonTriggered(1, BUTTON_B))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Kick_E, ANIM_SPD_15);
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_L) || IsButtonTriggered(1, BUTTON_X))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Hadou_E, ANIM_SPD_2);
		}
		// ������
		else if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(1, BUTTON_C))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Shoryu_E, ANIM_SPD_15);
		}
		// �K�[�h
		else if (GetKeyboardTrigger(DIK_I) || IsButtonTriggered(1, BUTTON_Y))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Guard_E, ANIM_SPD_1);
		}
		break;
	case Backwalk_E:
		// ���
		if (GetKeyboardPress(DIK_D) || IsButtonPressed(1, BUTTON_LEFT) || IsButtonPressed(1, STICK_LEFT))
		{

		}
		// �{�^�������[�X�őҋ@�ɖ߂�
		else
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle_E, ANIM_SPD_1);
		}

		// �U��
		// �p���`
		if (GetKeyboardTrigger(DIK_O) || IsButtonTriggered(1, BUTTON_A))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Punchi_E, ANIM_SPD_15);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_K) || IsButtonTriggered(1, BUTTON_B))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Kick_E, ANIM_SPD_15);
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_L) || IsButtonTriggered(1, BUTTON_X))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Hadou_E, ANIM_SPD_2);
		}
		// ������
		else if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(1, BUTTON_C))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Shoryu_E, ANIM_SPD_15);
		}
		// �K�[�h
		else if (GetKeyboardTrigger(DIK_I) || IsButtonTriggered(1, BUTTON_Y))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Guard_E, ANIM_SPD_1);
		}
		break;
	case Rightstep_E:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle_E, ANIM_SPD_1);
		}
		break;
	case Leftstep_E:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle_E, ANIM_SPD_1);
		}
		break;
	case Guard_E:
		if (GetKeyboardPress(DIK_I) || IsButtonPressed(1, BUTTON_Y))
		{

		}
		// �{�^�������[�X�őҋ@�ɖ߂�
		else
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle_E, ANIM_SPD_1);
		}

		// �U��
		// �p���`
		if (GetKeyboardTrigger(DIK_O) || IsButtonTriggered(1, BUTTON_A))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Punchi_E, ANIM_SPD_15);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_K) || IsButtonTriggered(1, BUTTON_B))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Kick_E, ANIM_SPD_15);
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_L) || IsButtonTriggered(1, BUTTON_X))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Hadou_E, ANIM_SPD_2);
		}
		// ������
		else if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(1, BUTTON_C))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Shoryu_E, ANIM_SPD_15);
		}
	case Damage_E:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle_E, ANIM_SPD_1);
		}
		break;
	case Down_E:
		// �A�j���[�V�����I���ŋN���オ��Ɉڍs
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Downpose_E, ANIM_SPD_1);
			D3DXMATRIXA16 newmatrix;	// ���[�V�������W���擾���邽�߂̍s��
			newmatrix = GetBoneMatrix(enemyWk.Animation, "Hips");
			enemyWk.pos = D3DXVECTOR3(newmatrix._41, 0.0f, newmatrix._43);
		}
		break;
	case Downpose_E:
		// �A�j���[�V�����I���ŋN���オ��Ɉڍs
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Getup_E, ANIM_SPD_15);
		}
		break;
	case Getup_E:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle_E, ANIM_SPD_1);
		}
		break;
	case Punchi_E:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle_E, ANIM_SPD_1);
		}
		break;
	case Kick_E:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle_E, ANIM_SPD_1);
		}
		break;
	case Hadou_E:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle_E, ANIM_SPD_1);
		}
		break;
	case Shoryu_E:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle_E, ANIM_SPD_1);
		}
		break;
	default:
		break;
	}

}

//=============================================================================
//���W�ړ�
//=============================================================================
void MoveEnemy(void)
{
	CAMERA *camera = GetCamera(0);
	D3DXVECTOR3 centerpos = GetCenterPos();
	PLAYER *playerWk = GetPlayer();
	D3DXVECTOR3		oldPos = enemyWk.pos;		// ���̈ʒu
	float PEdistance = GetPEdistance();

	// �A�N�V�����ɍ��킹�����W�ړ�
	switch (enemyWk.Animation->CurrentAnimID)
	{
		// �O�ړ����̍��W����
	case Frontwalk_E:
		enemyWk.move.x -= sinf(enemyWk.rot.y) * VALUE_FRONTWALK;
		enemyWk.move.z -= cosf(enemyWk.rot.y) * VALUE_FRONTWALK;
		// ����ɐڐG���Ă����ꍇ�A���������
		if (PEdistance <= MIN_DISTANCE)
		{
			playerWk->move.x -= sinf(playerWk->rot.y + D3DX_PI) * VALUE_FRONTWALK;
			playerWk->move.z -= cosf(playerWk->rot.y + D3DX_PI) * VALUE_FRONTWALK;
		}
		break;
		// ��ړ����̍��W����
	case Backwalk_E:
		// �t�B�[���h�O�ɂ͏o���Ȃ�
		// ���肩���苗�����ꂽ�炻��ȏ㗣����Ȃ�
		if (PEdistance >= MAX_DISTANCE)
		{

		}
		else
		{
			enemyWk.move.x -= sinf(enemyWk.rot.y + D3DX_PI) * VALUE_BACKWALK;
			enemyWk.move.z -= cosf(enemyWk.rot.y + D3DX_PI) * VALUE_BACKWALK;
		}
		break;
		// ��O�ړ����̍��W����
	case Rightstep_E:
		enemyWk.move.x -= sinf(enemyWk.rot.y - D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		enemyWk.move.z -= cosf(enemyWk.rot.y - D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		break;
		// ���ړ����̍��W����
	case Leftstep_E:
		enemyWk.move.x -= sinf(enemyWk.rot.y + D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		enemyWk.move.z -= cosf(enemyWk.rot.y + D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		break;
	default:
		break;
	}

	// �U�����ȊO��ɒ��S������
	if (enemyWk.Animation->CurrentAnimID == Punchi_E || enemyWk.Animation->CurrentAnimID == Kick_E ||
		enemyWk.Animation->CurrentAnimID == Hadou_E || enemyWk.Animation->CurrentAnimID == Shoryu_E)
	{

	}
	else
	{
		enemyWk.rot.y = atan2f(centerpos.x - enemyWk.pos.x, centerpos.z - enemyWk.pos.z) + D3DX_PI;
	}

	// �ʒu�ړ�
	enemyWk.pos.x += enemyWk.move.x;
	enemyWk.pos.y += enemyWk.move.y;
	enemyWk.pos.z += enemyWk.move.z;

	// �ړ��ʂ����Z�b�g����
	enemyWk.move.x = 0;
	enemyWk.move.y = 0;
	enemyWk.move.z = 0;


}