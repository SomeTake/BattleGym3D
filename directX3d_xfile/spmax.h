//=============================================================================
//
// SP�Q�[�WMAX�\������ [spmax.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _SPMAX_H_
#define _SPMAX_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_SPMAX		_T("data/TEXTURE/max000.png")			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	SPMAX_POS_X			(180)									// �v���C���[��SP�Q�[�WMAX�\���ʒu
#define	SPMAX_POS_Y			(660)									// �v���C���[��SP�Q�[�WMAX�\���ʒu
#define	SPMAX_SIZE_X		(150)									// SP�Q�[�WMAX�̕�
#define	SPMAX_SIZE_Y		(50)									// SP�Q�[�WMAX�̍���
#define	ESPMAX_POS_X		(950)									// �v���C���[��SP�Q�[�WMAX�\���ʒu
#define	ESPMAX_POS_Y		(660)									// �v���C���[��SP�Q�[�WMAX�\���ʒu

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSpmax(int type);
void UninitSpmax(void);
void UpdateSpmax(void);
void DrawSpmax(void);

#endif
