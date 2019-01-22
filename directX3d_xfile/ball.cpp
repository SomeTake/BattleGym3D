//=============================================================================
//
// �����蔻��p�{�[������ [ball.cpp]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#include "ball.h"
#include "player.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBall(int type, BALL *Ball, D3DXMATRIX Matrix, float Radius)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �|�C���^�̏�����
	Ball->D3DTexture = NULL;
	Ball->Mesh = NULL;
	Ball->D3DXMatBuff = NULL;

	// �ꏊ�A�傫���̐ݒ�
	Ball->pos.x = Matrix._41;
	Ball->pos.y = Matrix._42;
	Ball->pos.z = Matrix._43;
	Ball->rot = Ball->rot;
	Ball->scl = D3DXVECTOR3(Radius, Radius, Radius);

	if (type == 0)
	{
		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(BALL_XFILE,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&Ball->D3DXMatBuff,
			NULL,
			&Ball->NumMat,
			&Ball->Mesh)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitBall(BALL *Ball)
{
	// �e�N�X�`���̊J��
	SAFE_RELEASE(Ball->D3DTexture);

	// ���b�V���̊J��
	SAFE_RELEASE(Ball->Mesh);

	// �}�e���A���̊J��
	SAFE_RELEASE(Ball->D3DXMatBuff);

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBall(BALL *Ball, D3DXMATRIX Matrix)
{
	Ball->pos.x = Matrix._41;
	Ball->pos.y = Matrix._42;
	Ball->pos.z = Matrix._43;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBall(BALL *Ball)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&Ball->mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, Ball->scl.x, Ball->scl.y, Ball->scl.z);
	D3DXMatrixMultiply(&Ball->mtxWorld, &Ball->mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, Ball->rot.y, Ball->rot.x, Ball->rot.z);
	D3DXMatrixMultiply(&Ball->mtxWorld, &Ball->mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, Ball->pos.x, Ball->pos.y, Ball->pos.z);
	D3DXMatrixMultiply(&Ball->mtxWorld, &Ball->mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &Ball->mtxWorld);

	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)Ball->D3DXMatBuff->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)Ball->NumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, Ball->D3DTexture);

		// �`��
		Ball->Mesh->DrawSubset(nCntMat);
	}

	{// �}�e���A�����f�t�H���g�ɖ߂�
		D3DXMATERIAL mat;

		mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		mat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		mat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		pDevice->SetMaterial(&mat.MatD3D);
	}

	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}