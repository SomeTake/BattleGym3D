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
	"rolling",			// ���ړ��@�ł񂮂�Ԃ�
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
	Rolling_P,
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

bool upflag = false;
bool downflag = false;
int flagframecount = 0;

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
		// ��]
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Rolling_P])))
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
		playerWk.Animation->SetShiftTime(playerWk.Animation, Rolling_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Jump_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Guard_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Damage_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Down_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, DownPose_P, 0.1f);
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
	CAMERA *camera = GetCamera(0);
	D3DXVECTOR3 centerpos = GetCenterPos();

	// �A�j���[�V�������X�V
	playerWk.Animation->UpdateAnimation(playerWk.Animation, TIME_PER_FRAME);

	// ���݂̃A�j���[�V�����Ǘ�
	switch (playerWk.Action)
	{
	case Idle_P:
		if (GetKeyboardTrigger(DIK_0))
		{
			playerWk.NextAction = Punchi_P;
		}
		break;
	case Punchi_P:
		if (playerWk.NextAction == Punchi_P)
		{
			playerWk.NextAction = Idle_P;
		}
		break;
	default:
		break;
	}

	// ���̃A�j���[�V�����Ǘ�
	switch (playerWk.NextAction)
	{
	case Idle_P:
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, 0.1f);
			playerWk.Action = Idle_P;
		}
		break;
	case Punchi_P:
		if (playerWk.Action == Idle_P)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi_P, 0.1f);
			playerWk.Action = Punchi_P;
		}
		else
		{
			if (playerWk.Animation->MotionEnd == true)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi_P, 0.1f);
				playerWk.Action = Punchi_P;
			}
		}
	default:
		break;
	}
	//���f���̈ړ�
	// D�F�E( �� )
	if (GetKeyboardPress(DIK_D))
	{
		playerWk.move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * VALUE_MOVE;
		playerWk.move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * VALUE_MOVE;
		playerWk.rotDest.y = camera->rot.y + D3DX_PI * 0.50f;
	}
	// A�F��( �� )
	else if (GetKeyboardPress(DIK_A))
	{
		playerWk.move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * VALUE_MOVE;
		playerWk.move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * VALUE_MOVE;
		playerWk.rotDest.y = camera->rot.y - D3DX_PI * 0.50f;
	}
	// S�F��( �� )
	else if (GetKeyboardTrigger(DIK_S))
	{
		downflag = true;
	}
	// W�F��( �� )
	else if (GetKeyboardTrigger(DIK_W))
	{
		upflag = true;
	}

	// ��ړ����̍��W����
	if (upflag == true)
	{
		playerWk.move.x -= sinf(camera->rot.y) * VALUE_ROTATE;
		playerWk.move.z -= cosf(camera->rot.y) * VALUE_ROTATE;
		playerWk.rotDest.y = camera->rot.y;
		flagframecount++;
		if (flagframecount == 10)
		{
			flagframecount = 0;
			upflag = false;
		}
	}

	// ���ړ����̍��W����
	if (downflag == true)
	{
		playerWk.move.x -= sinf(D3DX_PI + camera->rot.y) * VALUE_ROTATE;
		playerWk.move.z -= cosf(D3DX_PI + camera->rot.y) * VALUE_ROTATE;
		playerWk.rotDest.y = D3DX_PI + camera->rot.y;
		flagframecount++;
		if (flagframecount == 10)
		{
			flagframecount = 0;
			downflag = false;
		}
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