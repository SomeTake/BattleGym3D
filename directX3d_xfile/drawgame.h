//=============================================================================
//
// ����������ʏ��� [drawgame.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _DRAWGAME_H_
#define _DRAWGAME_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_DRAWGAME		_T("data/TEXTURE/drawgame000.png")		// ����������ʗp�̉摜
#define	DRAWGAME_POS_X			(0)										// ����������ʂ̕\���ʒu
#define	DRAWGAME_POS_Y			(0)										// ����������ʂ̕\���ʒu
#define	DRAWGAME_SIZE_X		(1280)										// ����������ʂ̕�
#define	DRAWGAME_SIZE_Y		(720)										// ����������ʂ̍���

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitDrawgame(int type);
void UninitDrawgame(void);
void UpdateDrawgame(void);
void DrawDrawgame(void);
HRESULT MakeVertexDrawgame(void);

#endif
