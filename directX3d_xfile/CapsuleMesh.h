//=============================================================================
//
// �����蔻��p�J�v�Z������ [CapsuleMesh.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _CAPSULEMESH_H_
#define _CAPSULEMESH_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SPHERE_SLICE	(8)		// �J�v�Z���������̃X���C�X��(�~�����̕������ɂ��Ȃ�)
#define SPHERE_STACK	(4)		// �J�v�Z���������̃X�^�b�N��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 VtxBuffer;			// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	IdxBuffer;			// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				WorldMatrix;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3				P1, P2;				// ���ƉE����̔����̂̒��S���W
	D3DXVECTOR3				pos;				// �J�v�Z���̍��W
	D3DXVECTOR3				vec;				// �`�悷��Ƃ��̕����x�N�g��
	D3DXVECTOR3				CreateVec;			// �J�v�Z�����쐬����Ƃ��ݒu����x�N�g��
												// �Ⴆ�΁ACreate = (0.0f,1.0f,0.0f)�̂Ƃ�
												// �J�v�Z������]�����Ȃ��ꍇ�A�J�v�Z���͐^��Ɍ���
	int						NumOfVtxBuffer;
	int						NumOfTriangle;
	float					Length;				// �J�v�Z���̒���
	float					Radius;				// �J�v�Z���̔��a
} CAPSULE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
//HRESULT InitCapsule(void);
HRESULT CreateCapsule(CAPSULE *Capsule, D3DXVECTOR3 Pos, D3DXVECTOR3 CreateVec, float Length, float Radius, bool DrawInCenter);
void UninitCapsule(CAPSULE *Capsule);
void UpdateCapsule(CAPSULE *Capsule);
void DrawCapsule(CAPSULE *Capsule,const D3DXMATRIX *WorldMatrix);

#endif
