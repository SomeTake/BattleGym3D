//=============================================================================
//
// �e���� [shadow.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_SHADOW		"data/TEXTURE/shadow000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	SHADOW_SIZE_X		(50.0f)							// �e�̕�
#define	SHADOW_SIZE_Z		(50.0f)							// �e�̍���

#define	MAX_SHADOW			(128)							// �e�ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ��]
	bool bUse;				// �g�p���Ă��邩�ǂ���
} SHADOW;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitShadow(int type);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
HRESULT MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);

int CreateShadow(D3DXVECTOR3 pos, float fSizeX, float fSizeZ);		// �e�̐�������
void ReleaseShadow(int nIdxShadow);									// �e�̏�������
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);			// �e�����ꏊ���w��
void SetVertexShadow(int nIdxShadow, float fSizeX, float fSizeZ);	// �e�̃T�C�Y���w��
void SetColorShadow(int nIdxShadow, D3DXCOLOR col);					// �e�̐F���w��

#endif
