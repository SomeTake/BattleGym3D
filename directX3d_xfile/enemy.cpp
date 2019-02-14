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
#include "D3DXAnimation.h"
#include "debugproc.h"
#include "player.h"
#include "meshwall.h"
#include "particle.h"
#include "knockout.h"
#include "eredgauge.h"
#include "sound.h"
#include "shadow.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CHARA enemyWk;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	enemyWk.pos = FIRST_ENEMY_POS;
	enemyWk.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	enemyWk.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	enemyWk.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �X�e�[�^�X���̏����ݒ�
	enemyWk.HP = FULL_HP;
	enemyWk.HPzan = enemyWk.HP;
	enemyWk.SP = 0;
	enemyWk.HitFrag = false;
	enemyWk.score = 0;
	enemyWk.graceflag = false;
	enemyWk.graceframe = 0;
	enemyWk.gracetype = Idle;
	enemyWk.damagecount = 0;

	if (type == 0)
	{
		//�I�u�W�F�N�g�̏�����
		enemyWk.Animation = CreateAnimationObject();

		// xFile�̓ǂݍ���
		if (FAILED(Load_xFile(enemyWk.Animation, CHARA_XFILE, "enemy")))
		{
			return E_FAIL;
		}

		// ���ړ�
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Rightstep])))
		{
			return E_FAIL;
		}
		// ���ړ�
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Leftstep])))
		{
			return E_FAIL;
		}
		// �_���[�W
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Damage])))
		{
			return E_FAIL;
		}
		// �_�E��
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Down])))
		{
			return E_FAIL;
		}
		// �_�E�����
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Downpose])))
		{
			return E_FAIL;
		}
		// �N���オ��
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Getup])))
		{
			return E_FAIL;
		}
		// �p���`
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Punchi])))
		{
			return E_FAIL;
		}
		// �L�b�N
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Kick])))
		{
			return E_FAIL;
		}
		// �g��
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Hadou])))
		{
			return E_FAIL;
		}
		// ����
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Shoryu])))
		{
			return E_FAIL;
		}
		// SP�Z
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[SPattack])))
		{
			return E_FAIL;
		}
		// ����
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Throw])))
		{
			return E_FAIL;
		}
		// �����X�J��
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Miss])))
		{
			return E_FAIL;
		}

		// AnimationSet������������
		for (int i = 0; i < enemyWk.Animation->AnimSetNum; i++)
		{
			if (FAILED(enemyWk.Animation->InitAnimation(enemyWk.Animation, CharaStateAnim[i], i)))
			{
				return E_FAIL;
			}
		}
		enemyWk.Animation->CurrentAnimID = Idle;
		
		// �A�j���[�V�����Ԃ̕⊮��ݒ�
		for (int i = 0; i < AnimMax; i++)
		{
			enemyWk.Animation->SetShiftTime(enemyWk.Animation, i, Data[i].ShiftTime);
		}

		// �����蔻��p�{�[���𐶐�
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			D3DXMATRIX Mtx = GetBoneMatrix(enemyWk.Animation, CharaHitPos[i]);
			InitBall(0, &enemyWk.HitBall[i], Mtx, HitRadius[i]);
		}

	}
	else
	{
		enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle, Data[Idle].Spd);

		// �O�̉e���폜
		DeleteShadow(enemyWk.ShadowIdx);
	}

	// �g�����p�o���b�g���Z�b�g
	InitHadou(type, &enemyWk.HadouBullet);

	// 2P�\���̃r���{�[�h���쐬
	InitPop(type, &enemyWk.Popup, 1);

	// �e�̃Z�b�g
	enemyWk.ShadowIdx = SetShadow(enemyWk.pos, SHADOW_SIZE_X, SHADOW_SIZE_Z);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	// �A�j���[�V�����������[�X
	enemyWk.Animation->UninitAnimation(enemyWk.Animation);

	// �����蔻��{�[���������[�X
	for (int i = 0; i < HIT_CHECK_NUM; i++)
	{
		UninitBall(&enemyWk.HitBall[i]);
	}

	// �g�����p�o���b�g�������[�X
	UninitHadou(&enemyWk.HadouBullet);

	// 2P�\���������[�X
	UninitPop(&enemyWk.Popup);

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	int *Phase = GetPhase();
	CHARA *playerWk = GetPlayer();
	bool ko = GetKnockout()->pushok;

