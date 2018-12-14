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
#include "AnimationModel.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
ENEMY enemyWk[1];

// �I�u�W�F�N�g�̍쐬
D3DXAnimation* EnemyAnimation;
AnimationModel* EnemyModel;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(int type)
{
	ENEMY *enemyWk = GetEnemy(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ModelIndex = 0;

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	enemyWk->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	enemyWk->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	enemyWk->rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	enemyWk->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	enemyWk->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	enemyWk->D3DXMesh = NULL;
	enemyWk->D3DXBuffMat = NULL;
	enemyWk->NumMat = 0;

	enemyWk->jump = false;
	enemyWk->speed = ENEMY_JUMP_SPEED;
	enemyWk->HP = 1000;
	enemyWk->HPzan = enemyWk->HP;

	if (type == 0)
	{
		//�I�u�W�F�N�g�̏�����
		EnemyAnimation = new D3DXAnimation(pDevice);
		EnemyModel = new AnimationModel();

		//X�t�@�C���̓ǂݍ���
		EnemyAnimation->Load_xFile(ENEMY_XFILE);
		EnemyModel->InitController(EnemyAnimation);

		// X�t�@�C���̓ǂݍ���
		//if (FAILED(D3DXLoadMeshFromX(ENEMY_XFILE,			// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		//	D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		//	pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		//	NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		//	&enemyWk->D3DXBuffMat,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		//	NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		//	&enemyWk->NumMat,		// D3DXMATERIAL�\���̂̐�
		//	&enemyWk->D3DXMesh)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X=���_���̌��ʂ�����
		//{
		//	return E_FAIL;
		//}

		// �������̓��I�m��
		enemyWk->D3DTexture = (LPDIRECT3DTEXTURE9 *)malloc(enemyWk->NumMat * sizeof(LPDIRECT3DTEXTURE9));
		enemyWk->MeshMaterial = (D3DMATERIAL9 *)malloc(enemyWk->NumMat * sizeof(D3DMATERIAL9));

#if 0
		D3DXMATERIAL *pD3DXMat;

		pD3DXMat = (D3DXMATERIAL*)enemyWk->D3DXBuffMat->GetBufferPointer();

		// �e�N�X�`���̓ǂݍ���
		for (int i = 0; i < ((signed)enemyWk->NumMat); i++)
		{
			enemyWk->D3DTexture[i] = NULL;

			D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
				pD3DXMat[i].pTextureFilename,					// �t�@�C���̖��O
				&enemyWk->D3DTexture[i]);						// �ǂݍ��ރ������[
		}
#endif
	}

	enemyWk->IdxShadow = CreateShadow(enemyWk->pos, SHADOW_SIZE_X, SHADOW_SIZE_Z);
	enemyWk->SizeShadow = 25.0f;
	enemyWk->ColShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	ENEMY *enemyWk = GetEnemy(0);

	// �f�X�g���N�^
	SAFE_RELEASE(*enemyWk->D3DTexture) {if(*enemyWk->D3DTexture){(*enemyWk->D3DTexture)->Release();(*enemyWk->D3DTexture)=NULL;}}
	SAFE_DELETE(enemyWk->D3DXMesh)	{if(enemyWk->D3DXMesh){delete (enemyWk->D3DXMesh); (enemyWk->D3DXMesh) = NULL;}}
	SAFE_DELETE(enemyWk->D3DXBuffMat) { if (enemyWk->D3DXBuffMat) { delete (enemyWk->D3DXBuffMat); (enemyWk->D3DXBuffMat) = NULL; } }

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	ENEMY *enemyWk = GetEnemy(0);
	CAMERA *camera = GetCamera(1);
	float fDiffRotY;

	//���f���̍X�V
	EnemyModel->AdvanceTime(1.0f / 60);
	
	//���f���̈ړ�
	// D�F�E( �� )
	if (GetKeyboardPress(DIK_L))
	{
		// D + S�F�E��( ���E�� )
		if (GetKeyboardPress(DIK_K))
		{
			enemyWk->move.x -= sinf(camera->rot.y + D3DX_PI * 0.75f) * VALUE_MOVE;
			enemyWk->move.z -= cosf(camera->rot.y + D3DX_PI * 0.75f) * VALUE_MOVE;

			enemyWk->rotDest.y = camera->rot.y + D3DX_PI * 0.75f;
		}
		// D + W�F�E��( ���E�� )
		else if (GetKeyboardPress(DIK_I))
		{
			enemyWk->move.x -= sinf(camera->rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;
			enemyWk->move.z -= cosf(camera->rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;

			enemyWk->rotDest.y = camera->rot.y + D3DX_PI * 0.25f;
		}
		// D�F�E( �� )
		else
		{
			enemyWk->move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * VALUE_MOVE;
			enemyWk->move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * VALUE_MOVE;

			enemyWk->rotDest.y = camera->rot.y + D3DX_PI * 0.50f;
		}
	}
	// A�F��( �� )
	else if (GetKeyboardPress(DIK_J))
	{
		// A + S�F����( ���E�� )
		if (GetKeyboardPress(DIK_K))
		{
			enemyWk->move.x -= sinf(camera->rot.y - D3DX_PI * 0.75f) * VALUE_MOVE;
			enemyWk->move.z -= cosf(camera->rot.y - D3DX_PI * 0.75f) * VALUE_MOVE;

			enemyWk->rotDest.y = camera->rot.y - D3DX_PI * 0.75f;
		}
		// A + W�F����( ���E�� )
		else if (GetKeyboardPress(DIK_I))
		{
			enemyWk->move.x -= sinf(camera->rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;
			enemyWk->move.z -= cosf(camera->rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;

			enemyWk->rotDest.y = camera->rot.y - D3DX_PI * 0.25f;
		}
		// A�F��( �� )
		else
		{
			enemyWk->move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * VALUE_MOVE;
			enemyWk->move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * VALUE_MOVE;

			enemyWk->rotDest.y = camera->rot.y - D3DX_PI * 0.50f;
		}
	}
	// S�F��( �� )
	else if (GetKeyboardPress(DIK_K))
	{
		enemyWk->move.x -= sinf(D3DX_PI + camera->rot.y) * VALUE_MOVE;
		enemyWk->move.z -= cosf(D3DX_PI + camera->rot.y) * VALUE_MOVE;

		enemyWk->rotDest.y = D3DX_PI + camera->rot.y;
	}
	// W�F��( �� )
	else if (GetKeyboardPress(DIK_I))
	{
		enemyWk->move.x -= sinf(camera->rot.y) * VALUE_MOVE;
		enemyWk->move.z -= cosf(camera->rot.y) * VALUE_MOVE;

		enemyWk->rotDest.y = camera->rot.y;
	}

	// �ړI�̊p�x�܂ł̍���
	fDiffRotY = enemyWk->rotDest.y - enemyWk->rot.y;
	if (fDiffRotY > D3DX_PI)
	{
		fDiffRotY -= D3DX_PI * 2.0f;
	}
	if (fDiffRotY < -D3DX_PI)
	{
		fDiffRotY += D3DX_PI * 2.0f;
	}

	// �ړI�̊p�x�܂Ŋ�����������
	enemyWk->rot.y += fDiffRotY * RATE_ROTATE_MODEL;
	if (enemyWk->rot.y > D3DX_PI)
	{
		enemyWk->rot.y -= D3DX_PI * 2.0f;
	}
	if (enemyWk->rot.y < -D3DX_PI)
	{
		enemyWk->rot.y += D3DX_PI * 2.0f;
	}

	/// �ʒu�ړ�
	enemyWk->pos.x += enemyWk->move.x;
	enemyWk->pos.y += enemyWk->move.y;
	//if (enemyWk->pos.y < 5.0f)
	//{
	//	enemyWk->pos.y = 5.0f;
	//}
	//if (enemyWk->pos.y > 75.0f)
	//{
	//	enemyWk->pos.y = 75.0f;
	//}
	enemyWk->pos.z += enemyWk->move.z;

	// �ړ��ʂɊ�����������
	enemyWk->move.x += (0.0f - enemyWk->move.x) * RATE_MOVE_MODEL;
	enemyWk->move.y += (0.0f - enemyWk->move.y) * RATE_MOVE_MODEL;
	enemyWk->move.z += (0.0f - enemyWk->move.z) * RATE_MOVE_MODEL;

	//���f���̃W�����v
	if (GetKeyboardTrigger(DIK_RETURN) && enemyWk->pos.y == 0.0f)
	{
		enemyWk->jump = true;
	}
	if (enemyWk->jump == true)
	{
		enemyWk->pos.y += enemyWk->speed;
		enemyWk->speed -= 0.98f;
		if (enemyWk->pos.y <= 0.0f)
		{
			enemyWk->speed = ENEMY_JUMP_SPEED;
			enemyWk->pos.y = 0.0f;
			enemyWk->jump = false;
		}
	}

	// �e�̈ʒu�ݒ�
	SetPositionShadow(enemyWk->IdxShadow, D3DXVECTOR3(enemyWk->pos.x, 0.1f, enemyWk->pos.z));
	SetVertexShadow(enemyWk->IdxShadow, enemyWk->SizeShadow, enemyWk->SizeShadow);
	SetColorShadow(enemyWk->IdxShadow, enemyWk->ColShadow);
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

	ENEMY *enemyWk = GetEnemy(0);
	CAMERA *cameraWk = GetCamera(0);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_world);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&g_scale, enemyWk->scl.x, enemyWk->scl.y, enemyWk->scl.z);
	D3DXMatrixMultiply(&g_world, &g_world, &g_scale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&g_scale, enemyWk->rot.y, enemyWk->rot.x, enemyWk->rot.z);
	D3DXMatrixMultiply(&g_world, &g_world, &g_scale);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&g_translation, enemyWk->pos.x, enemyWk->pos.y, enemyWk->pos.z);
	D3DXMatrixMultiply(&g_world, &g_world, &g_translation);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_world);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �G�l�~�[�̃����_�����O
	EnemyModel->SetMatrix(&g_world);
	EnemyModel->Render();

#if 0
	D3DXMATERIAL *pD3DXMat;

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)enemyWk->D3DXBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)enemyWk->NumMat; nCntMat++)
	{

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, enemyWk->D3DTexture[nCntMat]);

		// �`��
		enemyWk->D3DXMesh->DrawSubset(nCntMat);
	}

#endif

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
//�G�l�~�[�̏����擾����
//=============================================================================
ENEMY *GetEnemy(int pno)
{
	return &enemyWk[pno];
}