//=============================================================================
//
// �E�B�i�[�\������ [winner.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _WINNER_H_
#define _WINNER_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_WINNER		_T("data/TEXTURE/win000.png")			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	WINNER_POS_X		(750)									// �E�B�i�[�̕\���ʒu
#define WINNER_POS_Y		(70)									// �E�B�i�[�̕\���ʒu
#define	WINNER_SIZE_X		(200)									// �E�B�i�[�̕�
#define	WINNER_SIZE_Y		(300)									// �E�B�i�[�̍���

#define TEXTURE_PATTERN_WINNER_X	(1)							// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_WINNER_Y	(2)							// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_WINNER		(TEXTURE_PATTERN_WINNER_X*TEXTURE_PATTERN_WINNER_Y)	// �A�j���[�V�����p�^�[����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitWinner(int type);
void UninitWinner(void);
void UpdateWinner(void);
void DrawWinner(void);

#endif
