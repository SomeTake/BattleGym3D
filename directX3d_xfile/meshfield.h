//=============================================================================
//
// ���b�V���n�ʂ̏��� [meshfield.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"

#define	TEXTURE_MESHFIELD	"data/TEXTURE/field004.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
						int nNumBlockX, int nNumBlockZ, float nBlockSizeX, float nBlockSizeZ, int type);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);

#endif
