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
#include "model.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitParts(void);
HRESULT LoadXFile(void);

// �A�j���[�V�����֘A
void InitAnim(void);
void UpdateAnim(void);

void DrawPlayer_RArm(void);
void DrawPlayer_LArm(void);
void DrawPlayer_RFoot(void);
void DrawPlayer_LFoot(void);
void DrawPlayer_Head(void);

D3DXMATRIX *GetPlayerWorldMatrix(void);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

PLAYER playerWk[1];

// �I�u�W�F�N�g�̍쐬
//D3DXAnimation* PlayerAnimation;
//AnimationModel* PlayerModel;
//UINT Idle;

CModel* PlayerModel;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(int type)
{
	PLAYER *playerWk = GetPlayer(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�I�u�W�F�N�g�̏�����
	//PlayerAnimation = new D3DXAnimation(pDevice);
	//PlayerModel = new AnimationModel();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	InitParts();

	// �A�j���[�V�����f�[�^�̏�����
	InitAnim();

	playerWk->jump = false;
	playerWk->speed = JUMP_SPEED;

	//X�t�@�C���̓ǂݍ���
	//PlayerAnimation->Load_xFile(PLAYER_XFILE);
	//PlayerModel->InitController(PlayerAnimation);

	PlayerModel->CModel::CModel(pDevice, PLAYER_XFILE, 0);

	//�����A�j���[�V����
	//PlayerModel->ChangeAnimation(Idle);

	if (type == 0)
	{
		// X�t�@�C���̓ǂݍ���
		//LoadXFile();

		// �������̓��I�m��
		playerWk->D3DTexture = (LPDIRECT3DTEXTURE9 *)malloc(playerWk->NumMat * sizeof(LPDIRECT3DTEXTURE9));
		playerWk->MeshMaterial = (D3DMATERIAL9 *)malloc(playerWk->NumMat * sizeof(D3DMATERIAL9));

#if 1		
		D3DXMATERIAL *pD3DXMat;

		pD3DXMat = (D3DXMATERIAL*)playerWk->D3DXBuffMat->GetBufferPointer();

		// �e�N�X�`���̓ǂݍ���
		for (int i = 0; i < ((signed)playerWk->NumMat); i++)
		{
			playerWk->D3DTexture[i] = NULL;

			D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
				pD3DXMat[i].pTextureFilename,					// �t�@�C���̖��O
				&playerWk->D3DTexture[i]);						// �ǂݍ��ރ������[
		}
#endif
	}

	playerWk->IdxShadow = CreateShadow(playerWk->pos, SHADOW_SIZE_X, SHADOW_SIZE_Z);
	playerWk->SizeShadow = 25.0f;
	playerWk->ColShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	PLAYER *playerWk = GetPlayer(0);

	for (int i = 0; i < (signed)playerWk->NumMat; i++)
	{
		if (playerWk->D3DTexture != NULL)
		{// �e�N�X�`���̊J��
			playerWk->D3DTexture[i]->Release();
			playerWk->D3DTexture[i] = NULL;
		}
	}

	if(playerWk->D3DXMesh != NULL)
	{// ���b�V���̊J��
		playerWk->D3DXMesh->Release();
		playerWk->D3DXMesh = NULL;
	}

	if(playerWk->D3DXBuffMat != NULL)
	{// �}�e���A���̊J��
		playerWk->D3DXBuffMat->Release();
		playerWk->D3DXBuffMat = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	PLAYER *playerWk = GetPlayer(0);
	CAMERA *camera = GetCamera(0);
	float fDiffRotY;

	////���f���̍X�V
	//PlayerModel->AdvanceTime(1.0f / 60);

	//���f���̈ړ�
	// D�F�E( �� )
	if (GetKeyboardPress(DIK_D))
	{
		// D + S�F�E��( ���E�� )
		if (GetKeyboardPress(DIK_S))
		{
			playerWk->move.x -= sinf(camera->rot.y + D3DX_PI * 0.75f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y + D3DX_PI * 0.75f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y + D3DX_PI * 0.75f;
		}
		// D + W�F�E��( ���E�� )
		else if (GetKeyboardPress(DIK_W))
		{
			playerWk->move.x -= sinf(camera->rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y + D3DX_PI * 0.25f;
		}
		// D�F�E( �� )
		else
		{
			playerWk->move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y + D3DX_PI * 0.50f;
		}
	}
	// A�F��( �� )
	else if (GetKeyboardPress(DIK_A))
	{
		// A + S�F����( ���E�� )
		if (GetKeyboardPress(DIK_S))
		{
			playerWk->move.x -= sinf(camera->rot.y - D3DX_PI * 0.75f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y - D3DX_PI * 0.75f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y - D3DX_PI * 0.75f;
		}
		// A + W�F����( ���E�� )
		else if (GetKeyboardPress(DIK_W))
		{
			playerWk->move.x -= sinf(camera->rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y - D3DX_PI * 0.25f;
		}
		// A�F��( �� )
		else
		{
			playerWk->move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y - D3DX_PI * 0.50f;
		}
	}
	// S�F��( �� )
	else if (GetKeyboardPress(DIK_S))
	{
		playerWk->move.x -= sinf(D3DX_PI + camera->rot.y) * VALUE_MOVE;
		playerWk->move.z -= cosf(D3DX_PI + camera->rot.y) * VALUE_MOVE;

		playerWk->rotDest.y = D3DX_PI + camera->rot.y;
	}
	// W�F��( �� )
	else if (GetKeyboardPress(DIK_W))
	{
		playerWk->move.x -= sinf(camera->rot.y) * VALUE_MOVE;
		playerWk->move.z -= cosf(camera->rot.y) * VALUE_MOVE;

		playerWk->rotDest.y = camera->rot.y;
	}


	// �ړI�̊p�x�܂ł̍���
	fDiffRotY = playerWk->rotDest.y - playerWk->rot.y;
	if (fDiffRotY > D3DX_PI)
	{
		fDiffRotY -= D3DX_PI * 2.0f;
	}
	if (fDiffRotY < -D3DX_PI)
	{
		fDiffRotY += D3DX_PI * 2.0f;
	}

	// �ړI�̊p�x�܂Ŋ�����������
	playerWk->rot.y += fDiffRotY * RATE_ROTATE_MODEL;
	if (playerWk->rot.y > D3DX_PI)
	{
		playerWk->rot.y -= D3DX_PI * 2.0f;
	}
	if (playerWk->rot.y < -D3DX_PI)
	{
		playerWk->rot.y += D3DX_PI * 2.0f;
	}

	/// �ʒu�ړ�
	playerWk->pos.x += playerWk->move.x;
	playerWk->pos.y += playerWk->move.y;
	playerWk->pos.z += playerWk->move.z;

	// �ړ��ʂɊ�����������
	playerWk->move.x += (0.0f - playerWk->move.x) * RATE_MOVE_MODEL;
	playerWk->move.y += (0.0f - playerWk->move.y) * RATE_MOVE_MODEL;
	playerWk->move.z += (0.0f - playerWk->move.z) * RATE_MOVE_MODEL;

	//���f���̊g��k��
	if (GetKeyboardPress(DIK_C))
	{
		playerWk->scl.x += VALUE_ENLARGEMENT;
		playerWk->scl.y += VALUE_ENLARGEMENT;
		playerWk->scl.z += VALUE_ENLARGEMENT;
	}
	else if (GetKeyboardPress(DIK_Z))
	{
		playerWk->scl.x -= VALUE_ENLARGEMENT;
		playerWk->scl.y -= VALUE_ENLARGEMENT;
		playerWk->scl.z -= VALUE_ENLARGEMENT;
	}

	//���f���̃W�����v
	if (GetKeyboardTrigger(DIK_SPACE) && playerWk->pos.y == 0.0f)
	{
		playerWk->jump = true;
	}
	if (playerWk->jump == true)
	{
		playerWk->pos.y += playerWk->speed;
		playerWk->speed -= 0.98f;
		if (playerWk->pos.y <= 0.0f)
		{
			playerWk->speed = JUMP_SPEED;
			playerWk->pos.y = 0.0f;
			playerWk->jump = false;
		}
	}

	// �A�j���[�V�����̍X�V
	UpdateAnim();

	// �e�̈ʒu�ݒ�
	SetPositionShadow(playerWk->IdxShadow, D3DXVECTOR3(playerWk->pos.x, 0.1f, playerWk->pos.z));
	SetVertexShadow(playerWk->IdxShadow, playerWk->SizeShadow, playerWk->SizeShadow);
	SetColorShadow(playerWk->IdxShadow, playerWk->ColShadow);
}

//=============================================================================
// �`�揈��
//=============================================================================
MATRIX g_matrix;

void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	PLAYER *playerWk = GetPlayer(0);
	CAMERA *cameraWk = GetCamera(0);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_matrix.world);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&g_matrix.scale, playerWk->scl.x, playerWk->scl.y, playerWk->scl.z);
	D3DXMatrixMultiply(&g_matrix.world, &g_matrix.world, &g_matrix.scale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&g_matrix.rotation, playerWk->rotDest.y, playerWk->rotDest.x, playerWk->rotDest.z);
	D3DXMatrixMultiply(&g_matrix.world, &g_matrix.world, &g_matrix.rotation);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&g_matrix.translation, playerWk->pos.x, playerWk->pos.y, playerWk->pos.z);
	D3DXMatrixMultiply(&g_matrix.world, &g_matrix.world, &g_matrix.translation);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_matrix.world);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �v���C���[�̃����_�����O
	//PlayerModel->SetMatrix(&g_matrix.world);
	//PlayerModel->Render();

	//Draw_Shader();

#if 1
	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)playerWk->D3DXBuffMat->GetBufferPointer();

	for(int nCntMat = 0; nCntMat < (int)playerWk->NumMat; nCntMat++)
	{

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, playerWk->D3DTexture[nCntMat]);

		// �`��
		playerWk->D3DXMesh->DrawSubset(nCntMat);
	}

