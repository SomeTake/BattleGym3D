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
#include "meshwall.h"
#include "particle.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CHARA playerWk;

D3DXMATRIX WorldMtxPlayer;

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
	playerWk.SP = 0;
	playerWk.HitFrag = false;

	if (type == 0)
	{
		//�I�u�W�F�N�g�̏�����
		playerWk.Animation = CreateAnimationObject();

		// xFile�̓ǂݍ���
		if (FAILED(Load_xFile(playerWk.Animation, CHARA_XFILE, "Player")))
		{
			return E_FAIL;
		}

		// AnimationCallback���Z�b�g����
		// �O����
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Frontwalk])))
		{
			return E_FAIL;
		}
		// ������
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Backwalk])))
		{
			return E_FAIL;
		}
		// ���ړ�
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Rightstep])))
		{
			return E_FAIL;
		}
		// ���ړ�
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Leftstep])))
		{
			return E_FAIL;
		}
		// �_���[�W
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Damage])))
		{
			return E_FAIL;
		}
		// �_�E��
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Down])))
		{
			return E_FAIL;
		}
		// �_�E���|�[�Y
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Downpose])))
		{
			return E_FAIL;
		}
		// �N���オ��
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Getup])))
		{
			return E_FAIL;
		}
		// �p���`
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Punchi])))
		{
			return E_FAIL;
		}
		// �L�b�N
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Kick])))
		{
			return E_FAIL;
		}
		// �g��
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Hadou])))
		{
			return E_FAIL;
		}
		// ����
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Shoryu])))
		{
			return E_FAIL;
		}
		// SP�Z
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[SPattack])))
		{
			return E_FAIL;
		}
		// ����
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Throw])))
		{
			return E_FAIL;
		}

		// AnimationSet������������
		for (int i = 0; i < playerWk.Animation->AnimSetNum; i++)
		{
			if (FAILED(playerWk.Animation->InitAnimation(playerWk.Animation, CharaStateAnim[i], i)))
			{
				return E_FAIL;
			}
		}
		playerWk.Animation->CurrentAnimID = Idle;

		// �A�j���[�V�������̕⊮��ݒ�
		playerWk.Animation->SetShiftTime(playerWk.Animation, Idle, 0.5f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Frontwalk, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Backwalk, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Rightstep, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Leftstep, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Guard, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Damage, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Down, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Downpose, 1.0f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Getup, 1.0f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Punchi, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Kick, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Hadou, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Shoryu, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, SPattack, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Throw, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Win, 0.1f);

		// �����蔻��p�{�[���𐶐�
		D3DXMATRIX Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[Hips]);
		InitBall(0, &playerWk.HitBall[0], Mtx, BODY_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[Neck]);
		InitBall(0, &playerWk.HitBall[1], Mtx, BODY_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[Head]);
		InitBall(0, &playerWk.HitBall[2], Mtx, BODY_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[LeftShoulder]);
		InitBall(0, &playerWk.HitBall[3], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[RightShoulder]);
		InitBall(0, &playerWk.HitBall[4], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[LeftHand]);
		InitBall(0, &playerWk.HitBall[5], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[RightHand]);
		InitBall(0, &playerWk.HitBall[6], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[LeftFoot]);
		InitBall(0, &playerWk.HitBall[7], Mtx, FOOT_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[RightFoot]);
		InitBall(0, &playerWk.HitBall[8], Mtx, FOOT_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[LeftForeArm]);
		InitBall(0, &playerWk.HitBall[9], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[RightForeArm]);
		InitBall(0, &playerWk.HitBall[10], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[LeftLeg]);
		InitBall(0, &playerWk.HitBall[11], Mtx, FOOT_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[RightLeg]);
		InitBall(0, &playerWk.HitBall[12], Mtx, FOOT_RADIUS);

		// �e�̐���
		playerWk.IdxShadow = CreateShadow(playerWk.pos, SHADOW_SIZE_X, SHADOW_SIZE_Z);
		playerWk.SizeShadow = 25.0f;
		playerWk.ColShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);	
	}
	else
	{
		playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
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
	
	// �����蔻��{�[���������[�X
	for (int i = 0; i < HIT_CHECK_NUM; i++)
	{
		UninitBall(&playerWk.HitBall[i]);
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	int *Phase = GetPhase();
	CHARA *enemyWk = GetEnemy();

#ifdef _DEBUG
	// �f�o�b�O�p����
	if (GetKeyboardTrigger(DIK_1))
	{
		playerWk.Animation->ChangeAnimation(playerWk.Animation, Down, 1.5f);
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
		playerWk.Animation->ChangeAnimation(playerWk.Animation, Damage, 1.5f);
	}

	// �f�o�b�O�\��
	PrintDebugProc("�v���C���[���W X:%f Y:%f Z:%f\n", playerWk.pos.x, playerWk.pos.y, playerWk.pos.z);
	PrintDebugProc("�v���C���[�p�x X:%f Y:%f Z:%f\n", playerWk.rot.x, playerWk.rot.y, playerWk.rot.z);
	PrintDebugProc("�v���C���[�A�j���[�V�����ԍ� X:%d\n", playerWk.Animation->CurrentAnimID);
	PrintDebugProc("���C���[�t���[���\�� �L�[�{�[�h:9 = OFF �L�[�{�[�h:0 = ON\n");
#endif

	// HP0&�_�E�����[�V�������I�������ꍇ�A����ȏ�A�j���[�V�������X�V���Ȃ��A����ł��Ȃ�
	if (playerWk.Animation->CurrentAnimID == Downpose && playerWk.HPzan == 0)
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
		if (*Phase == PhaseFinish && playerWk.HPzan > enemyWk->HPzan && playerWk.Animation->CurrentAnimID == Idle)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Win, 1.5f);
		}
		// �s�k��HP0�ɂȂ�����_�E��
		if (playerWk.HPzan <= 0)
		{
			playerWk.HPzan = 0;
			// �����I�ɃA�j���[�V�����ύX
			if (playerWk.Animation->CurrentAnimID != Downpose && playerWk.Animation->CurrentAnimID != Down)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Down, 0.5f);
			}
			else if (playerWk.Animation->CurrentAnimID == Down && playerWk.Animation->MotionEnd == true)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Down, ANIM_SPD_1);
			}
			SetPhase(PhaseFinish);
		}
	}

	// ���W�ړ�
	MovePlayer();
	
	// �����蔻��p�{�[�����W�̍X�V
	D3DXMATRIX Mtx;
	for (int i = 0; i < HIT_CHECK_NUM; i++)
	{
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[i]);
		UpdateBall(&playerWk.HitBall[i], Mtx);
	}

	// �����蔻��
	if (playerWk.HitFrag == false)
	{
		// �L�����N�^�[���m�̓����蔻��
		if (HitCheckCToC(&playerWk, enemyWk) == true)
		{
			// ����������̓���
			HitAction(&playerWk, enemyWk);
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
	D3DXMATRIX ScaleMatrix, RotMatrix, TransMatrix, CapsuleMatrix, BallMatrix;
	bool RenderState = GetRenderState();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&WorldMtxPlayer);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&ScaleMatrix, playerWk.scl.x, playerWk.scl.y, playerWk.scl.z);
	D3DXMatrixMultiply(&WorldMtxPlayer, &WorldMtxPlayer, &ScaleMatrix);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&ScaleMatrix, playerWk.rot.y, playerWk.rot.x, playerWk.rot.z);
	D3DXMatrixMultiply(&WorldMtxPlayer, &WorldMtxPlayer, &ScaleMatrix);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&TransMatrix, playerWk.pos.x, playerWk.pos.y, playerWk.pos.z);
	D3DXMatrixMultiply(&WorldMtxPlayer, &WorldMtxPlayer, &TransMatrix);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &WorldMtxPlayer);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �`�惂�[�h�����C���[�t���[���ɐ؂�ւ���
	if (RenderState == true)
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}

	// �����_�����O
	playerWk.Animation->DrawAnimation(playerWk.Animation, &WorldMtxPlayer);

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

	if (RenderState == true)
	{
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			// �v���C���[�̓����蔻��p�{�[����`�悷��
			DrawBall(&playerWk.HitBall[i]);
		}
	}
}

