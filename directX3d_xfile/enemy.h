//=============================================================================
//
// �G�l�~�[���� [enemy.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "D3DXAnimation.h"
#include "CapsuleMesh.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_XFILE		"data/MODEL/Boy.x"				// �ǂݍ��ރ��f��
#define FIRST_ENEMY_POS	D3DXVECTOR3(-50.0f, 0.0f, 0.0f)	// �����ʒu

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
typedef struct {
	D3DXVECTOR3			pos;			// ���f���̈ʒu
	D3DXVECTOR3			move;			// ���f���̈ړ���
	D3DXVECTOR3			rot;			// ���݂̌���
	D3DXVECTOR3			scl;			// ���f���̑傫��(�X�P�[��)
	int					IdxShadow;		// �eID
	float				SizeShadow;		// �e�̃T�C�Y
	D3DXCOLOR			ColShadow;		// �e�̐F
	int					HP;				// �̗�
	int					HPzan;			// �c��̗�
	D3DXANIMATION		*Animation;		// �A�j���[�V����
	CAPSULE				HitCapsule;		// �����蔻��p�J�v�Z��
}ENEMY;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(int type);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY *GetEnemy(void);
void EasyInputEnemy(void);		// �ȒP����
void MoveEnemy(void);			// ���W�ړ�

#endif