#endif

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

	DrawPlayer_RArm();
	DrawPlayer_LArm();
	DrawPlayer_RFoot();
	DrawPlayer_LFoot();
	DrawPlayer_Head();

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer_RArm(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	MATRIX *matrix = GetMatrix();

	D3DXMATRIX mtxWorld;

	PLAYER *playerWk = GetPlayer(0);
	CAMERA *cameraWk = GetCamera(0);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&matrix->scale, playerWk->scl_rarm.x, playerWk->scl_rarm.y, playerWk->scl_rarm.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->scale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&matrix->rotation, playerWk->rot_rarm.y, playerWk->rot_rarm.x, playerWk->rot_rarm.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->rotation);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&matrix->translation, playerWk->pos_rarm.x, playerWk->pos_rarm.y, playerWk->pos_rarm.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->translation);

	// ���̂̃��[���h�}�g���N�X�ƘA������
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerWorldMatrix());

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//player_model->SetMatrix(&mtxWorld);
	//player_model->Render();

#if 1
	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)playerWk->D3DXBuffMat_rarm->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)playerWk->NumMat_rarm; nCntMat++)
	{

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, playerWk->D3DTexture[nCntMat]);

		// �`��
		playerWk->D3DXMesh_rarm->DrawSubset(nCntMat);
	}

