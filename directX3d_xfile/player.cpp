//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "D3DXAnimation.h"
#include "enemy.h"
#include "debugproc.h"
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
PLAYER playerWk;

// �L�����N�^�[�̃A�j���[�V�����ԍ�
const char* PlayerAnimNum[] =
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

enum PlayerStateNum
{
	Idle_P,
	Frontwalk_P,
	Backwalk_P,
	Rightstep_P,
	Leftstep_P,
	Guard_P,
	Damage_P,
	Down_P,
	Downpose_P,
	Getup_P,
	Punchi_P,
	Kick_P,
	Hadou_P,
	Shoryu_P,
	SPattack_P,
	Throw_P,
	Win_P
};

bool RenderWireframe = false;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	playerWk.pos = FIRST_PLAYER_POS;
	playerWk.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	playerWk.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk.HP = FULL_HP;
	playerWk.HPzan = playerWk.HP;

	if (type == 0)
	{
		//�I�u�W�F�N�g�̏�����
		playerWk.Animation = CreateAnimationObject();

		// xFile�̓ǂݍ���
		if (FAILED(Load_xFile(playerWk.Animation, PLAYER_XFILE, "Player")))
		{
			return E_FAIL;
		}

		// AnimationCallback���Z�b�g����
		// �O����
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Frontwalk_P])))
		{
			return E_FAIL;
		}
		// ������
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Backwalk_P])))
		{
			return E_FAIL;
		}
		// ���ړ�
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Rightstep_P])))
		{
			return E_FAIL;
		}
		// ���ړ�
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Leftstep_P])))
		{
			return E_FAIL;
		}
		// �_���[�W
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Damage_P])))
		{
			return E_FAIL;
		}
		// �_�E��
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Down_P])))
		{
			return E_FAIL;
		}
		// �_�E���|�[�Y
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Downpose_P])))
		{
			return E_FAIL;
		}
		// �N���オ��
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Getup_P])))
		{
			return E_FAIL;
		}
		// �p���`
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Punchi_P])))
		{
			return E_FAIL;
		}
		// �L�b�N
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Kick_P])))
		{
			return E_FAIL;
		}
		// �g��
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Hadou_P])))
		{
			return E_FAIL;
		}
		// ����
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Shoryu_P])))
		{
			return E_FAIL;
		}
		// SP�Z
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[SPattack_P])))
		{
			return E_FAIL;
		}
		// ����
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Throw_P])))
		{
			return E_FAIL;
		}

		// AnimationSet������������
		for (int i = 0; i < playerWk.Animation->AnimSetNum; i++)
		{
			if (FAILED(playerWk.Animation->InitAnimation(playerWk.Animation, PlayerAnimNum[i], i)))
			{
				return E_FAIL;
			}
		}
		playerWk.Animation->CurrentAnimID = Idle_P;
		playerWk.Animation->SetShiftTime(playerWk.Animation, Idle_P, 0.5f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Frontwalk_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Backwalk_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Rightstep_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Leftstep_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Guard_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Damage_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Down_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Downpose_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Getup_P, 1.0f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Punchi_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Kick_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Hadou_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Shoryu_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, SPattack_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Throw_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Win_P, 0.1f);

		// �����蔻��J�v�Z���𐶐�
		if (FAILED(
			CreateCapsule(&playerWk.HitCapsule, D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), 60.0f, 15.0f, true)))
		{
			return E_FAIL;
		}

		// �e�̐���
		playerWk.IdxShadow = CreateShadow(playerWk.pos, SHADOW_SIZE_X, SHADOW_SIZE_Z);
		playerWk.SizeShadow = 25.0f;
		playerWk.ColShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);	
	}

	// �����A�j���[�V������ҋ@�ɂ���
	playerWk.Animation->CurrentAnimID = Idle_P;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	// �A�j���[�V�����������[�X
	playerWk.Animation->UninitAnimation(playerWk.Animation);
	
	// �J�v�Z���������[�X
	UninitCapsule(&playerWk.HitCapsule);

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	int *Phase = GetPhase();
	ENEMY *enemyWk = GetEnemy();

