//=============================================================================
//
// ����ʏ��� [blackscreen.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _BLACKSCREEN_H_
#define _BLACKSCREEN_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BLACKSCREEN	_T("data/TEXTURE/blackscreen000.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	BLACKSCREEN_POS_X	(0)								// �^�C�g�����S�̕\���ʒu
#define	BLACKSCREEN_POS_Y	(0)								// �^�C�g�����S�̕\���ʒu
#define	BLACKSCREEN_SIZE_X	(1280)							// �^�C�g�����S�̕�
#define	BLACKSCREEN_SIZE_Y	(720)							// �^�C�g�����S�̍���

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBlackscreen(int type);
void UninitBlackscreen(void);
void UpdateBlackscreen(void);
void DrawBlackscreen(void);

#endif
