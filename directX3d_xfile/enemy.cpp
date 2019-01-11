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

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
ENEMY enemyWk;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ModelIndex = 0;

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	enemyWk.pos = D3DXVECTOR3(-50.0f, 0.0f, 0.0f);
	enemyWk.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	enemyWk.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	enemyWk.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	enemyWk.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	enemyWk.jump = false;
	enemyWk.speed = ENEMY_JUMP_SPEED;
	enemyWk.HP = 1000;
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

		// AnimationCallback���Z�b�g����

		// AnimationSet������������

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
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	CAMERA *camera = GetCamera(1);
	D3DXVECTOR3 centerpos = GetCenterPos();

	// �A�j���[�V�������X�V
	enemyWk.Animation->UpdateAnimation(enemyWk.Animation, TIME_PER_FRAME);

	//���f���̈ړ�
	// D�F�E( �� )
	if (GetKeyboardPress(DIK_L))
	{
		// D + S�F�E��( ���E�� )
		if (GetKeyboardPress(DIK_K))
		{
			enemyWk.move.x -= sinf(camera->rot.y + D3DX_PI * 0.75f) * VALUE_MOVE;
			enemyWk.move.z -= cosf(camera->rot.y + D3DX_PI * 0.75f) * VALUE_MOVE;

			enemyWk.rotDest.y = camera->rot.y + D3DX_PI * 0.75f;
		}
		// D + W�F�E��( ���E�� )
		else if (GetKeyboardPress(DIK_I))
		{
			enemyWk.move.x -= sinf(camera->rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;
			enemyWk.move.z -= cosf(camera->rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;

			enemyWk.rotDest.y = camera->rot.y + D3DX_PI * 0.25f;
		}
		// D�F�E( �� )
		else
		{
			enemyWk.move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * VALUE_MOVE;
			enemyWk.move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * VALUE_MOVE;

			enemyWk.rotDest.y = camera->rot.y + D3DX_PI * 0.50f;
		}
	}
	// A�F��( �� )
	else if (GetKeyboardPress(DIK_J))
	{
		// A + S�F����( ���E�� )
		if (GetKeyboardPress(DIK_K))
		{
			enemyWk.move.x -= sinf(camera->rot.y - D3DX_PI * 0.75f) * VALUE_MOVE;
			enemyWk.move.z -= cosf(camera->rot.y - D3DX_PI * 0.75f) * VALUE_MOVE;

			enemyWk.rotDest.y = camera->rot.y - D3DX_PI * 0.75f;
		}
		// A + W�F����( ���E�� )
		else if (GetKeyboardPress(DIK_I))
		{
			enemyWk.move.x -= sinf(camera->rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;
			enemyWk.move.z -= cosf(camera->rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;

			enemyWk.rotDest.y = camera->rot.y - D3DX_PI * 0.25f;
		}
		// A�F��( �� )
		else
		{
			enemyWk.move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * VALUE_MOVE;
			enemyWk.move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * VALUE_MOVE;

			enemyWk.rotDest.y = camera->rot.y - D3DX_PI * 0.50f;
		}
	}
	// S�F��( �� )
	else if (GetKeyboardPress(DIK_K))
	{
		enemyWk.move.x -= sinf(D3DX_PI + camera->rot.y) * VALUE_MOVE;
		enemyWk.move.z -= cosf(D3DX_PI + camera->rot.y) * VALUE_MOVE;

		enemyWk.rotDest.y = D3DX_PI + camera->rot.y;
	}
	// W�F��( �� )
	else if (GetKeyboardPress(DIK_I))
	{
		enemyWk.move.x -= sinf(camera->rot.y) * VALUE_MOVE;
		enemyWk.move.z -= cosf(camera->rot.y) * VALUE_MOVE;

		enemyWk.rotDest.y = camera->rot.y;
	}

	// �s�k��
	if (enemyWk.HPzan <= 0)
	{
		enemyWk.HPzan = 0;
		SetPhase(PhaseFinish);
	}

	// ��ɒ��S������
	enemyWk.rot.y = atan2f(centerpos.x - enemyWk.pos.x, centerpos.z - enemyWk.pos.z) + D3DX_PI;

	// �ʒu�ړ�
	enemyWk.pos.x += enemyWk.move.x;
	enemyWk.pos.y += enemyWk.move.y;
	enemyWk.pos.z += enemyWk.move.z;

	// �ړ��ʂɊ�����������
	enemyWk.move.x += (0.0f - enemyWk.move.x) * RATE_MOVE_MODEL;
	enemyWk.move.y += (0.0f - enemyWk.move.y) * RATE_MOVE_MODEL;
	enemyWk.move.z += (0.0f - enemyWk.move.z) * RATE_MOVE_MODEL;

	//���f���̃W�����v
	if (GetKeyboardTrigger(DIK_RETURN) && enemyWk.pos.y == 0.0f)
	{
		enemyWk.jump = true;
	}
	if (enemyWk.jump == true)
	{
		enemyWk.pos.y += enemyWk.speed;
		enemyWk.speed -= 0.98f;
		if (enemyWk.pos.y <= 0.0f)
		{
			enemyWk.speed = ENEMY_JUMP_SPEED;
			enemyWk.pos.y = 0.0f;
			enemyWk.jump = false;
		}
	}

#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("�G�l�~�[���W X:%f Y:%f Z:%f\n", enemyWk.pos.x, enemyWk.pos.y, enemyWk.pos.z);
	PrintDebugProc("�G�l�~�[�p�x X:%f Y:%f Z:%f\n", enemyWk.rot.x, enemyWk.rot.y, enemyWk.rot.z);
#endif

	// �e�̈ʒu�ݒ�
	SetPositionShadow(enemyWk.IdxShadow, D3DXVECTOR3(enemyWk.pos.x, 0.1f, enemyWk.pos.z));
	SetVertexShadow(enemyWk.IdxShadow, enemyWk.SizeShadow, enemyWk.SizeShadow);
	SetColorShadow(enemyWk.IdxShadow, enemyWk.ColShadow);
}

D3DXMATRIX g_scale;		// �X�P�[���}�g���N�X
D3DXMATRIX g_rotation;	// ���[�e�[�V�����}�g���N�X
D3DXMATRIX g_translation;	// �g�����X���[�V�����}�g���N�X
D3DXMATRIX g_view;		// �r���[�}�g���N�X
D3DXMATRIX g_projection;	// �v���W�F�N�V�����}�g���N�X
D3DXMATRIX g_world;		// ���[���h�}�g���N�X

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DMATERIAL9 matDef;
	//MATRIX matrix = *GetMatrix();

	CAMERA *cameraWk = GetCamera(0);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_world);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&g_scale, enemyWk.scl.x, enemyWk.scl.y, enemyWk.scl.z);
	D3DXMatrixMultiply(&g_world, &g_world, &g_scale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&g_scale, enemyWk.rot.y, enemyWk.rot.x, enemyWk.rot.z);
	D3DXMatrixMultiply(&g_world, &g_world, &g_scale);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&g_translation, enemyWk.pos.x, enemyWk.pos.y, enemyWk.pos.z);
	D3DXMatrixMultiply(&g_world, &g_world, &g_translation);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_world);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �����_�����O
	enemyWk.Animation->DrawAnimation(enemyWk.Animation, &g_world);

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
//�G�l�~�[�̏����擾����
//=============================================================================
ENEMY *GetEnemy(void)
{
	return &enemyWk;
}