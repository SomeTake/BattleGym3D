//=============================================================================
//
// ���b�V���ǂ̏��� [meshwall.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"

#define	MAX_MESHWALL		(10)							// �ǂ̑���
#define	TEXTURE_MESHWALL	"data/TEXTURE/wall000.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	VALUE_MOVE_MESHWALL		(5.0f)							// �ړ����x
#define	VALUE_ROTATE_MESHWALL	(D3DX_PI * 0.001f)				// ��]���x

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
						int nNumBlockX, int nNumBlockY, float fSizeBlockX, float fSizeBlockY);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);
int hitCheckMeshwall(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);

#endif
