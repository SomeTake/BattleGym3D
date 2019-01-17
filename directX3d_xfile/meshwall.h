//=============================================================================
//
// ���b�V���ǂ̏��� [meshwall.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_MESHWALL		(10)							// �ǂ̑���
#define	TEXTURE_MESHWALL	"data/TEXTURE/wall000.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	VALUE_MOVE_MESHWALL		(5.0f)							// �ړ����x
#define	VALUE_ROTATE_MESHWALL	(D3DX_PI * 0.001f)				// ��]���x

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 pD3DIdxBuff;			// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

	D3DXMATRIX mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;							// �|���S���\���ʒu�̒��S���W
	D3DXVECTOR3 rot;							// �|���S���̉�]�p
	int nNumBlockX, nNumBlockY;					// �u���b�N��
	int nNumVertex;								// �����_��	
	int nNumVertexIndex;						// ���C���f�b�N�X��
	int nNumPolygon;							// ���|���S����
	float fBlockSizeX, fBlockSizeY;				// �u���b�N�T�C�Y
} MESH_WALL;

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