#ifdef _DEBUG
	// �f�o�b�O�p����
	if (GetKeyboardTrigger(DIK_1))
	{
		playerWk.Animation->ChangeAnimation(playerWk.Animation, Down_P, 1.5f);
	}
	// �v���C���[HP0
	else if (GetKeyboardTrigger(DIK_2))
	{
		playerWk.HPzan = 0;
	}
	// �G�l�~�[HP0
	else if (GetKeyboardTrigger(DIK_3))
	{
		enemyWk->HPzan = 0;
	}
	else if (GetKeyboardTrigger(DIK_4))
	{
		playerWk.Animation->ChangeAnimation(playerWk.Animation, Damage_P, 1.5f);
	}
	else if (GetKeyboardTrigger(DIK_9))
	{
		RenderWireframe = true;
	}
	else if (GetKeyboardTrigger(DIK_0))
	{
		RenderWireframe = false;
	}

	// �f�o�b�O�\��
	PrintDebugProc("�v���C���[���W X:%f Y:%f Z:%f\n", playerWk.pos.x, playerWk.pos.y, playerWk.pos.z);
	PrintDebugProc("�v���C���[�p�x X:%f Y:%f Z:%f\n", playerWk.rot.x, playerWk.rot.y, playerWk.rot.z);
	PrintDebugProc("�v���C���[�A�j���[�V�����ԍ� X:%d\n", playerWk.Animation->CurrentAnimID);
	PrintDebugProc("���C���[�t���[���\�� �L�[�{�[�h:9 = OFF �L�[�{�[�h:0 = ON\n");
#endif

	// HP0&�_�E�����[�V�������I�������ꍇ�A����ȏ�A�j���[�V�������X�V���Ȃ��A����ł��Ȃ�
	if (playerWk.Animation->CurrentAnimID == Downpose_P && playerWk.HPzan == 0)
	{

	}
	else
	{
		// �ȒP����
		EasyInputPlayer();

		// �{�i����

		// �A�j���[�V�������X�V
		playerWk.Animation->UpdateAnimation(playerWk.Animation, TIME_PER_FRAME);

		// ������
		if (*Phase == PhaseFinish && playerWk.HPzan > enemyWk->HPzan && playerWk.Animation->CurrentAnimID == Idle_P)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Win_P, 1.5f);
		}
		// �s�k��HP0�ɂȂ�����_�E��
		if (playerWk.HPzan <= 0)
		{
			playerWk.HPzan = 0;
			// �����I�ɃA�j���[�V�����ύX
			if (playerWk.Animation->CurrentAnimID != Downpose_P && playerWk.Animation->CurrentAnimID != Down_P)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Down_P, 0.5f);
			}
			else if (playerWk.Animation->CurrentAnimID == Down_P && playerWk.Animation->MotionEnd == true)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Down_P, ANIM_SPD_1);
			}
			SetPhase(PhaseFinish);
		}
	}

	// ���W�ړ�
	MovePlayer();

	// �U������
	HitCheckPlayer();

	// �e�̈ʒu�ݒ�
	SetPositionShadow(playerWk.IdxShadow, D3DXVECTOR3(playerWk.pos.x, 0.1f, playerWk.pos.z));
	SetVertexShadow(playerWk.IdxShadow, playerWk.SizeShadow, playerWk.SizeShadow);
	SetColorShadow(playerWk.IdxShadow, playerWk.ColShadow);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DMATERIAL9 matDef;
	D3DXMATRIX CapsuleMatrix;
	MATRIX matrix = *GetMatrix();
	CAMERA *cameraWk = GetCamera(0);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&matrix.world);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&matrix.scale, playerWk.scl.x, playerWk.scl.y, playerWk.scl.z);
	D3DXMatrixMultiply(&matrix.world, &matrix.world, &matrix.scale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&matrix.scale, playerWk.rot.y, playerWk.rot.x, playerWk.rot.z);
	D3DXMatrixMultiply(&matrix.world, &matrix.world, &matrix.scale);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&matrix.translation, playerWk.pos.x, playerWk.pos.y, playerWk.pos.z);
	D3DXMatrixMultiply(&matrix.world, &matrix.world, &matrix.translation);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &matrix.world);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	if (RenderWireframe == true)
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}

	// �����_�����O
	playerWk.Animation->DrawAnimation(playerWk.Animation, &matrix.world);

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

	// �v���C���[�̃J�v�Z����`�悷��
	CapsuleMatrix = GetBoneMatrix(playerWk.Animation, "Hips");
	DrawCapsule(&playerWk.HitCapsule, &CapsuleMatrix);
}

