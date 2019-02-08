//=============================================================================
//
// ���̓��[�h�Z���N�g��ʏ��� [inputselect.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _INPUTSELECT_H_
#define _INPUTSELECT_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_INPUTSELECT	_T("data/TEXTURE/input000.png")					// �ǂݍ��ރe�N�X�`���t�@�C����
#define	INPUTSELECT_SIZE_X	(640)											// ��
#define	INPUTSELECT_SIZE_Y	(360)											// ����
#define	INPUTSELECT_POS_X	(SCREEN_WIDTH / 2 - INPUTSELECT_SIZE_X / 2)		// �\���ʒu
#define	INPUTSELECT_POS_Y	(SCREEN_HEIGHT / 2)								// �\���ʒu

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitInputselect(int type);
void UninitInputselect(void);
void UpdateInputselect(void);
void DrawInputselect(void);
HRESULT MakeVertexInputselect(void);

#endif
