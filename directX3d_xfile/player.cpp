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
#include "D3DXAnimation.h"
#include "enemy.h"
#include "debugproc.h"
#include "meshwall.h"
#include "particle.h"
#include "knockout.h"
#include "redgauge.h"
#include "sound.h"
#include "shadow.h"
#include "game.h"

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
	
	// �X�e�[�^�X���̏����ݒ�
	playerWk.HP = FULL_HP;
	playerWk.HPzan = playerWk.HP;
	playerWk.SP = 0;
	playerWk.HitFrag = false;
	playerWk.score = 0;
	playerWk.graceflag = false;
	playerWk.graceframe = 0;
	playerWk.gracetype = Idle;
	playerWk.damagecount = 0;

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
		// �����X�J��
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Miss])))
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

		// �A�j���[�V�����Ԃ̕⊮��ݒ�
		for (int i = 0; i < AnimMax; i++)
		{
			playerWk.Animation->SetShiftTime(playerWk.Animation, i, Data[i].ShiftTime);
		}

		// �����蔻��p�{�[���𐶐�
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			D3DXMATRIX Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[i]);
			InitBall(0, &playerWk.HitBall[i], Mtx, HitRadius[i]);
		}

	}
	else
	{
		playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, Data[Idle].Spd);

		// �O�̉e���폜
		DeleteShadow(playerWk.ShadowIdx);
	}

	// �g�����p�o���b�g���Z�b�g
	InitHadou(type, &playerWk.HadouBullet);

	// 1P�\���̃r���{�[�h���쐬
	InitPop(type, &playerWk.Popup, 0);

	// �e�̃Z�b�g
	playerWk.ShadowIdx = SetShadow(playerWk.pos, SHADOW_SIZE_X, SHADOW_SIZE_Z);

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

	// �g�����p�o���b�g�������[�X
	UninitHadou(&playerWk.HadouBullet);

	// 1P�\���������[�X
	UninitPop(&playerWk.Popup);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	int *Phase = GetPhase();
	CHARA *enemyWk = GetEnemy();
	bool ko = GetKnockout()->pushok;

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
	// ����SP�Q�[�WMAX
	else if (GetKeyboardTrigger(DIK_4))
	{
		AddSpGauge(&playerWk, FULL_SPGAUGE);
		AddSpGauge(enemyWk, FULL_SPGAUGE);
	}
	// �����̗�MAX
	else if (GetKeyboardTrigger(DIK_5))
	{
		SubDamage(&playerWk, -FULL_HP, false);
		SubDamage(enemyWk, -FULL_HP, false);
	}

	// ���̓��[�h�̐ؑ�
	if (GetKeyboardTrigger(DIK_9))
	{
		playerWk.CommandInput = playerWk.CommandInput ? false : true;
	}

	int Num = NumParticle();

	// �f�o�b�O�\��
	PrintDebugProc("�v���C���[���W X:%f Y:%f Z:%f\n", playerWk.pos.x, playerWk.pos.y, playerWk.pos.z);
	PrintDebugProc("�v���C���[�p�x X:%f Y:%f Z:%f\n", playerWk.rot.x, playerWk.rot.y, playerWk.rot.z);
	PrintDebugProc("�v���C���[�A�j���[�V�����ԍ� X:%d\n", playerWk.Animation->CurrentAnimID);
	PrintDebugProc("���C���[�t���[���\�� �L�[�{�[�h:0 = ON or OFF\n");
	PrintDebugProc("�v���C���[���͗P�\�t���O %s\n", playerWk.graceflag ? "ON" : "OFF");
	PrintDebugProc("�g�p���Ă���p�[�e�B�N���̐� %d\n", Num);
	PrintDebugProc("���݂̓��̓��[�h Player:%s �؂�ւ�9�L�[\n", playerWk.CommandInput ? "CommandInput" : "EasyInput");
