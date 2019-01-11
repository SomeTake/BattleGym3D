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
	"jump",				// �W�����v
	"guard",			// �K�[�h �߂����ዹ����
	"damage",			// �_���[�W�󂯂�
	"down",				// �_�E��
	"downpose",			// �_�E�������܂܁i�s�k���j
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
	Jump_P,
	Guard_P,
	Damage_P,
	Down_P,
	DownPose_P,
	Getup_P,
	Punchi_P,
	Kick_P,
	Hadou_P,
	Shoryu_P,
	SPattack_P,
	Throw_P,
	Win_P
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	playerWk.pos = D3DXVECTOR3(50.0f, 0.0f, 0.0f);
	playerWk.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	playerWk.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk.jump = false;
	playerWk.speed = JUMP_SPEED;
	playerWk.HP = FULL_HP;
	playerWk.HPzan = playerWk.HP;
	playerWk.Action = Idle_P;
	playerWk.NextAction = Idle_P;

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
		// �W�����v
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Jump_P])))
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
		playerWk.Animation->SetShiftTime(playerWk.Animation, Frontwalk_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Backwalk_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Rightstep_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Leftstep_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Jump_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Guard_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Damage_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Down_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, DownPose_P, 0.0f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Getup_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Punchi_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Kick_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Hadou_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Shoryu_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, SPattack_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Throw_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Win_P, 0.1f);

		// �e�̐���
		playerWk.IdxShadow = CreateShadow(playerWk.pos, SHADOW_SIZE_X, SHADOW_SIZE_Z);
		playerWk.SizeShadow = 25.0f;
		playerWk.ColShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);	
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	// �A�j���[�V�����������[�X
	playerWk.Animation->UninitAnimation(playerWk.Animation);

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
		playerWk.NextAction = Down_P;
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

	// �f�o�b�O�\��
	PrintDebugProc("�v���C���[���W X:%f Y:%f Z:%f\n", playerWk.pos.x, playerWk.pos.y, playerWk.pos.z);
	PrintDebugProc("�v���C���[�p�x X:%f Y:%f Z:%f\n", playerWk.rot.x, playerWk.rot.y, playerWk.rot.z);