//=============================================================================
//�v���C���[�̏����擾����
//=============================================================================
CHARA *GetPlayer(void)
{
	return &playerWk;
}

//=============================================================================
//�ȒP���́i�A�j���[�V�����̊Ǘ����j
//=============================================================================
void EasyInputPlayer(void)
{
	switch (playerWk.Animation->CurrentAnimID)
	{
	case Idle:
		// �ړ�
		// �O
		if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_RIGHT) || IsButtonTriggered(0, STICK_RIGHT))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Frontwalk, ANIM_SPD_2);
		}
		// ���
		else if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_LEFT) || IsButtonTriggered(0, STICK_LEFT))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Backwalk, ANIM_SPD_2);
		}
		// ��O
		else if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN) || IsButtonTriggered(0, STICK_DOWN))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Rightstep, ANIM_SPD_2);
		}
		// ��
		else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, STICK_UP))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Leftstep, ANIM_SPD_2);
		}

		// �U��
		// �p���`
		else if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi, ANIM_SPD_15);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_M) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick, ANIM_SPD_15);
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_N) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Hadou, ANIM_SPD_2);
		}
		// ������
		else if (GetKeyboardTrigger(DIK_H) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Shoryu, ANIM_SPD_15);
		}
		// �K�[�h
		else if (GetKeyboardTrigger(DIK_G) || IsButtonTriggered(0, BUTTON_Y))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Guard, ANIM_SPD_1);
		}
		else
		{
		}
		break;
	case Frontwalk:
		// �O
		if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, STICK_RIGHT))
		{

		}
		// �{�^�������[�X�őҋ@�ɖ߂�
		else
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
		}

		// �U��
		// �p���`
		if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi, ANIM_SPD_15);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_M) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick, ANIM_SPD_15);
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_N) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Hadou, ANIM_SPD_2);
		}
		// ������
		else if (GetKeyboardTrigger(DIK_H) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Shoryu, ANIM_SPD_15);
		}
		// �K�[�h
		else if (GetKeyboardTrigger(DIK_G) || IsButtonTriggered(0, BUTTON_Y))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Guard, ANIM_SPD_1);
		}
		break;
	case Backwalk:
		// ���
		if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, STICK_LEFT))
		{

		}
		// �{�^�������[�X�őҋ@�ɖ߂�
		else
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
		}

		// �U��
		// �p���`
		if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi, ANIM_SPD_15);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_M) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick, ANIM_SPD_15);
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_N) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Hadou, ANIM_SPD_2);
		}
		// ������
		else if (GetKeyboardTrigger(DIK_H) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Shoryu, ANIM_SPD_15);
		}
		// �K�[�h
		else if (GetKeyboardTrigger(DIK_G) || IsButtonTriggered(0, BUTTON_Y))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Guard, ANIM_SPD_1);
		}
		break;
	case Rightstep:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
		}
		break;
	case Leftstep:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
		}
		break;
	case Guard:
		if (GetKeyboardPress(DIK_G) || IsButtonPressed(0, BUTTON_Y))
		{

		}
			// �{�^�������[�X�őҋ@�ɖ߂�
		else
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
		}

		// �U��
		// �p���`
		if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi, ANIM_SPD_15);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_M) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick, ANIM_SPD_15);
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_N) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Hadou, ANIM_SPD_2);
		}
		// ������
		else if (GetKeyboardTrigger(DIK_H) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Shoryu, ANIM_SPD_15);
		}
	case Damage:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
		}
		break;
	case Down:
		// �A�j���[�V�����I���ŋN���オ��Ɉڍs
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Downpose, ANIM_SPD_1);
			// �v���C���[���W�����[�V�������W�ɍ��킹��
			D3DXMATRIXA16 newmatrix;	// ���[�V�������W���擾���邽�߂̍s��
			newmatrix = GetBoneMatrix(playerWk.Animation, "Hips");
			playerWk.pos = D3DXVECTOR3(newmatrix._41, 0.0f, newmatrix._43);
		}
		break;
	case Downpose:
		// �A�j���[�V�����I���ŋN���オ��Ɉڍs
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Getup, ANIM_SPD_15);
		}
	case Getup:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
		}
		break;
	case Punchi:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
			playerWk.HitFrag = false;
		}
		break;
	case Kick:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
			playerWk.HitFrag = false;
		}
		break;
	case Hadou:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
			playerWk.HitFrag = false;
		}
		break;
	case Shoryu:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
			playerWk.HitFrag = false;
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
	D3DXVECTOR3 centerpos = GetCenterPos();
	CHARA *enemyWk = GetEnemy();
	float PEdistance = GetPEdistance();

	// �A�N�V�����ɍ��킹�����W�ړ�
	switch (playerWk.Animation->CurrentAnimID)
	{
		// �O�ړ����̍��W����
	case Frontwalk:
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
	case Backwalk:
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
	case Rightstep:
		playerWk.move.x -= sinf(playerWk.rot.y + D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		playerWk.move.z -= cosf(playerWk.rot.y + D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		break;
		// ���ړ����̍��W����
	case Leftstep:
		playerWk.move.x -= sinf(playerWk.rot.y - D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		playerWk.move.z -= cosf(playerWk.rot.y - D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		break;
	default:
		break;
	}

	// �U�����[�V�������ȊO�ɒ��S������
	if (playerWk.Animation->CurrentAnimID == Punchi || playerWk.Animation->CurrentAnimID == Kick 
		|| playerWk.Animation->CurrentAnimID == Hadou || playerWk.Animation->CurrentAnimID == Shoryu)
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

	// (���a*�p�x)�{����W�Ńv���C���[�̍��W���v�Z����

	// �ړ��ʂ����Z�b�g����
	playerWk.move.x = 0.0f;
	playerWk.move.y = 0.0f;
	playerWk.move.z = 0.0f;	

	// �ړ����̃G�t�F�N�g�̔���
	if (playerWk.Animation->CurrentAnimID == Frontwalk || playerWk.Animation->CurrentAnimID == Backwalk ||
		playerWk.Animation->CurrentAnimID == Rightstep || playerWk.Animation->CurrentAnimID == Leftstep)
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