#endif

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer_LArm(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	MATRIX *matrix = GetMatrix();

	D3DXMATRIX mtxWorld;

	PLAYER *playerWk = GetPlayer(0);
	CAMERA *cameraWk = GetCamera(0);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&matrix->scale, playerWk->scl_larm.x, playerWk->scl_larm.y, playerWk->scl_larm.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->scale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&matrix->rotation, playerWk->rot_larm.y, playerWk->rot_larm.x, playerWk->rot_larm.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->rotation);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&matrix->translation, playerWk->pos_larm.x, playerWk->pos_larm.y, playerWk->pos_larm.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->translation);

	// ���̂̃��[���h�}�g���N�X�ƘA������
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerWorldMatrix());

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//player_model->SetMatrix(&mtxWorld);
	//player_model->Render();

#if 1
	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)playerWk->D3DXBuffMat_larm->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)playerWk->NumMat_larm; nCntMat++)
	{

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, playerWk->D3DTexture[nCntMat]);

		// �`��
		playerWk->D3DXMesh_larm->DrawSubset(nCntMat);
	}

#endif

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer_RFoot(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	MATRIX *matrix = GetMatrix();

	D3DXMATRIX mtxWorld;

	PLAYER *playerWk = GetPlayer(0);
	CAMERA *cameraWk = GetCamera(0);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&matrix->scale, playerWk->scl_rfoot.x, playerWk->scl_rfoot.y, playerWk->scl_rfoot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->scale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&matrix->rotation, playerWk->rot_rfoot.y, playerWk->rot_rfoot.x, playerWk->rot_rfoot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->rotation);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&matrix->translation, playerWk->pos_rfoot.x, playerWk->pos_rfoot.y, playerWk->pos_rfoot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->translation);

	// ���̂̃��[���h�}�g���N�X�ƘA������
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerWorldMatrix());

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//player_model->SetMatrix(&mtxWorld);
	//player_model->Render();

