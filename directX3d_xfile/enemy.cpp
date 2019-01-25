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
#include "meshwall.h"
#include "particle.h"

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
	enemyWk.HP = FULL_HP;
	enemyWk.HPzan = enemyWk.HP;
	enemyWk.SP = 0;
	enemyWk.HitFrag = false;

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
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Idle, 0.5f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Frontwalk, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Backwalk, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Rightstep, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Leftstep, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Guard, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Damage, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Down, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Downpose, 1.0f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Getup, 1.0f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Punchi, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Kick, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Hadou, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Shoryu, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, SPattack, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Throw, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Win, 0.1f);

		// �����蔻��p�{�[���𐶐�
		D3DXMATRIX Mtx = GetBoneMatrix(enemyWk.Animation, "Hips");
		InitBall(0, &enemyWk.HitBall[0], Mtx, BODY_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "Neck");
		InitBall(0, &enemyWk.HitBall[1], Mtx, BODY_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "Head");
		InitBall(0, &enemyWk.HitBall[2], Mtx, BODY_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "LeftShoulder");
		InitBall(0, &enemyWk.HitBall[3], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "RightShoulder");
		InitBall(0, &enemyWk.HitBall[4], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "LeftHand");
		InitBall(0, &enemyWk.HitBall[5], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "RightHand");
		InitBall(0, &enemyWk.HitBall[6], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "LeftFoot");
		InitBall(0, &enemyWk.HitBall[7], Mtx, FOOT_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "RightFoot");
		InitBall(0, &enemyWk.HitBall[8], Mtx, FOOT_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "LeftForeArm");
		InitBall(0, &enemyWk.HitBall[9], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "RightForeArm");
		InitBall(0, &enemyWk.HitBall[10], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "LeftLeg");
		InitBall(0, &enemyWk.HitBall[11], Mtx, FOOT_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "RightLeg");
		InitBall(0, &enemyWk.HitBall[12], Mtx, FOOT_RADIUS);

		// �e�̐���
		enemyWk.IdxShadow = CreateShadow(enemyWk.pos, SHADOW_SIZE_X, SHADOW_SIZE_Z);
		enemyWk.SizeShadow = 25.0f;
		enemyWk.ColShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	}
	else
	{
		enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle, ANIM_SPD_1);
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

	// �����蔻��{�[���������[�X
	for (int i = 0; i < HIT_CHECK_NUM; i++)
	{
		UninitBall(&enemyWk.HitBall[i]);
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	int *Phase = GetPhase();
	CHARA *playerWk = GetPlayer();

#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("�G�l�~�[���W X:%f Y:%f Z:%f\n", enemyWk.pos.x, enemyWk.pos.y, enemyWk.pos.z);
	PrintDebugProc("�G�l�~�[�p�x X:%f Y:%f Z:%f\n", enemyWk.rot.x, enemyWk.rot.y, enemyWk.rot.z);
	PrintDebugProc("�G�l�~�[�c��HP:%d\n", enemyWk.HPzan);
#endif

	// �ȒP����
	EasyInputEnemy();

	// �{�i����

	// �A�j���[�V�������X�V
	enemyWk.Animation->UpdateAnimation(enemyWk.Animation, TIME_PER_FRAME);

	// ������
	if (*Phase == PhaseFinish && enemyWk.HPzan > playerWk->HPzan && enemyWk.Animation->CurrentAnimID == Idle)
	{
		enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Win, 1.5f);
	}
	// �s�k��HP0�ɂȂ�����_�E��
	if (enemyWk.HPzan <= 0)
	{
		enemyWk.HPzan = 0;
		enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Down, 0.5f);
		SetPhase(PhaseFinish);
	}

	// ���W�ړ�
	MoveEnemy();

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
}

