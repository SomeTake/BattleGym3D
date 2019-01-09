//=============================================================================
//
// �G�l�~�[���� [enemy.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "D3DXAnimation.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_XFILE		"data/MODEL/Boy.x"

#define ENEMY_JUMP_SPEED	(10.0f)	//�W�����v�̏���

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
typedef struct {
	D3DXVECTOR3			pos;			// ���f���̈ʒu
	D3DXVECTOR3			move;			// ���f���̈ړ���
	D3DXVECTOR3			rot;			// ���݂̌���
	D3DXVECTOR3			rotDest;		// �ړI�̌���
	D3DXVECTOR3			scl;			// ���f���̑傫��(�X�P�[��)
	int					IdxShadow;		// �eID
	float				SizeShadow;		// �e�̃T�C�Y
	D3DXCOLOR			ColShadow;		// �e�̐F
	bool				jump;			// �W�����v�t���O
	float				speed;			// �W�����v�̃X�s�[�h
	int					HP;				// �̗�
	int					HPzan;			// �c��̗�

	D3DXANIMATION		*Animation;			// �A�j���[�V����
	int					Action;				// ���݂̃A�j���[�V����
	int					NextAction;			// ���̃A�j���[�V����
}ENEMY;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(int type);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY *GetEnemy(void);

#endif
