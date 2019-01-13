//=============================================================================
//
// �I����ʏ��� [thanks.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _THANKS_H_
#define _THANKS_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_THANKS	_T("data/TEXTURE/thanks000.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	THANKS_SIZE_X	(640)								// ��
#define	THANKS_SIZE_Y	(360)								// ����
#define	THANKS_POS_X	(SCREEN_WIDTH - THANKS_SIZE_X)		// �\���ʒu
#define	THANKS_POS_Y	(SCREEN_HEIGHT - THANKS_SIZE_Y)		// �\���ʒu

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitThanks(int type);
void UninitThanks(void);
void UpdateThanks(void);
void DrawThanks(void);
HRESULT MakeVertexThanks(void);
void SetReflectThanks(float per);	// ���ˌ��̐ݒ�

#endif
