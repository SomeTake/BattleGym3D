//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_PARTICLE			(8192)	// �p�[�e�B�N���ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 scl;		// �X�P�[��
	D3DXCOLOR col;			// �F
	float fWidth;			// ��
	float fHeight;			// ����
	int nIdxShadow;			// �eID
	bool bUse;				// �g�p���Ă��邩�ǂ���
	int time;				// �\�����鎞��
	float theta;			// �p�x��
} PARTICLE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitParticle(int type);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

int SetParticle(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col);

PARTICLE *GetParticle(int no);

#endif