//=============================================================================
//�G�l�~�[�̏����擾����
//=============================================================================
CHARA *GetEnemy(void)
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
	case Idle:
		// �ړ�
		// �O
		if (GetKeyboardTrigger(DIK_A) || IsButtonTriggered(1, BUTTON_RIGHT) || IsButtonTriggered(1, STICK_RIGHT))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Frontwalk, ANIM_SPD_2);
		}
		// ���
		else if (GetKeyboardTrigger(DIK_D) || IsButtonTriggered(1, BUTTON_LEFT) || IsButtonTriggered(1, STICK_LEFT))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Backwalk, ANIM_SPD_2);
		}
		// ��O
		else if (GetKeyboardTrigger(DIK_S) || IsButtonTriggered(1, BUTTON_DOWN) || IsButtonTriggered(1, STICK_DOWN))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Rightstep, ANIM_SPD_2);
		}
		// ��
		else if (GetKeyboardTrigger(DIK_W) || IsButtonTriggered(1, BUTTON_UP) || IsButtonTriggered(1, STICK_UP))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Leftstep, ANIM_SPD_2);
		}

		// �U��
		// �p���`
		else if (GetKeyboardTrigger(DIK_O) || IsButtonTriggered(1, BUTTON_A))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Punchi, ANIM_SPD_15);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_K) || IsButtonTriggered(1, BUTTON_B))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Kick, ANIM_SPD_15);
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_L) || IsButtonTriggered(1, BUTTON_X))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Hadou, ANIM_SPD_2);
		}
		// ������
		else if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(1, BUTTON_C))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Shoryu, ANIM_SPD_15);
		}
		// �K�[�h
		else if (GetKeyboardTrigger(DIK_I) || IsButtonTriggered(1, BUTTON_Y))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Guard, ANIM_SPD_1);
		}
		else
		{
		}
		break;
	case Frontwalk:
		// �O
		if (GetKeyboardPress(DIK_A) || IsButtonPressed(1, BUTTON_RIGHT) || IsButtonPressed(1, STICK_RIGHT))
		{

		}
		// �{�^�������[�X�őҋ@�ɖ߂�
		else
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle, ANIM_SPD_1);
		}

		// �U��
		// �p���`
		if (GetKeyboardTrigger(DIK_O) || IsButtonTriggered(1, BUTTON_A))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Punchi, ANIM_SPD_15);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_K) || IsButtonTriggered(1, BUTTON_B))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Kick, ANIM_SPD_15);
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_L) || IsButtonTriggered(1, BUTTON_X))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Hadou, ANIM_SPD_2);
		}
		// ������
		else if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(1, BUTTON_C))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Shoryu, ANIM_SPD_15);
		}
		// �K�[�h
		else if (GetKeyboardTrigger(DIK_I) || IsButtonTriggered(1, BUTTON_Y))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Guard, ANIM_SPD_1);
		}
		break;
	case Backwalk:
		// ���
		if (GetKeyboardPress(DIK_D) || IsButtonPressed(1, BUTTON_LEFT) || IsButtonPressed(1, STICK_LEFT))
		{

		}
		// �{�^�������[�X�őҋ@�ɖ߂�
		else
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle, ANIM_SPD_1);
		}

		// �U��
		// �p���`
		if (GetKeyboardTrigger(DIK_O) || IsButtonTriggered(1, BUTTON_A))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Punchi, ANIM_SPD_15);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_K) || IsButtonTriggered(1, BUTTON_B))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Kick, ANIM_SPD_15);
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_L) || IsButtonTriggered(1, BUTTON_X))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Hadou, ANIM_SPD_2);
		}
		// ������
		else if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(1, BUTTON_C))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Shoryu, ANIM_SPD_15);
		}
		// �K�[�h
		else if (GetKeyboardTrigger(DIK_I) || IsButtonTriggered(1, BUTTON_Y))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Guard, ANIM_SPD_1);
		}
		break;
	case Rightstep:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle, ANIM_SPD_1);
		}
		break;
	case Leftstep:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle, ANIM_SPD_1);
		}
		break;
	case Guard:
		if (GetKeyboardPress(DIK_I) || IsButtonPressed(1, BUTTON_Y))
		{

		}
		// �{�^�������[�X�őҋ@�ɖ߂�
		else
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle, ANIM_SPD_1);
		}

		// �U��
		// �p���`
		if (GetKeyboardTrigger(DIK_O) || IsButtonTriggered(1, BUTTON_A))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Punchi, ANIM_SPD_15);
		}
		// �L�b�N
		else if (GetKeyboardTrigger(DIK_K) || IsButtonTriggered(1, BUTTON_B))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Kick, ANIM_SPD_15);
		}
		// �g����
		else if (GetKeyboardTrigger(DIK_L) || IsButtonTriggered(1, BUTTON_X))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Hadou, ANIM_SPD_2);
		}
		// ������
		else if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(1, BUTTON_C))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Shoryu, ANIM_SPD_15);
		}
	case Damage:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle, ANIM_SPD_1);
		}
		break;
	case Down:
		// �A�j���[�V�����I���ŋN���オ��Ɉڍs
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Downpose, ANIM_SPD_1);
			D3DXMATRIXA16 newmatrix;	// ���[�V�������W���擾���邽�߂̍s��
			newmatrix = GetBoneMatrix(enemyWk.Animation, "Hips");
			enemyWk.pos = D3DXVECTOR3(newmatrix._41, 0.0f, newmatrix._43);
		}
		break;
	case Downpose:
		// �A�j���[�V�����I���ŋN���オ��Ɉڍs
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Getup, ANIM_SPD_15);
		}
		break;
	case Getup:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle, ANIM_SPD_1);
		}
		break;
	case Punchi:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle, ANIM_SPD_1);
			enemyWk.HitFrag = false;
		}
		break;
	case Kick:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle, ANIM_SPD_1);
			enemyWk.HitFrag = false;
		}
		break;
	case Hadou:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle, ANIM_SPD_1);
			enemyWk.HitFrag = false;
		}
		break;
	case Shoryu:
		// �A�j���[�V�����I���őҋ@�ɖ߂�
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle, ANIM_SPD_1);
			enemyWk.HitFrag = false;
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
	default:
		break;
	}

	// �U�����ȊO��ɒ��S������
	if (enemyWk.Animation->CurrentAnimID == Punchi || enemyWk.Animation->CurrentAnimID == Kick ||
		enemyWk.Animation->CurrentAnimID == Hadou || enemyWk.Animation->CurrentAnimID == Shoryu)
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