//=============================================================================
//�v���C���[�̏����擾����
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &playerWk;
}

//=============================================================================
//�ȒP���́i�A�j���[�V�����̊Ǘ����j
//=============================================================================
void EasyInputPlayer(void)
{
	CAMERA *camera = GetCamera(0);

	switch (playerWk.Animation->CurrentAnimID)
	{
	case Idle_P:
		// �ړ�
		// �O
		if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_RIGHT) || IsButtonTriggered(0, STICK_RIGHT))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Frontwalk_P, ANIM_SPD_2);
		}
		// ���
		else if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_LEFT) || IsButtonTriggered(0, STICK_LEFT))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Backwalk_P, ANIM_SPD_2);
		}
		// ��O
		else if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN) || IsButtonTriggered(0, STICK_DOWN))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Rightstep_P, ANIM_SPD_2);
		}
		// ��
		else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, STICK_UP))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Leftstep_P, ANIM_SPD_2);
		}

		// �U��
		// �p���`
		else if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi_P, ANIM_SPD_15);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_M) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick_P, ANIM_SPD_15);
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_N) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Hadou_P, ANIM_SPD_2);
		}
		// ������
		else if (GetKeyboardTrigger(DIK_H) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Shoryu_P, ANIM_SPD_15);
		}
		// �K�[�h
		else if (GetKeyboardTrigger(DIK_G) || IsButtonTriggered(0, BUTTON_Y))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Guard_P, ANIM_SPD_1);
		}
		else
		{
		}
		break;
	case Frontwalk_P:
		// �O
		if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, STICK_RIGHT))
		{

		}
		// �{�^�������[�X�őҋ@�ɖ߂�
		else
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, ANIM_SPD_1);
		}

		// �U��
		// �p���`
		if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi_P, ANIM_SPD_15);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_M) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick_P, ANIM_SPD_15);
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_N) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Hadou_P, ANIM_SPD_2);
		}
		// ������
		else if (GetKeyboardTrigger(DIK_H) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Shoryu_P, ANIM_SPD_15);
		}
		// �K�[�h
		else if (GetKeyboardTrigger(DIK_G) || IsButtonTriggered(0, BUTTON_Y))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Guard_P, ANIM_SPD_1);
		}
		break;
	case Backwalk_P:
		// ���
		if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, STICK_LEFT))
		{

		}
		// �{�^�������[�X�őҋ@�ɖ߂�
		else
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, ANIM_SPD_1);
		}

		// �U��
		// �p���`
		if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi_P, ANIM_SPD_15);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_M) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick_P, ANIM_SPD_15);
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_N) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Hadou_P, ANIM_SPD_2);
		}
		// ������
		else if (GetKeyboardTrigger(DIK_H) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Shoryu_P, ANIM_SPD_15);
		}
		// �K�[�h
		else if (GetKeyboardTrigger(DIK_G) || IsButtonTriggered(0, BUTTON_Y))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Guard_P, ANIM_SPD_1);
		}
		break;
	case Rightstep_P:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, ANIM_SPD_1);
		}
		break;
	case Leftstep_P:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, ANIM_SPD_1);
		}
		break;
	case Guard_P:
		if (GetKeyboardPress(DIK_G) || IsButtonPressed(0, BUTTON_Y))
		{

		}
			// �{�^�������[�X�őҋ@�ɖ߂�
		else
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, ANIM_SPD_1);
		}

		// �U��
		// �p���`
		if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi_P, ANIM_SPD_15);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_M) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick_P, ANIM_SPD_15);
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_N) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Hadou_P, ANIM_SPD_2);
		}
		// ������
		else if (GetKeyboardTrigger(DIK_H) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Shoryu_P, ANIM_SPD_15);
		}
	case Damage_P:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, ANIM_SPD_1);
		}
		break;
	case Down_P:
		// �A�j���[�V�����I���ŋN���オ��Ɉڍs
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Downpose_P, ANIM_SPD_1);
			D3DXMATRIXA16 newmatrix;	// ���[�V�������W���擾���邽�߂̍s��
			newmatrix = GetBoneMatrix(playerWk.Animation, "Hips");
			playerWk.pos = D3DXVECTOR3(newmatrix._41, 0.0f, newmatrix._43);
		}
		break;
	case Downpose_P:
		// �A�j���[�V�����I���ŋN���オ��Ɉڍs
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Getup_P, ANIM_SPD_15);
		}
	case Getup_P:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, ANIM_SPD_1);
		}
		break;
	case Punchi_P:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, ANIM_SPD_1);
		}
		break;
	case Kick_P:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, ANIM_SPD_1);
		}
		break;
	case Hadou_P:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, ANIM_SPD_1);
		}
		break;
	case Shoryu_P:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, ANIM_SPD_1);
		}
		break;
	default:
		break;
	}

}