#if 1
	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)playerWk->D3DXBuffMat_rfoot->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)playerWk->NumMat_rfoot; nCntMat++)
	{

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, playerWk->D3DTexture[nCntMat]);

		// �`��
		playerWk->D3DXMesh_rfoot->DrawSubset(nCntMat);
	}

#endif

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer_LFoot(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	MATRIX *matrix = GetMatrix();

	D3DXMATRIX mtxWorld;

	PLAYER *playerWk = GetPlayer(0);
	CAMERA *cameraWk = GetCamera(0);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&matrix->scale, playerWk->scl_lfoot.x, playerWk->scl_lfoot.y, playerWk->scl_lfoot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->scale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&matrix->rotation, playerWk->rot_lfoot.y, playerWk->rot_lfoot.x, playerWk->rot_lfoot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->rotation);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&matrix->translation, playerWk->pos_lfoot.x, playerWk->pos_lfoot.y, playerWk->pos_lfoot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->translation);

	// ���̂̃��[���h�}�g���N�X�ƘA������
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerWorldMatrix());

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//player_model->SetMatrix(&mtxWorld);
	//player_model->Render();

#if 1
	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)playerWk->D3DXBuffMat_lfoot->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)playerWk->NumMat_lfoot; nCntMat++)
	{

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, playerWk->D3DTexture[nCntMat]);

		// �`��
		playerWk->D3DXMesh_lfoot->DrawSubset(nCntMat);
	}

#endif

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer_Head(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	MATRIX *matrix = GetMatrix();

	D3DXMATRIX mtxWorld;

	PLAYER *playerWk = GetPlayer(0);
	CAMERA *cameraWk = GetCamera(0);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&matrix->scale, playerWk->scl_head.x, playerWk->scl_head.y, playerWk->scl_head.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->scale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&matrix->rotation, playerWk->rot_head.y, playerWk->rot_head.x, playerWk->rot_head.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->rotation);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&matrix->translation, playerWk->pos_head.x, playerWk->pos_head.y, playerWk->pos_head.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->translation);

	// ���̂̃��[���h�}�g���N�X�ƘA������
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerWorldMatrix());

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//player_model->SetMatrix(&mtxWorld);
	//player_model->Render();

#if 1
	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)playerWk->D3DXBuffMat_head->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)playerWk->NumMat_head; nCntMat++)
	{

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, playerWk->D3DTexture[nCntMat]);

		// �`��
		playerWk->D3DXMesh_head->DrawSubset(nCntMat);
	}

#endif

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
//�v���C���[�̏����擾����
//=============================================================================
PLAYER *GetPlayer(int pno)
{
	return &playerWk[pno];
}

//=============================================================================
//�v���C���[�̃��[���h�}�g���N�X���擾����
//=============================================================================
D3DXMATRIX *GetPlayerWorldMatrix(void)
{
	return &g_matrix.world;
}

//=============================================================================
//�p�[�c�A�j���[�V�������g���ꍇ�̏���������
//=============================================================================
void InitParts(void)
{
	// ����
	playerWk->pos = D3DXVECTOR3(0.0f, 80.0f, 0.0f);
	playerWk->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	playerWk->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�E�r
	playerWk->pos_rarm = D3DXVECTOR3(-100.0f, 180.0f, 0.0f);
	playerWk->rot_rarm = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->rotDest_rarm = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->scl_rarm = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	playerWk->move_rarm = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���r
	playerWk->pos_larm = D3DXVECTOR3(100.0f, 180.0f, 0.0f);
	playerWk->rot_larm = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->rotDest_larm = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->scl_larm = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	playerWk->move_larm = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�E��
	playerWk->pos_rfoot = D3DXVECTOR3(-30.0f, 0.0f, 0.0f);
	playerWk->rot_rfoot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->rotDest_rfoot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->scl_rfoot = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	playerWk->move_rfoot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//����
	playerWk->pos_lfoot = D3DXVECTOR3(30.0f, 0.0f, 0.0f);
	playerWk->rot_lfoot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->rotDest_lfoot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->scl_lfoot = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	playerWk->move_lfoot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//��
	playerWk->pos_head = D3DXVECTOR3(0.0f, 150.0f, 0.0f);
	playerWk->rot_head = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->rotDest_head = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->scl_head = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	playerWk->move_head = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	//����
	playerWk->D3DXMesh = NULL;
	playerWk->D3DXBuffMat = NULL;
	playerWk->NumMat = 0;

	//�E�r
	playerWk->D3DXMesh_rarm = NULL;
	playerWk->D3DXBuffMat_rarm = NULL;
	playerWk->NumMat_rarm = 0;

	//���r
	playerWk->D3DXMesh_larm = NULL;
	playerWk->D3DXBuffMat_larm = NULL;
	playerWk->NumMat_larm = 0;

	//�E��
	playerWk->D3DXMesh_rfoot = NULL;
	playerWk->D3DXBuffMat_rfoot = NULL;
	playerWk->NumMat_rfoot = 0;

	//����
	playerWk->D3DXMesh_lfoot = NULL;
	playerWk->D3DXBuffMat_lfoot = NULL;
	playerWk->NumMat_lfoot = 0;

	//��
	playerWk->D3DXMesh_head = NULL;
	playerWk->D3DXBuffMat_head = NULL;
	playerWk->NumMat_head = 0;
}