#ifdef _DEBUG
	// ���̓��[�h�̐ؑ�
	if (GetKeyboardTrigger(DIK_8))
	{
		enemyWk.CommandInput = enemyWk.CommandInput ? false : true;
	}
	// �f�o�b�O�\��
	PrintDebugProc("�G�l�~�[���W X:%f Y:%f Z:%f\n", enemyWk.pos.x, enemyWk.pos.y, enemyWk.pos.z);
	PrintDebugProc("�G�l�~�[�p�x X:%f Y:%f Z:%f\n", enemyWk.rot.x, enemyWk.rot.y, enemyWk.rot.z);
	PrintDebugProc("�G�l�~�[�A�j���[�V�����ԍ� No:%d\n", enemyWk.Animation->CurrentAnimID);
	PrintDebugProc("���݂̓��̓��[�h Enemy:%s �؂�ւ�8�L�[\n", enemyWk.CommandInput ? "CommandInput" : "EasyInput");

#endif
	// �`���[�g���A���p
	if (*Phase == PhaseTutorial)
	{
		REDGAUGE *RedWk = GetERedGauge();
		SetupTutorial(&enemyWk, RedWk);
	}

	if (*Phase != PhaseCountdown && *Phase != PhaseTraining && enemyWk.HPzan > 0)
	{
		// ���͐ؑ�
		if (enemyWk.CommandInput == false)
		{
			EasyInput(&enemyWk, 1);
		}
		else if (enemyWk.CommandInput == true)
		{
			CommandInput(&enemyWk, 1);
		}
	}

	if (*Phase == PhaseTraining)
	{
		// AI������
		BattleAI(&enemyWk, playerWk);
	}

	// KO�\�����͍X�V���Ȃ�
	if (*Phase == PhaseFinish && ko == false)
	{

	}
	else
	{
		// �A�j���[�V�������X�V
		enemyWk.Animation->UpdateAnimation(enemyWk.Animation, TIME_PER_FRAME);
	}

	// ������
	if (*Phase == PhaseFinish && enemyWk.HPzan > playerWk->HPzan && enemyWk.Animation->CurrentAnimID == Idle)
	{
		enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Win, Data[Win].Spd);
	}
	// �s�k��HP0�ɂȂ�����_�E��
	if (enemyWk.HPzan <= 0 && *Phase != PhaseTutorial)
	{
		enemyWk.HPzan = 0;
		// �����I�ɃA�j���[�V�����ύX
		if (enemyWk.Animation->CurrentAnimID != Downpose)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Downpose, Data[Downpose].Spd);
		}
		PlaySound(SE_KO, 0, 0);
		StopSound(BGM_BATTLE, 0);
		StopSound(BGM_TRAINING, 0);
		SetPhase(PhaseFinish);
	}

	// KO�\�����͍X�V���Ȃ�
	if (*Phase == PhaseFinish && ko == false)
	{

	}
	else
	{
		// ���W�ړ�
		MoveEnemy();
	}

	// �����蔻��p�{�[�����W�̍X�V
	D3DXMATRIX Mtx;
	for (int i = 0; i < HIT_CHECK_NUM; i++)
	{
		Mtx = GetBoneMatrix(enemyWk.Animation, CharaHitPos[i]);
		UpdateBall(&enemyWk.HitBall[i], Mtx);
	}

	// �����蔻��
	if (enemyWk.HitFrag == false)
	{
		// �L�����N�^�[���m�̓����蔻��
		if (HitCheckCToC(&enemyWk, playerWk) == true)
		{
			// ����������̓���
			HitAction(&enemyWk, playerWk);
		}
	}

	if (enemyWk.HadouBullet.use == true)
	{
		// �g�����p�o���b�g�̍X�V
		UpdateHadou(&enemyWk.HadouBullet);
		
		// �g�����̓����蔻��
		HitHadou(&enemyWk, playerWk);
	}

	// 2P�\���̈ʒu�X�V
	UpdatePop(&enemyWk.Popup, enemyWk.HitBall[Hips].pos);

	// �e�̍X�V
	SetPositionShadow(enemyWk.ShadowIdx, D3DXVECTOR3(enemyWk.HitBall[Hips].pos.x, 0.1f, enemyWk.HitBall[Hips].pos.z));
	SetVertexShadow(enemyWk.ShadowIdx, SHADOW_SIZE_X, SHADOW_SIZE_Z);
	SetColorShadow(enemyWk.ShadowIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

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
	bool RenderState = GetRenderState();

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

	// �`�惂�[�h�����C���[�t���[���ɐ؂�ւ���
	if (RenderState == true)
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}

	// �����_�����O
	enemyWk.Animation->DrawAnimation(enemyWk.Animation, &matrix.world);

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

	if (RenderState == true)
	{
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			// �v���C���[�̓����蔻��p�{�[����`�悷��
			DrawBall(&enemyWk.HitBall[i]);
		}
	}

	if (enemyWk.HadouBullet.use == true)
	{
		// �g�����p�o���b�g�̕`��
		//DrawHadou(&enemyWk.HadouBullet);
	}

	// 2P�\���p�r���{�[�h��`��
	DrawPop(&enemyWk.Popup);
}

