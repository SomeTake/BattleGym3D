//=============================================================================
//
// �v���C���[���� [player.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "D3DXAnimation.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	PLAYER_XFILE		"data/MODEL/Boy.x"			// �ǂݍ��ރ��f����(�t�@�C���p�X��)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �v���C���[�̃f�[�^���Ǘ�����\����
typedef struct {	
	D3DXVECTOR3			pos;				// ���f���̈ʒu
	D3DXVECTOR3			move;				// ���f���̈ړ���
	D3DXVECTOR3			rot;				// ���݂̌���
	D3DXVECTOR3			scl;				// ���f���̑傫��(�X�P�[��)
	int					IdxShadow;			// �eID
	float				SizeShadow;			// �e�̃T�C�Y
	D3DXCOLOR			ColShadow;			// �e�̐F
	int					HP;					// �̗�
	int					HPzan;				// �c��̗�

	D3DXANIMATION		*Animation;			// �A�j���[�V����
	int					Action;				// ���݂̃A�j���[�V����
	int					NextAction;			// ���̃A�j���[�V����
}PLAYER;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(int type);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);
void EasyInputPlayer(void);		// �ȒP����
void MovePlayer(void);			// ���W�ړ�

#endif
