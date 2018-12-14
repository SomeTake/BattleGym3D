//=============================================================================
//
// �X�R�A�]������ [evaluation.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _EVALUATION_H_
#define _EVALUATION_H_

// �}�N����`
#define TEXTURE_GAME_EVALUATION		_T("data/TEXTURE/evaluation000.png")	// �摜
#define TEXTURE_EVALUATION_SIZE_X	(150)			// �e�N�X�`���T�C�Y
#define TEXTURE_EVALUATION_SIZE_Y	(150)			// ����
#define TEXTURE_PATTERN_DIVIDE_X_EVALUATION	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_EVALUATION	(3)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_EVALUATION			(TEXTURE_PATTERN_DIVIDE_X_EVALUATION*TEXTURE_PATTERN_DIVIDE_Y_EVALUATION)	// �A�j���[�V�����p�^�[����
#define EVALUATION_MAX						(1)		// �]���̐�
#define EVALUATION_POS_X					(1040)	//�\���ʒu
#define EVALUATION_POS_Y					(490)	//�\���ʒu

//�X�R�A�]��
#define SCORE_A								(1500)
#define SCORE_B								(1000)

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

} EVALUATION;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEvaluation(int type);
void UninitEvaluation(void);
void UpdateEvaluation(void);
void DrawEvaluation(void);
HRESULT MakeVertexEvaluation(void);
void SetTextureEvaluation(int cntPattern);
void SetVertexEvaluation(void);

#endif
