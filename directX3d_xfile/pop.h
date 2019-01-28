//=============================================================================
//
// 1P2P�\������ [pop.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _POP_H_
#define _POP_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_POP1P		"data/TEXTURE/pointer001.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_POP2P		"data/TEXTURE/pointer002.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	POP_WIDTH			(20.0f)							// ���a����
#define	POP_HEIGHT			(20.0f)							// ���a��
#define POP_POS				D3DXVECTOR3(0.0f, 75.0f, 0.0f)	// �\���ꏊ

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9		D3DTexture = {};		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 D3DVtxBuff = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	D3DXMATRIX				mtxWorld;				// ���[���h�}�g���b�N�X
	D3DXVECTOR3				pos;					// �ʒu
	D3DXVECTOR3				scl;					// �X�P�[��
	float					width;					// ��
	float					height;					// ����
} POP;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPop(int type, POP *PopWk, int ControllerNum);
void UninitPop(POP *PopWk);
void UpdatePop(POP *PopWk, D3DXVECTOR3 pos);
void DrawPop(POP *PopWk);
int SetPop(POP *PopWk, D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col);

#endif
