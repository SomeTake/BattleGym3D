//=============================================================================
//
// ���v���C��ʏ��� [replay.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _REPLAY_H_
#define _REPLAY_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_REPLAY		_T("data/TEXTURE/replay000.png")		// �摜
#define	REPLAY_SIZE_X		(800)									// ��
#define	REPLAY_SIZE_Y		(500)									// ����
#define	REPLAY_POS_X		(SCREEN_WIDTH / 2 - REPLAY_SIZE_X / 2)	// �\���ʒu
#define	REPLAY_POS_Y		(SCREEN_HEIGHT / 2 - REPLAY_SIZE_Y / 2)	// �\���ʒu

#define FLASHING_TIME		(20)									// �_�ł���t���[����
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitReplay(int type);
void UninitReplay(void);
void UpdateReplay(void);
void DrawReplay(void);
HRESULT MakeVertexReplay(void);
void SetReflectReplay(float per);

#endif
