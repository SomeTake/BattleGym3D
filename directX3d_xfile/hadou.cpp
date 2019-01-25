//=============================================================================
//
// �g�����p�o���b�g���� [hadou.cpp]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#include "battle.h"
#include "player.h"
#include "enemy.h"
#include "hadou.h"
#include "D3DXAnimation.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// ����������
//=============================================================================
HRESULT InitHadou(int type, HADOU *Hadou)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �|�C���^�̏�����
	Hadou->D3DTexture = NULL;
	Hadou->Mesh = NULL;
	Hadou->D3DXMatBuff = NULL;

	// �ꏊ�A�傫���̐ݒ�
	Hadou->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Hadou->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Hadou->scl = D3DXVECTOR3(HADOU_SIZE, HADOU_SIZE, HADOU_SIZE);
	Hadou->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (type == 0)
	{
		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(BALL_XFILE,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&Hadou->D3DXMatBuff,
			NULL,
			&Hadou->NumMat,
			&Hadou->Mesh)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitHadou(HADOU *Hadou)
{
	// �e�N�X�`���̊J��
	SAFE_RELEASE(Hadou->D3DTexture);

	// ���b�V���̊J��
	SAFE_RELEASE(Hadou->Mesh);

	// �}�e���A���̊J��
	SAFE_RELEASE(Hadou->D3DXMatBuff);

}

//=============================================================================
// �X�V���� �����FHadou:�U�����g���� DefendHadou:�h�䑤�g���� DefendAnimation:�h�䑤�A�j���[�V���� HitFrag:�g�����̃q�b�g�t���O
//=============================================================================
void UpdateHadou(HADOU *Hadou)
{
	if (Hadou->use == true)
	{
		// �ړ��v�Z
		Hadou->move.x -= sinf(Hadou->rot.y) * HADOU_SPEED_A;
		Hadou->move.z -= cosf(Hadou->rot.y) * HADOU_SPEED_A;

		Hadou->pos += Hadou->move;
		Hadou->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// ��苗���������������悤��

	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawHadou(HADOU *Hadou)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;

	if (Hadou->use == true)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&Hadou->mtxWorld);

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScl, Hadou->scl.x, Hadou->scl.y, Hadou->scl.z);
		D3DXMatrixMultiply(&Hadou->mtxWorld, &Hadou->mtxWorld, &mtxScl);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, Hadou->rot.y, Hadou->rot.x, Hadou->rot.z);
		D3DXMatrixMultiply(&Hadou->mtxWorld, &Hadou->mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, Hadou->pos.x, Hadou->pos.y, Hadou->pos.z);
		D3DXMatrixMultiply(&Hadou->mtxWorld, &Hadou->mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &Hadou->mtxWorld);

		// �}�e���A�����ɑ΂���|�C���^���擾
		pD3DXMat = (D3DXMATERIAL*)Hadou->D3DXMatBuff->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)Hadou->NumMat; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, Hadou->D3DTexture);

			// �`��
			Hadou->Mesh->DrawSubset(nCntMat);
		}

		{// �}�e���A�����f�t�H���g�ɖ߂�
			D3DXMATERIAL mat;

			mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			mat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			mat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			pDevice->SetMaterial(&mat.MatD3D);
		}
	}
}

//=============================================================================
// ���ˏ��� ���� Hadou:�g���� HitBall:�U�����̓����蔻��p�{�[�� CharaRot:�U�����L�����̉�]
//=============================================================================
void SetHadou(HADOU *Hadou, BALL *HitBall, D3DXVECTOR3 CharaRot)
{
	// �ꔭ���Ƃɂ����o���Ȃ�
	if (Hadou->use == false)
	{
		D3DXVECTOR3 FirePos = (HitBall[RightHand].pos + HitBall[LeftHand].pos) * 0.5f;	// ���ˈʒu�𗼎�̊Ԃɂ���
		Hadou->pos = FirePos;
		Hadou->rot = CharaRot;	// �������L�����N�^�ɍ��킹��
		Hadou->use = true;
	}
}