//=============================================================================
//���W�ړ�
//=============================================================================
void MovePlayer(void)
{
	CAMERA *camera = GetCamera(0);
	D3DXVECTOR3 centerpos = GetCenterPos();
	ENEMY *enemyWk = GetEnemy();
	D3DXVECTOR3		oldPos = playerWk.pos;		// ���̈ʒu
	float PEdistance = GetPEdistance();

	//D3DXVECTOR3 newpos;

	// �A�N�V�����ɍ��킹�����W�ړ�
	switch (playerWk.Animation->CurrentAnimID)
	{
		// �O�ړ����̍��W����
	case Frontwalk_P:
		playerWk.move.x -= sinf(playerWk.rot.y) * VALUE_FRONTWALK;
		playerWk.move.z -= cosf(playerWk.rot.y) * VALUE_FRONTWALK;
		// ����ɐڐG���Ă����ꍇ�A���������
		if (PEdistance <= MIN_DISTANCE)
		{
			enemyWk->move.x -= sinf(enemyWk->rot.y + D3DX_PI) * VALUE_FRONTWALK;
			enemyWk->move.z -= cosf(enemyWk->rot.y + D3DX_PI) * VALUE_FRONTWALK;
		}
		break;
		// ��ړ����̍��W����
	case Backwalk_P:
		// �t�B�[���h�O�ɂ͏o���Ȃ�
		// ���肩���苗�����ꂽ�炻��ȏ㗣����Ȃ�
		if (PEdistance >= MAX_DISTANCE)
		{

		}
		else
		{
			playerWk.move.x -= sinf(playerWk.rot.y + D3DX_PI) * VALUE_BACKWALK;
			playerWk.move.z -= cosf(playerWk.rot.y + D3DX_PI) * VALUE_BACKWALK;
		}
		break;
		// ��O�ړ����̍��W����
	case Rightstep_P:
		playerWk.move.x -= sinf(playerWk.rot.y + D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		playerWk.move.z -= cosf(playerWk.rot.y + D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		break;
		// ���ړ����̍��W����
	case Leftstep_P:
		playerWk.move.x -= sinf(playerWk.rot.y - D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		playerWk.move.z -= cosf(playerWk.rot.y - D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		break;
	default:
		break;
	}

	//D3DXMATRIXA16 newmatrix;	// ���[�V�������W���擾���邽�߂̍s��
	//// �O�����A�������̃{�^���������[�X���ꂽ�ꍇ�A���[�V�����r���̍��W���v���C���[���W�Ƃ���
	//if (GetKeyboardRelease(DIK_RIGHT) || (GetKeyboardRelease(DIK_LEFT)))
	//{
	//	// �v���C���[���W�����[�V�������W�ɍ��킹��
	//	newmatrix = GetBoneMatrix(playerWk.Animation, "Hips");
	//	playerWk.pos = D3DXVECTOR3(newmatrix._41, 0.0f, newmatrix._43);
	//}
	//// ���[�V�����I�����Ƀv���C���[���W�����[�V�������W�ɍ��킹��
	//if (playerWk.Animation->MotionEnd == true)
	//{
	//	// �O�����A���������p������ꍇ�A�t���O��߂�
	//	if (playerWk.Animation->CurrentAnimID == Frontwalk_P || playerWk.Animation->CurrentAnimID == Backwalk_P)
	//	{
	//		playerWk.Animation->MotionEnd = false;
	//	}
	//	newmatrix = GetBoneMatrix(playerWk.Animation, "Hips");
	//	playerWk.pos = D3DXVECTOR3(newmatrix._41, 0.0f, newmatrix._43);
	//}

	// �U�����[�V�������ȊO�ɒ��S������
	if (playerWk.Animation->CurrentAnimID == Punchi_P || playerWk.Animation->CurrentAnimID == Kick_P 
		|| playerWk.Animation->CurrentAnimID == Hadou_P || playerWk.Animation->CurrentAnimID == Shoryu_P)
	{
	}
	else
	{
		playerWk.rot.y = atan2f(centerpos.x - playerWk.pos.x, centerpos.z - playerWk.pos.z) + D3DX_PI;
	}

	// �ʒu�ړ�
	playerWk.pos.x += playerWk.move.x;
	playerWk.pos.y += playerWk.move.y;
	playerWk.pos.z += playerWk.move.z;

	// �ړ��O�ƌ��݂̍��W�̒����𑪂�ړ����Ă���悤�Ȃ瓖���蔻����s��
	//D3DXVECTOR3		vec = playerWk.move - oldPos;
	//float			len = D3DXVec3Length(&vec);
	//if (len > 0.1f)
	//{	// �r���{�[�h�Ƃ̓����蔻��
	//	if (hitCheckMeshwall(oldPos, playerWk.move) != 0)
	//	{
	//		// �������Ă���̂Ō��̈ʒu�ɖ߂�
	//		playerWk.pos = oldPos;
	//	}
	//}

	// (���a*�p�x)�{����W�Ńv���C���[�̍��W���v�Z����

	// �ړ��ʂ����Z�b�g����
	playerWk.move.x = 0.0f;
	playerWk.move.y = 0.0f;
	playerWk.move.z = 0.0f;	

	// �ړ����̃G�t�F�N�g�̔���
	if (playerWk.Animation->CurrentAnimID == Frontwalk_P || playerWk.Animation->CurrentAnimID == Backwalk_P ||
		playerWk.Animation->CurrentAnimID == Rightstep_P || playerWk.Animation->CurrentAnimID == Leftstep_P)
	{
		D3DXVECTOR3 pos;

		pos.x = playerWk.pos.x + sinf(playerWk.rot.y) * 10.0f;
		pos.y = playerWk.pos.y + 2.0f;
		pos.z = playerWk.pos.z + cosf(playerWk.rot.y) * 10.0f;

		SetParticle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.85f, 0.05f, 0.65f, 0.50f), 14.0f, 14.0f, 360);
		SetParticle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.65f, 0.85f, 0.05f, 0.30f), 10.0f, 10.0f, 360);
		SetParticle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.45f, 0.45f, 0.05f, 0.15f), 5.0f, 5.0f, 360);
	}

}