#endif

	// �A�j���[�V�������X�V
	playerWk.Animation->UpdateAnimation(playerWk.Animation, TIME_PER_FRAME);

	// �ȒP����
	EasyInputPlayer();

	// �{�i����


	// ������
	if (*Phase == PhaseFinish && playerWk.HPzan > enemyWk->HPzan && playerWk.Action == Idle_P)
	{
		playerWk.NextAction = Win_P;
	}
	// �s�k��HP0�ɂȂ�����_�E��
	if (playerWk.HPzan <= 0)
	{
		playerWk.HPzan = 0;
		playerWk.NextAction = Down_P;
		SetPhase(PhaseFinish);
	}

	// ���W�ړ�
	MovePlayer();

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

	// �����_�����O
	playerWk.Animation->DrawAnimation(playerWk.Animation, &matrix.world);

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
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

	// ���݂̃A�j���[�V�����Ǘ�
	switch (playerWk.Action)
	{
	case Idle_P:
		// ���͏���
		// D�F�E( �� )
		if (GetKeyboardTrigger(DIK_D) || IsButtonTriggered(0, BUTTON_RIGHT) || IsButtonTriggered(0, STICK_RIGHT))
		{
			playerWk.NextAction = Frontwalk_P;
		}
		// A�F��( �� )
		else if (GetKeyboardTrigger(DIK_A) || IsButtonTriggered(0, BUTTON_LEFT) || IsButtonTriggered(0, STICK_LEFT))
		{
			playerWk.NextAction = Backwalk_P;
		}
		// S�F��( �� )
		else if (GetKeyboardTrigger(DIK_S) || IsButtonTriggered(0, BUTTON_DOWN) || IsButtonTriggered(0, STICK_DOWN))
		{
			playerWk.NextAction = Rightstep_P;
		}
		// W�F��( �� )
		else if (GetKeyboardTrigger(DIK_W) || IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, STICK_UP))
		{
			playerWk.NextAction = Leftstep_P;
		}

		// �U������
		// �p���`
		if (GetKeyboardTrigger(DIK_U) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.NextAction = Punchi_P;
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.NextAction = Kick_P;
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_K) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.NextAction = Hadou_P;
		}
		// ������
		else if (GetKeyboardTrigger(DIK_I) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.NextAction = Shoryu_P;
		}

		// �K�[�h����
		if (GetKeyboardTrigger(DIK_O) || IsButtonTriggered(0, BUTTON_Y))
		{
			playerWk.NextAction = Guard_P;
		}
		break;
	case Frontwalk_P:
		// �U������
		// �p���`
		if (GetKeyboardTrigger(DIK_U) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.NextAction = Punchi_P;
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.NextAction = Kick_P;
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_K) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.NextAction = Hadou_P;
		}
		// ������
		else if (GetKeyboardTrigger(DIK_I) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.NextAction = Shoryu_P;
		}
		// �K�[�h����
		else if (GetKeyboardTrigger(DIK_O) || IsButtonTriggered(0, BUTTON_Y))
		{
			playerWk.NextAction = Guard_P;
		}
		// ���f���̈ړ�
		else if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, STICK_RIGHT))
		{
			// �����Ă����Frontwalk���ێ������
			// ���W�ړ���MovePlayer��
		}
		// �����[�X���ꂽ�ꍇ�ҋ@��Ԃɖ߂�
		else
		{
			playerWk.NextAction = Idle_P;
		}
		break;
	case Backwalk_P:
		// �U������
		// �p���`
		if (GetKeyboardTrigger(DIK_U) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.NextAction = Punchi_P;
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.NextAction = Kick_P;
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_K) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.NextAction = Hadou_P;
		}
		// ������
		else if (GetKeyboardTrigger(DIK_I) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.NextAction = Shoryu_P;
		}
		// �K�[�h����
		else if (GetKeyboardTrigger(DIK_O) || IsButtonTriggered(0, BUTTON_Y))
		{
			playerWk.NextAction = Guard_P;
		}
		// ���f���̈ړ�
		else if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, STICK_LEFT))
		{
			// �����Ă����Backwalk���ێ������
			// ���W�ړ���MovePlayer��
		}
		// �����[�X���ꂽ�ꍇ�ҋ@��Ԃɖ߂�
		else
		{
			playerWk.NextAction = Idle_P;
		}
		break;
	case Rightstep_P:
		// �A�j���[�V�������I������̂őҋ@��Ԃɖ߂�
		if (playerWk.NextAction == Rightstep_P)
		{
			playerWk.NextAction = Idle_P;
		}
		break;
	case Leftstep_P:
		// �A�j���[�V�������I������̂őҋ@��Ԃɖ߂�
		if (playerWk.NextAction == Leftstep_P)
		{
			playerWk.NextAction = Idle_P;
		}
		break;
	case Guard_P:
		// �K�[�h����
		if (GetKeyboardPress(DIK_O) || IsButtonPressed(0, BUTTON_Y))
		{
			playerWk.NextAction = Guard_P;
		}
		// �����[�X���ꂽ�ꍇ�ҋ@��Ԃɖ߂�
		else
		{
			playerWk.NextAction = Idle_P;
		}
		break;
	case Damage_P:
		// �A�j���[�V�������I������̂őҋ@��Ԃɖ߂�
		if (playerWk.NextAction == Damage_P)
		{
			playerWk.NextAction = Idle_P;
		}
		break;
	case Down_P:
		// �A�j���[�V�������I������̂Ń_�E����Ԃ̃|�[�Y�Ɉڍs
		if (playerWk.NextAction == Down_P)
		{
			playerWk.NextAction = DownPose_P;
		}
		break;
	case DownPose_P:
		// �A�j���[�V�������I������̂�HP���܂�����΋N���オ��Ɉڍs
		if (playerWk.NextAction == DownPose_P && playerWk.HPzan > 0)
		{
			playerWk.NextAction = Getup_P;
		}
		break;
	case Getup_P:
		// �A�j���[�V�������I������̂őҋ@��Ԃɖ߂�
		if (playerWk.NextAction == Getup_P)
		{
			playerWk.NextAction = Idle_P;
		}
		break;
	case Punchi_P:
		// �A�j���[�V�������I������̂őҋ@��Ԃɖ߂�
		if (playerWk.NextAction == Punchi_P)
		{
			playerWk.NextAction = Idle_P;
		}
		break;
	case Kick_P:
		// �A�j���[�V�������I������̂őҋ@��Ԃɖ߂�
		if (playerWk.NextAction == Kick_P)
		{
			playerWk.NextAction = Idle_P;
		}
		break;
	case Hadou_P:
		// �A�j���[�V�������I������̂őҋ@��Ԃɖ߂�
		if (playerWk.NextAction == Hadou_P)
		{
			playerWk.NextAction = Idle_P;
		}
		break;
	case Shoryu_P:
		// �A�j���[�V�������I������̂őҋ@��Ԃɖ߂�
		if (playerWk.NextAction == Shoryu_P)
		{
			playerWk.NextAction = Idle_P;
		}
		break;
	case Win_P:
		// ������ԂȂ̂ł����Ƃ��̃A�N�V���������Â���
		break;
	default:
		break;
	}

	// ���̃A�j���[�V�����Ǘ�
	switch (playerWk.NextAction)
	{
	case Idle_P:
		// ���A�N�V��������{�^�������[�X�őҋ@��Ԃɖ߂�
		if (playerWk.Action == Idle_P || playerWk.Action == Frontwalk_P || playerWk.Action == Backwalk_P || playerWk.Action == Guard_P)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, 0.1f);
			playerWk.Action = Idle_P;
		}
		else
		{
			if (playerWk.Animation->MotionEnd == true)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, 0.1f);
				playerWk.Action = Idle_P;
			}
		}
		break;

	case Frontwalk_P:
		playerWk.Animation->ChangeAnimation(playerWk.Animation, Frontwalk_P, 0.1f);
		playerWk.Action = Frontwalk_P;
		break;
	case Backwalk_P:
		playerWk.Animation->ChangeAnimation(playerWk.Animation, Backwalk_P, 0.1f);
		playerWk.Action = Backwalk_P;
		break;
	case Rightstep_P:
		// �A�j���[�V�������ڍs������
		if (playerWk.Action != Rightstep_P)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Rightstep_P, 0.1f);
			playerWk.Action = Rightstep_P;
		}
		else
		{
			// �A�j���[�V�������I��������ҋ@���[�V�����ɖ߂�
			if (playerWk.Animation->MotionEnd == true)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Rightstep_P, 0.1f);
				playerWk.Action = Rightstep_P;
			}
		}
		break;
	case Leftstep_P:
		// �A�j���[�V�������ڍs������
		if (playerWk.Action != Leftstep_P)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Leftstep_P, 0.1f);
			playerWk.Action = Leftstep_P;
		}
		else
		{
			// �A�j���[�V�������I��������ҋ@���[�V�����ɖ߂�
			if (playerWk.Animation->MotionEnd == true)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Leftstep_P, 0.1f);
				playerWk.Action = Leftstep_P;
			}
		}
		break;
	case Guard_P:
		playerWk.Animation->ChangeAnimation(playerWk.Animation, Guard_P, 0.1f);
		playerWk.Action = Guard_P;
		break;
	case Damage_P:
		// �A�j���[�V�������ڍs������
		if (playerWk.Action != Damage_P)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Damage_P, 0.1f);
			playerWk.Action = Damage_P;
		}
		else
		{
			// �A�j���[�V�������I��������ҋ@���[�V�����ɖ߂�
			if (playerWk.Animation->MotionEnd == true)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Damage_P, 0.1f);
				playerWk.Action = Damage_P;
			}
		}
		break;
	case Down_P:
		// �A�j���[�V�������ڍs������
		if (playerWk.Action != Down_P)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Down_P, 0.1f);
			playerWk.Action = Down_P;
		}
		else
		{
			// �A�j���[�V�������I�������玟�̃A�j���[�V�����Ɉڍs
			if (playerWk.Animation->MotionEnd == true)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Down_P, 0.0f);
				playerWk.Action = Down_P;
			}
		}
		break;
	case DownPose_P:
		// �A�j���[�V�������ڍs������
		if (playerWk.Action != DownPose_P)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, DownPose_P, 0.1f);
			playerWk.Action = DownPose_P;
		}
		else
		{
			// �A�j���[�V�������I�������玟�̃A�j���[�V�����Ɉڍs
			if (playerWk.Animation->MotionEnd == true)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, DownPose_P, 0.1f);
				playerWk.Action = DownPose_P;
			}
		}
		break;
	case Getup_P:
		// �A�j���[�V�������ڍs������
		if (playerWk.Action != Getup_P)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Getup_P, 0.1f);
			playerWk.Action = Getup_P;
		}
		else
		{
			// �A�j���[�V�������I�������玟�̃A�j���[�V�����Ɉڍs
			if (playerWk.Animation->MotionEnd == true)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Getup_P, 0.1f);
				playerWk.Action = Getup_P;
			}
		}
		break;
	case Punchi_P:
		// �A�j���[�V�������ڍs������
		if (playerWk.Action != Punchi_P)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi_P, 0.1f);
			playerWk.Action = Punchi_P;
		}
		else
		{
			// �A�j���[�V�������I��������ҋ@���[�V�����ɖ߂�
			if (playerWk.Animation->MotionEnd == true)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi_P, 0.1f);
				playerWk.Action = Punchi_P;
			}
		}
		break;
	case Kick_P:
		// �A�j���[�V�������ڍs������
		if (playerWk.Action != Kick_P)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick_P, 0.1f);
			playerWk.Action = Kick_P;
		}
		else
		{
			// �A�j���[�V�������I��������ҋ@���[�V�����ɖ߂�
			if (playerWk.Animation->MotionEnd == true)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick_P, 0.1f);
				playerWk.Action = Kick_P;
			}
		}
		break;
	case Hadou_P:
		// �A�j���[�V�������ڍs������
		if (playerWk.Action != Hadou_P)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Hadou_P, 0.1f);
			playerWk.Action = Hadou_P;
		}
		else
		{
			// �A�j���[�V�������I��������ҋ@���[�V�����ɖ߂�
			if (playerWk.Animation->MotionEnd == true)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Hadou_P, 0.1f);
				playerWk.Action = Hadou_P;
			}
		}
		break;
	case Shoryu_P:
		// �A�j���[�V�������ڍs������
		if (playerWk.Action != Shoryu_P)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Shoryu_P, 0.1f);
			playerWk.Action = Shoryu_P;
		}
		else
		{
			// �A�j���[�V�������I��������ҋ@���[�V�����ɖ߂�
			if (playerWk.Animation->MotionEnd == true)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Shoryu_P, 0.1f);
				playerWk.Action = Shoryu_P;
			}
		}
		break;
	case Win_P:
		// �A�j���[�V�������ڍs������
		if (playerWk.Action != Win_P)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Win_P, 0.1f);
			playerWk.Action = Win_P;
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

	// �E�ړ����̍��W����
	if (playerWk.Action == Frontwalk_P)
	{
		playerWk.move.x -= sinf(camera->rot.y) * VALUE_FRONTWALK;
		playerWk.move.z -= cosf(camera->rot.y) * VALUE_FRONTWALK;
		playerWk.rotDest.y = camera->rot.y;
	}
	// ���ړ����̍��W����
	if (playerWk.Action == Backwalk_P)
	{
		playerWk.move.x -= sinf(camera->rot.y + D3DX_PI) * VALUE_BACKWALK;
		playerWk.move.z -= cosf(camera->rot.y + D3DX_PI) * VALUE_BACKWALK;
		playerWk.rotDest.y = camera->rot.y + D3DX_PI;
	}
	// ��ړ����̍��W����
	if (playerWk.Action == Rightstep_P)
	{
		playerWk.move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * VALUE_ROTATE;
		playerWk.move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * VALUE_ROTATE;
		playerWk.rotDest.y = camera->rot.y - D3DX_PI * 0.50f;
	}
	// ���ړ����̍��W����
	if (playerWk.Action == Leftstep_P)
	{
		playerWk.move.x -= sinf(camera->rot.y + D3DX_PI * 0.5f) * VALUE_ROTATE;
		playerWk.move.z -= cosf(camera->rot.y + D3DX_PI * 0.5f) * VALUE_ROTATE;
		playerWk.rotDest.y = camera->rot.y + D3DX_PI * 0.5f;
	}

	// ��ɒ��S������
	playerWk.rot.y = atan2f(centerpos.x - playerWk.pos.x, centerpos.z - playerWk.pos.z) + D3DX_PI;

	/// �ʒu�ړ�
	playerWk.pos.x += playerWk.move.x;
	playerWk.pos.y += playerWk.move.y;
	playerWk.pos.z += playerWk.move.z;

	// �ړ��ʂɊ�����������
	playerWk.move.x += (0.0f - playerWk.move.x) * RATE_MOVE_MODEL;
	playerWk.move.y += (0.0f - playerWk.move.y) * RATE_MOVE_MODEL;
	playerWk.move.z += (0.0f - playerWk.move.z) * RATE_MOVE_MODEL;

	//���f���̃W�����v
	if (GetKeyboardTrigger(DIK_SPACE) && playerWk.pos.y == 0.0f)
	{
		playerWk.jump = true;
	}
	if (playerWk.jump == true)
	{
		playerWk.pos.y += playerWk.speed;
		playerWk.speed -= 0.98f;
		if (playerWk.pos.y <= 0.0f)
		{
			playerWk.speed = JUMP_SPEED;
			playerWk.pos.y = 0.0f;
			playerWk.jump = false;
		}
	}
}