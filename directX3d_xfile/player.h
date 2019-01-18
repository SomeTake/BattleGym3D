//=============================================================================
//
// �v���C���[���� [player.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "D3DXAnimation.h"
#include "CapsuleMesh.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	PLAYER_XFILE		"data/MODEL/Boy.x"				// �ǂݍ��ރ��f����(�t�@�C���p�X��)
#define FIRST_PLAYER_POS	D3DXVECTOR3(50.0f, 0.0f, 0.0f)	// �����ʒu

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
	CAPSULE				HitCapsule;			// �����蔻��p�J�v�Z��
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
void HitCheckPlayer(void);		// �����蔻��

#endif
