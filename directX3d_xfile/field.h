//=============================================================================
//
// �n�ʏ��� [field.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"
#define	TEXTURE_FIELD	"data/TEXTURE/field000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitField(void);
void UninitField(void);
void UpdateField(void);
void DrawField(void);
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);

#endif