//=============================================================================
//�G�l�~�[�̏����擾����
//=============================================================================
CHARA *GetEnemy(void)
{
	return &enemyWk;
}

//=============================================================================
//���W�ړ�
//=============================================================================
void MoveEnemy(void)
{
	CAMERA *camera = GetCamera(0);
	D3DXVECTOR3 centerpos = GetCenterPos();
	CHARA *playerWk = GetPlayer();
	D3DXVECTOR3		oldPos = enemyWk.pos;		// ���̈ʒu
	float PEdistance = GetPEdistance();

	// �A�N�V�����ɍ��킹�����W�ړ�
	switch (enemyWk.Animation->CurrentAnimID)
	{
		// �O�ړ����̍��W����
	case Frontwalk:
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
	case Backwalk:
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
	case Rightstep:
		enemyWk.move.x -= sinf(enemyWk.rot.y - D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		enemyWk.move.z -= cosf(enemyWk.rot.y - D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		break;
		// ���ړ����̍��W����
	case Leftstep:
		enemyWk.move.x -= sinf(enemyWk.rot.y + D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		enemyWk.move.z -= cosf(enemyWk.rot.y + D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		break;
	case Shoryu:
		enemyWk.move.x -= sinf(enemyWk.rot.y) * VALUE_FRONTWALK;
		enemyWk.move.z -= cosf(enemyWk.rot.y) * VALUE_FRONTWALK;
		// ����ɐڐG���Ă����ꍇ�A���������
		if (PEdistance <= MIN_DISTANCE)
		{
			playerWk->move.x -= sinf(playerWk->rot.y + D3DX_PI) * VALUE_FRONTWALK;
			playerWk->move.z -= cosf(playerWk->rot.y + D3DX_PI) * VALUE_FRONTWALK;
		}
	default:
		break;
	}

	// �U����&�_�E�����ȊO��ɒ��S������
	if (enemyWk.Animation->CurrentAnimID == Punchi || enemyWk.Animation->CurrentAnimID == Kick 
		|| enemyWk.Animation->CurrentAnimID == Hadou || enemyWk.Animation->CurrentAnimID == Shoryu
		|| enemyWk.Animation->CurrentAnimID == Down || enemyWk.Animation->CurrentAnimID == Downpose)
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

	// ��ʊO����
	if (enemyWk.pos.x > MOVABLE_AREA)
	{
		enemyWk.pos.x = MOVABLE_AREA;
	}
	else if (enemyWk.pos.x < -MOVABLE_AREA)
	{
		enemyWk.pos.x = -MOVABLE_AREA;
	}
	if (enemyWk.pos.z > MOVABLE_AREA)
	{
		enemyWk.pos.z = MOVABLE_AREA;
	}
	else if (enemyWk.pos.z < -MOVABLE_AREA)
	{
		enemyWk.pos.z = -MOVABLE_AREA;
	}

	// �ړ����̃G�t�F�N�g�̔���
	if (enemyWk.Animation->CurrentAnimID == Frontwalk || enemyWk.Animation->CurrentAnimID == Backwalk ||
		enemyWk.Animation->CurrentAnimID == Rightstep || enemyWk.Animation->CurrentAnimID == Leftstep)
	{
		// �E��
		SetWalkParticle(D3DXVECTOR3(enemyWk.HitBall[RightFoot].pos.x, -5.0f, enemyWk.HitBall[RightFoot].pos.z));

		// ����
		SetWalkParticle(D3DXVECTOR3(enemyWk.HitBall[LeftFoot].pos.x, -5.0f, enemyWk.HitBall[LeftFoot].pos.z));

	}

}
