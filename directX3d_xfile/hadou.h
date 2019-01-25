//=============================================================================
//
// �g�����p�o���b�g���� [hadou.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _HADOU_H_
#define _HADOU_H_

#include "main.h"
#include "battle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	HADOU_XFILE		"data/MODEL/Ball.x"					// �ǂݍ��ރ��f����
#define HADOU_SIZE		(8.0f)								// �g�����̃T�C�Y
#define HADOU_SPEED_A	(2.0f)								// �ړ��X�s�[�h
#define HADOU_SPEED_B	(3.0f)								// �ړ��X�s�[�h

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3			pos;			// ���݂̈ʒu
	D3DXVECTOR3			rot;			// ��]
	D3DXVECTOR3			scl;			// �傫��
	LPDIRECT3DTEXTURE9	D3DTexture;		// �e�N�X�`���ǂݍ��ݏꏊ
	LPD3DXMESH			Mesh;			// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
	LPD3DXBUFFER		D3DXMatBuff;	// ���b�V���̃}�e���A�������i�[
	DWORD				NumMat;			// �������̑���
	D3DXMATRIX			mtxWorld;		// ���[���h�}�g���b�N�X
	bool				use;			// �g�p���Ă��邩�ǂ���
	D3DXVECTOR3			move;			// �ړ���

} HADOU;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitHadou(int type, HADOU *Hadou);	// �������ABALL�\���́A�s��A���a
void UninitHadou(HADOU *Hadou);
void UpdateHadou(HADOU *Hadou);
void DrawHadou(HADOU *Hadou);
void SetHadou(HADOU *Hadou, BALL *HitBall, D3DXVECTOR3 CharaRot);

#endif