//=============================================================================
//�p�[�c�A�j���[�V�������g���ꍇ��X�t�@�C���ǂݍ��ݏ���
//=============================================================================
HRESULT LoadXFile(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//����
	if (FAILED(D3DXLoadMeshFromX("body.x",			// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&playerWk->D3DXBuffMat,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&playerWk->NumMat,		// D3DXMATERIAL�\���̂̐�
		&playerWk->D3DXMesh)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X=���_���̌��ʂ�����
	{
		return E_FAIL;
	}

	//�E�r
	if (FAILED(D3DXLoadMeshFromX("rightarm.x",			// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&playerWk->D3DXBuffMat_rarm,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&playerWk->NumMat_rarm,		// D3DXMATERIAL�\���̂̐�
		&playerWk->D3DXMesh_rarm)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X=���_���̌��ʂ�����
	{
		return E_FAIL;
	}

	//���r
	if (FAILED(D3DXLoadMeshFromX("leftarm.x",			// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&playerWk->D3DXBuffMat_larm,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&playerWk->NumMat_larm,		// D3DXMATERIAL�\���̂̐�
		&playerWk->D3DXMesh_larm)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X=���_���̌��ʂ�����
	{
		return E_FAIL;
	}

	//�E��
	if (FAILED(D3DXLoadMeshFromX("rightfoot.x",			// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&playerWk->D3DXBuffMat_rfoot,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&playerWk->NumMat_rfoot,		// D3DXMATERIAL�\���̂̐�
		&playerWk->D3DXMesh_rfoot)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X=���_���̌��ʂ�����
	{
		return E_FAIL;
	}

	//����
	if (FAILED(D3DXLoadMeshFromX("leftfoot.x",			// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&playerWk->D3DXBuffMat_lfoot,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&playerWk->NumMat_lfoot,		// D3DXMATERIAL�\���̂̐�
		&playerWk->D3DXMesh_lfoot)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X=���_���̌��ʂ�����
	{
		return E_FAIL;
	}

	//��
	if (FAILED(D3DXLoadMeshFromX("head.x",			// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&playerWk->D3DXBuffMat_head,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&playerWk->NumMat_head,		// D3DXMATERIAL�\���̂̐�
		&playerWk->D3DXMesh_head)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X=���_���̌��ʂ�����
	{
		return E_FAIL;
	}

	return S_OK;
}

struct PART {
	PART *parent;
};

PART g_Part[PARTS_MAX];	//�p�[�c�̐����I�u�W�F�N�g�����

struct KEY {
	D3DXVECTOR3	pos;
	D3DXVECTOR3	rot;
};

// �A�j���[�V�����f�[�^�̒�`
struct KEY_ANIME
{
	int			frame;
	KEY			key[PARTS_MAX];
};

// �A�j���[�V�����f�[�^�̐錾
KEY_ANIME g_anim[] = {
	//frame,pos,rot
	{
		//�t���[����
		60, 
	// ����
	D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	// �E�r
	D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	// ���r
	D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	// �E��
	D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	// ����
	D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	// ��
	D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	},
		{
			//�t���[����
			60,
			// ����
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			// �E�r
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f),
			// ���r
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
			// �E��
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
			// ����
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f),
			// ��
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		},
				{
					//�t���[����
					60,
					// ����
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					// �E�r
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
					// ���r
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f),
					// �E��
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f),
					// ����
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
					// ��
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				},
						{
							//�t���[����
							30,
							// ����
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							// �E�r
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f),
							// ���r
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
							// �E��
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
							// ����
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f),
							// ��
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						},
};

int g_keyMax;					// �ʒu�f�[�^�̐�
float g_MotionTime = 0.0f;		// �A�j���[�V�����S�̎���

//=============================================================================
//�A�j���[�V�����̏���������
//=============================================================================
void InitAnim(void)
{
	//g_Part[0]��=�e
	//�������e�Ȃ�NULL�A����ȊO�Ȃ�e�̃A�h���X
	g_Part[0].parent = NULL;

	//g_Part[1]��=�q��
	//�������e�Ȃ�NULL�A����ȊO�Ȃ�e�̃A�h���X
	g_Part[1].parent = &g_Part[0];
	//g_Part[1]��=�q��
	//�������e�Ȃ�NULL�A����ȊO�Ȃ�e�̃A�h���X
	g_Part[2].parent = &g_Part[0];
	//g_Part[1]��=�q��
	//�������e�Ȃ�NULL�A����ȊO�Ȃ�e�̃A�h���X
	g_Part[3].parent = &g_Part[0];
	//g_Part[1]��=�q��
	//�������e�Ȃ�NULL�A����ȊO�Ȃ�e�̃A�h���X
	g_Part[4].parent = &g_Part[0];
	//g_Part[1]��=�q��
	//�������e�Ȃ�NULL�A����ȊO�Ȃ�e�̃A�h���X
	g_Part[5].parent = &g_Part[0];

	// �L�[�f�[�^�̐�
	g_keyMax = sizeof(g_anim) / sizeof(KEY_ANIME);	// �f�[�^�̃T�C�Y/�f�[�^�̗v�f��=��
}

//=============================================================================
//�A�j���[�V�����̃A�b�v�f�[�g����
//=============================================================================
void UpdateAnim(void)
{
	int i = 0;
	i += (int)g_MotionTime;				// i �ɑS�̃A�j�����Ԃ̐�����������

	// �A�j���X�V
	g_MotionTime += 1.0f / g_anim[i].frame;

	if (GetKeyState(VK_BACK) & 0x80)
	{
		g_MotionTime = 0.0f;
	}

	if (g_MotionTime > g_keyMax - 1.0f)
	{
		g_MotionTime = g_keyMax - 1.0f;
	}

	if (i > g_keyMax - 2)
	{
		i = g_keyMax - 2;
	}

	for (int j = 0; j < PARTS_MAX; j++)
	{
		// �ړ�
		// playerWk->pos = g_anim[i].key[j].pos + (g_anim[i + 1].key[j].pos - g_anim[i].key[j].pos) * (g_MotionTime - i);	// 0�̈ʒu
																														// 1��0�̍���
																														// �ɃA�j���[�V�������Ԃ̔䗦������������

		// ��]
		// ����
		if (j == 0)
		{
			// ��x���W�𒆐S�ɖ߂��Ă����]������
			playerWk->rot = g_anim[i].key[j].rot + (g_anim[i + 1].key[j].rot - g_anim[i].key[j].rot) * (g_MotionTime - i);
			// �܂����̈ʒu�ɖ߂�
		}
		// �E�r
		if (j == 1)
		{
			playerWk->rot_rarm = g_anim[i].key[j].rot + (g_anim[i + 1].key[j].rot - g_anim[i].key[j].rot) * (g_MotionTime - i);
		}
		// ���r
		if (j == 2)
		{
			playerWk->rot_larm = g_anim[i].key[j].rot + (g_anim[i + 1].key[j].rot - g_anim[i].key[j].rot) * (g_MotionTime - i);
		}
		// �E��
		if (j == 3)
		{
			playerWk->rot_rfoot = g_anim[i].key[j].rot + (g_anim[i + 1].key[j].rot - g_anim[i].key[j].rot) * (g_MotionTime - i);
		}
		// ����
		if (j == 4)
		{
			playerWk->rot_lfoot = g_anim[i].key[j].rot + (g_anim[i + 1].key[j].rot - g_anim[i].key[j].rot) * (g_MotionTime - i);
		}
		// ��
		if (j == 5)
		{
			playerWk->rot_head = g_anim[i].key[j].rot + (g_anim[i + 1].key[j].rot - g_anim[i].key[j].rot) * (g_MotionTime - i);
		}
	}

}