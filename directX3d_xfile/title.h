//=============================================================================
//
// �^�C�g����ʏ��� [title.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_TITLE_LOGO	_T("data/TEXTURE/logo000.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TITLELOGO_POS_X		(0)								// �^�C�g�����S�̕\���ʒu
#define	TITLELOGO_POS_Y		(0)								// �^�C�g�����S�̕\���ʒu
#define	TITLELOGO_SIZE_X	(1280)							// �^�C�g�����S�̕�
#define	TITLELOGO_SIZE_Y	(720)							// �^�C�g�����S�̍���


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTitle(int type);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
HRESULT MakeVertexTitle(void);

#endif
