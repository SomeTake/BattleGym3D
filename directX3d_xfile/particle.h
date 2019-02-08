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
#define	TEXTURE_PARTICLE		"data/TEXTURE/effect000.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	PARTICLE_WIDTH			(10.0f)								// �؂̕�
#define	PARTICLE_HEIGHT			(10.0f)								// �؂̍���
#define	VALUE_MOVE_PARTICLE		(3.00f)								// �ړ����x
#define RAND_RANGE				(500)								// �����_���͈̔�
#define CIRCLE_RADIUS			(100.0f)							// �~�̔��a
#define	MAX_PARTICLE			(8192)								// �p�[�e�B�N���ő吔

// �����̍���
#define PARTICLE_SIZE_A			D3DXVECTOR2(10.0f, 10.0f)
#define PARTICLE_SIZE_B			D3DXVECTOR2(7.0f, 7.0f)
#define PARTICLE_SIZE_C			D3DXVECTOR2(3.0f, 3.0f)
#define PARTICLE_TIME_A			(20)
#define PARTICLE_TIME_B			(40)
#define PARTICLE_TIME_C			(60)

// �U����
#define ATTACK_MOVE				(1.0f)
#define ATTACK_SIZE_A			D3DXVECTOR2(5.0f, 5.0f)
#define ATTACK_SIZE_B			D3DXVECTOR2(3.0f, 3.0f)
#define ATTACK_SIZE_C			D3DXVECTOR2(1.0f, 1.0f)
#define ATTACK_TIME_A			(5)
#define ATTACK_TIME_B			(8)
#define ATTACK_TIME_C			(10)


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ��]
	D3DXVECTOR3 scl;		// �X�P�[��
	D3DXCOLOR col;			// �F
	D3DXVECTOR3 move;		// �ړ���
	float fWidth;			// ��
	float fHeight;			// ����
	int nIdxShadow;			// �eID
	bool bUse;				// �g�p���Ă��邩�ǂ���
	float DecAlpha;			// �����l
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
HRESULT MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice);
void SetVertexParticle(int nIdxParticle, float fWidth, float fHeight);
void SetColorParticle(int nIdxParticle, D3DXCOLOR col);

int SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fWidth, float fHeight, int Time);	// �p�[�e�B�N�������p(�ꏊ, �ړ���, �F, ��, ����, ����)
PARTICLE *GetParticle(int no);																				// �p�[�e�B�N���\���̂̃Q�b�^�[
int NumParticle(void);						// �g�p���Ă��鐔�𐔂���
void SetHitParticle(D3DXVECTOR3 AttackPos);	// �U���q�b�g���Ɏg�p����p�[�e�B�N���G�t�F�N�g

#endif