//=============================================================================
//�����蔻��
//=============================================================================
void HitCheckPlayer(void)
{
	D3DXMATRIXA16 AtkMtx;									// �����蔻��𔭐�������{�[�����W���擾����s��
	D3DXVECTOR3 AtkPos1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 AtkPos2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����蔻��𔭐���������W

	switch (playerWk.Animation->CurrentAnimID)
	{
	case Punchi_P:
		AtkMtx = GetBoneMatrix(playerWk.Animation, "LeftHand");
		AtkPos1 = D3DXVECTOR3(AtkMtx._41, AtkMtx._42, AtkMtx._43);
		break;
	case Kick_P:
		AtkMtx = GetBoneMatrix(playerWk.Animation, "RightFoot");
		AtkPos1 = D3DXVECTOR3(AtkMtx._41, AtkMtx._42, AtkMtx._43);
		break;
	case Hadou_P:
		AtkMtx = GetBoneMatrix(playerWk.Animation, "RightHand");
		AtkPos1 = D3DXVECTOR3(AtkMtx._41, AtkMtx._42, AtkMtx._43);
		break;
	case Shoryu_P:
		AtkMtx = GetBoneMatrix(playerWk.Animation, "RightFoot");
		AtkPos1 = D3DXVECTOR3(AtkMtx._41, AtkMtx._42, AtkMtx._43);
		AtkMtx = GetBoneMatrix(playerWk.Animation, "LeftFoot");
		AtkPos2 = D3DXVECTOR3(AtkMtx._41, AtkMtx._42, AtkMtx._43);
		break;
	default:
		break;
	}

	// ������AtkPos�Ɣ��a�ƁA�G�l�~�[���̓����蔻��̍��W�Ɣ��a�������铖���蔻��̊֐������
}