//=============================================================================
//�����蔻��
//=============================================================================
void HitCheckEnemy(void)
{
	// �����蔻��̍X�V
	D3DXMATRIX Mtx = GetBoneMatrix(enemyWk.Animation, "Hips");
	UpdateBall(&enemyWk.HitBall[0], Mtx);
	Mtx = GetBoneMatrix(enemyWk.Animation, "Neck");
	UpdateBall(&enemyWk.HitBall[1], Mtx);
	Mtx = GetBoneMatrix(enemyWk.Animation, "Head");
	UpdateBall(&enemyWk.HitBall[2], Mtx);
	Mtx = GetBoneMatrix(enemyWk.Animation, "LeftShoulder");
	UpdateBall(&enemyWk.HitBall[3], Mtx);
	Mtx = GetBoneMatrix(enemyWk.Animation, "RightShoulder");
	UpdateBall(&enemyWk.HitBall[4], Mtx);
	Mtx = GetBoneMatrix(enemyWk.Animation, "LeftHand");
	UpdateBall(&enemyWk.HitBall[5], Mtx);
	Mtx = GetBoneMatrix(enemyWk.Animation, "RightHand");
	UpdateBall(&enemyWk.HitBall[6], Mtx);
	Mtx = GetBoneMatrix(enemyWk.Animation, "LeftFoot");
	UpdateBall(&enemyWk.HitBall[7], Mtx);
	Mtx = GetBoneMatrix(enemyWk.Animation, "RightFoot");
	UpdateBall(&enemyWk.HitBall[8], Mtx);
	Mtx = GetBoneMatrix(enemyWk.Animation, "LeftForeArm");
	UpdateBall(&enemyWk.HitBall[9], Mtx);
	Mtx = GetBoneMatrix(enemyWk.Animation, "RightForeArm");
	UpdateBall(&enemyWk.HitBall[10], Mtx);
	Mtx = GetBoneMatrix(enemyWk.Animation, "LeftLeg");
	UpdateBall(&enemyWk.HitBall[11], Mtx);
	Mtx = GetBoneMatrix(enemyWk.Animation, "RightLeg");
	UpdateBall(&enemyWk.HitBall[12], Mtx);

	// ������AtkPos�Ɣ��a�ƁA�G�l�~�[���̓����蔻��̍��W�Ɣ��a�������铖���蔻��̊֐������
}