//=============================================================================
//
// �`���[�g���A���\������ [tutorial.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

// �}�N����`
#define TEXTURE_GAME_TUTORIAL		_T("data/TEXTURE/tutorial000.png")	// �摜
#define TEXTURE_TUTORIAL_SIZE_X	(1280)			// �e�N�X�`���T�C�Y
#define TEXTURE_TUTORIAL_SIZE_Y	(720)			// ����

#define TEXTURE_PATTERN_DIVIDE_X_TUTORIAL	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_TUTORIAL	(2)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_TUTORIAL			(TEXTURE_PATTERN_DIVIDE_X_TUTORIAL*TEXTURE_PATTERN_DIVIDE_Y_TUTORIAL)	// �A�j���[�V�����p�^�[����

#define TUTORIAL_MAX						(1)		// �`���[�g���A���̐�

#define TUTORIAL_POS_X						(0)		// �\���ʒu
#define TUTORIAL_POS_Y						(0)		// �\���ʒu

//*****************************************************************************
// �\���̐錾
//*****************************************************************************

typedef struct	// �G�l�~�[�\����
{
	D3DXVECTOR3		pos;						// �|���S���̈ړ���
	int				PatternAnim;				// �A�j���[�V�����p�^�[���i���o�[
	int				CountAnim;					// �A�j���[�V�����J�E���g
	LPDIRECT3DTEXTURE9	Texture;				// �e�N�X�`�����
	VERTEX_2D		vertexWk[NUM_VERTEX];		// ���_���i�[���[�N

} TUTORIAL;



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTutorial(int type);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);
TUTORIAL *GetTutorial(int no);


#endif