#endif
	// �`���[�g���A���p
	if (*Phase == PhaseTutorial)
	{
		REDGAUGE *RedWk = GetRedGauge();
		SetupTutorial(&playerWk, RedWk);
	}

	if (*Phase != PhaseCountdown && playerWk.HPzan > 0)
	{
		// ���͐ؑ�
		if (playerWk.CommandInput == false)
		{
			EasyInput(&playerWk, 0);
		}
		else if (playerWk.CommandInput == true)
		{
			CommandInput(&playerWk, 0);
		}
	}

	// KO�\����or�q�b�g�X�g�b�v���͍X�V���Ȃ�
	if ((*Phase == PhaseFinish && ko == false) || GetHitStop() == true)
	{

	}
	else
	{
		// �A�j���[�V�������X�V
		playerWk.Animation->UpdateAnimation(playerWk.Animation, TIME_PER_FRAME);
	}

	// ������
	if (*Phase == PhaseFinish && playerWk.HPzan > enemyWk->HPzan && playerWk.Animation->CurrentAnimID == Idle)
	{
		playerWk.Animation->ChangeAnimation(playerWk.Animation, Win, Data[Win].Spd);
	}

	// KO�{�C�X
	if ((*Phase == PhaseGame || *Phase == PhaseTraining) && playerWk.HPzan == 0)
	{
		PlaySound(SE_KO);
	}

	// �s�k��HP0�ɂȂ�����_�E��
	if (playerWk.HPzan <= 0 && *Phase != PhaseTutorial)
	{
		playerWk.HPzan = 0;
		// �����I�ɃA�j���[�V�����ύX
		if (playerWk.Animation->CurrentAnimID != Downpose)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Downpose, Data[Downpose].Spd);
		}
		StopSound(BGM_BATTLE);
		StopSound(BGM_TRAINING);
		if (*Phase != PhaseFinish)
		{
			SetPhase(PhaseFinish);
		}
	}

	// KO�\����or�q�b�g�X�g�b�v���͍X�V���Ȃ�
	if ((*Phase == PhaseFinish && ko == false) || GetHitStop() == true)
	{

	}
	else
	{
		// ���W�ړ�
		MovePlayer();
	}

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

	if (playerWk.HadouBullet.use == true)
	{
		// �g�����p�o���b�g�̍X�V
		UpdateHadou(&playerWk.HadouBullet);

		// �g�����̓����蔻��
		HitHadou(&playerWk, enemyWk);
	}

	// 1P�\���̈ʒu�X�V
	UpdatePop(&playerWk.Popup, playerWk.HitBall[Hips].pos);

	// �e�̍X�V
	SetPositionShadow(playerWk.ShadowIdx, D3DXVECTOR3(playerWk.HitBall[Hips].pos.x, 0.1f, playerWk.HitBall[Hips].pos.z));
	SetVertexShadow(playerWk.ShadowIdx, SHADOW_SIZE_X, SHADOW_SIZE_Z);
	SetColorShadow(playerWk.ShadowIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

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

	if (playerWk.HadouBullet.use == true)
	{
		// �g�����p�o���b�g�̕`��
		//DrawHadou(&playerWk.HadouBullet);
	}

	// 1P�\���p�r���{�[�h��`��
	DrawPop(&playerWk.Popup);
}

//=============================================================================
//�v���C���[�̏����擾����
//=============================================================================
CHARA *GetPlayer(void)
{
	return &playerWk;
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
	case Shoryu:
		playerWk.move.x -= sinf(playerWk.rot.y) * VALUE_FRONTWALK;
		playerWk.move.z -= cosf(playerWk.rot.y) * VALUE_FRONTWALK;
		// ����ɐڐG���Ă����ꍇ�A���������
		if (PEdistance <= MIN_DISTANCE)
		{
			enemyWk->move.x -= sinf(enemyWk->rot.y + D3DX_PI) * VALUE_FRONTWALK;
			enemyWk->move.z -= cosf(enemyWk->rot.y + D3DX_PI) * VALUE_FRONTWALK;
		}
		break;
	default:
		break;
	}

	// �U����&�_�E�����ȊO�ɒ��S������
	if (playerWk.Animation->CurrentAnimID == Punchi || playerWk.Animation->CurrentAnimID == Kick 
		|| playerWk.Animation->CurrentAnimID == Hadou || playerWk.Animation->CurrentAnimID == Shoryu
		|| playerWk.Animation->CurrentAnimID == Down || playerWk.Animation->CurrentAnimID == Downpose)
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

	// ��ʊO����
	if (playerWk.pos.x > MOVABLE_AREA)
	{
		playerWk.pos.x = MOVABLE_AREA;
	}
	else if (playerWk.pos.x < -MOVABLE_AREA)
	{
		playerWk.pos.x = -MOVABLE_AREA;
	}
	if (playerWk.pos.z > MOVABLE_AREA)
	{
		playerWk.pos.z = MOVABLE_AREA;
	}
	else if (playerWk.pos.z < -MOVABLE_AREA)
	{
		playerWk.pos.z = -MOVABLE_AREA;
	}

	// �ړ����̃G�t�F�N�g�̔���
	if (playerWk.Animation->CurrentAnimID == Frontwalk || playerWk.Animation->CurrentAnimID == Backwalk ||
		playerWk.Animation->CurrentAnimID == Rightstep || playerWk.Animation->CurrentAnimID == Leftstep)
	{		
		// �E��
		SetWalkParticle(D3DXVECTOR3(playerWk.HitBall[RightFoot].pos.x, -5.0f, playerWk.HitBall[RightFoot].pos.z));

		// ����
		SetWalkParticle(D3DXVECTOR3(playerWk.HitBall[LeftFoot].pos.x, -5.0f, playerWk.HitBall[LeftFoot].pos.z));

	}
}