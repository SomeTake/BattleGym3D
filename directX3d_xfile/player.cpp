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
#include "effect.h"
#include "HitCheck.h"
#include "meshwall.h"

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
		playerWk.Animation->SetShiftTime(playerWk.Animation, Guard_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Damage_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Down_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Getup_P, 1.0f);
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

	// �f�o�b�O�\��
	PrintDebugProc("�v���C���[���W X:%f Y:%f Z:%f\n", playerWk.pos.x, playerWk.pos.y, playerWk.pos.z);
	PrintDebugProc("�v���C���[�p�x X:%f Y:%f Z:%f\n", playerWk.rot.x, playerWk.rot.y, playerWk.rot.z);
	PrintDebugProc("�v���C���[�A�j���[�V�����ԍ� X:%d\n", playerWk.Animation->CurrentAnimID);
#endif

	// HP0&�_�E�����[�V�������I�������ꍇ�A����ȏ�A�j���[�V�������X�V���Ȃ��A����ł��Ȃ�
	if (playerWk.Animation->CurrentAnimID == Down_P && playerWk.HPzan == 0 && playerWk.Animation->MotionEnd == true)
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
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Down_P, 0.5f);
			SetPhase(PhaseFinish);
		}
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

	switch (playerWk.Animation->CurrentAnimID)
	{
	case Idle_P:
		// �ړ�
		// �O
		if (GetKeyboardTrigger(DIK_D) || IsButtonTriggered(0, BUTTON_RIGHT) || IsButtonTriggered(0, STICK_RIGHT))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Frontwalk_P, 2.0f);
		}
		// ���
		else if (GetKeyboardTrigger(DIK_A) || IsButtonTriggered(0, BUTTON_LEFT) || IsButtonTriggered(0, STICK_LEFT))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Backwalk_P, 2.0f);
		}
		// ��O
		else if (GetKeyboardTrigger(DIK_S) || IsButtonTriggered(0, BUTTON_DOWN) || IsButtonTriggered(0, STICK_DOWN))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Rightstep_P, 2.0f);
		}
		// ��
		else if (GetKeyboardTrigger(DIK_W) || IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, STICK_UP))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Leftstep_P, 2.0f);
		}

		// �U��
		// �p���`
		else if (GetKeyboardTrigger(DIK_U) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi_P, 1.5f);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick_P, 1.5f);
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_K) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Hadou_P, 2.0f);
		}
		// ������
		else if (GetKeyboardTrigger(DIK_I) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Shoryu_P, 1.5f);
		}
		// �K�[�h
		else if (GetKeyboardTrigger(DIK_O) || IsButtonTriggered(0, BUTTON_Y))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Guard_P, 1.0f);
		}
		else
		{
		}
		break;
	case Frontwalk_P:
		// �O
		if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, STICK_RIGHT))
		{

		}
		// �{�^�������[�X�őҋ@�ɖ߂�
		else
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, 1.0f);
		}

		// �U��
		// �p���`
		if (GetKeyboardTrigger(DIK_U) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi_P, 1.5f);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick_P, 1.5f);
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_K) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Hadou_P, 2.0f);
		}
		// ������
		else if (GetKeyboardTrigger(DIK_I) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Shoryu_P, 1.5f);
		}
		// �K�[�h
		else if (GetKeyboardTrigger(DIK_O) || IsButtonTriggered(0, BUTTON_Y))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Guard_P, 1.0f);
		}
		break;
	case Backwalk_P:
		// ���
		if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, STICK_LEFT))
		{

		}
		// �{�^�������[�X�őҋ@�ɖ߂�
		else
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, 1.0f);
		}

		// �U��
		// �p���`
		if (GetKeyboardTrigger(DIK_U) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi_P, 1.5f);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick_P, 1.5f);
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_K) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Hadou_P, 2.0f);
		}
		// ������
		else if (GetKeyboardTrigger(DIK_I) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Shoryu_P, 1.5f);
		}
		// �K�[�h
		else if (GetKeyboardTrigger(DIK_O) || IsButtonTriggered(0, BUTTON_Y))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Guard_P, 1.0f);
		}
		break;
	case Rightstep_P:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, 1.0f);
		}
		break;
	case Leftstep_P:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, 1.0f);
		}
		break;
	case Guard_P:
		if (GetKeyboardPress(DIK_O) || IsButtonPressed(0, BUTTON_Y))
		{

		}
			// �{�^�������[�X�őҋ@�ɖ߂�
		else
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, 1.0f);
		}

		// �U��
		// �p���`
		if (GetKeyboardTrigger(DIK_U) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi_P, 1.5f);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick_P, 1.5f);
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_K) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Hadou_P, 2.0f);
		}
		// ������
		else if (GetKeyboardTrigger(DIK_I) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Shoryu_P, 1.5f);
		}
	case Damage_P:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, 1.0f);
		}
		break;
	case Down_P:
		// �A�j���[�V�����I���ŋN���オ��Ɉڍs
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Getup_P, 1.5f);
		}
		break;
	case Getup_P:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, 1.0f);
		}
		break;
	case Punchi_P:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, 1.0f);
		}
		break;
	case Kick_P:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, 1.0f);
		}
		break;
	case Hadou_P:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, 1.0f);
		}
		break;
	case Shoryu_P:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, 1.0f);
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

	D3DXVECTOR3 newpos;

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
		//playerWk.move.x -= PEdistance * cosf(playerWk.rot.y + D3DXToRadian(1));
		//playerWk.move.z -= PEdistance * sinf(playerWk.rot.y + D3DXToRadian(1));
		//playerWk.move = newpos - playerWk.pos;
		playerWk.move.x -= sinf(playerWk.rot.y + D3DX_PI * 0.50f) * VALUE_ROTATE;
		playerWk.move.z -= cosf(playerWk.rot.y + D3DX_PI * 0.50f) * VALUE_ROTATE;
		break;
		// ���ړ����̍��W����
	case Leftstep_P:
		//playerWk.move.x -= PEdistance * cosf(playerWk.rot.y + D3DXToRadian(-1));
		//playerWk.move.z -= PEdistance * sinf(playerWk.rot.y + D3DXToRadian(-1));
		//playerWk.move = newpos - playerWk.pos;
		playerWk.move.x -= sinf(playerWk.rot.y - D3DX_PI * 0.5f) * VALUE_ROTATE;
		playerWk.move.z -= cosf(playerWk.rot.y - D3DX_PI * 0.5f) * VALUE_ROTATE;
		break;
	default:
		break;
	}

	// ��ɒ��S������
	playerWk.rot.y = atan2f(centerpos.x - playerWk.pos.x, centerpos.z - playerWk.pos.z) + D3DX_PI;

	/// �ʒu�ړ�
	playerWk.pos.x += playerWk.move.x;
	playerWk.pos.y += playerWk.move.y;
	playerWk.pos.z += playerWk.move.z;

	// �ړ��O�ƌ��݂̍��W�̒����𑪂�ړ����Ă���悤�Ȃ瓖���蔻����s��
	D3DXVECTOR3		vec = playerWk.move - oldPos;
	float			len = D3DXVec3Length(&vec);
	if (len > 0.1f)
	{	// �r���{�[�h�Ƃ̓����蔻��
		//if (hitCheckMeshwall(oldPos, playerWk.move) != 0)
		//{
		//	// �������Ă���̂Ō��̈ʒu�ɖ߂�
		//	playerWk.pos = oldPos;
		//}
	}
	// (���a*�p�x)�{����W


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

		SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.85f, 0.05f, 0.65f, 0.50f), 14.0f, 14.0f, 20);
		SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.65f, 0.85f, 0.05f, 0.30f), 10.0f, 10.0f, 20);
		SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.45f, 0.45f, 0.05f, 0.15f), 5.0f, 5.0f, 20);
	}

}