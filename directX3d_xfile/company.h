//=============================================================================
//
// ��Ѓ��S��ʕ\������ [company.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _COMPANY_H_
#define _COMPANY_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_COMPANY	_T("data/TEXTURE/company000.png")			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	COMPANY_SIZE_X	(500)										// ��Ѓ��S�̕�
#define	COMPANY_SIZE_Y	(500)										// ��Ѓ��S�̍���
#define	COMPANY_POS_X	(SCREEN_WIDTH / 2 - COMPANY_SIZE_X / 2)		// ��Ѓ��S�̕\���ʒu
#define	COMPANY_POS_Y	(SCREEN_HEIGHT / 2 - COMPANY_SIZE_Y / 2)	// ��Ѓ��S�̕\���ʒu

#define CLEAR_DOWN		(100)										// �����x�������鎞��
#define CLEAR_NONE		(200)										// 100%�\������Ă��鎞��
#define CLEAR_UP		(300)										// �����x���グ�鎞��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitCompany(int type);
void UninitCompany(void);
void UpdateCompany(void);
void DrawCompany(void);

#endif
