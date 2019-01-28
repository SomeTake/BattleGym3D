//=============================================================================
//
// �X�R�A���� [score.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_


// �}�N����`
#define TEXTURE_GAME_SCORE00	("data/TEXTURE/score000.png")	// �X�R�A�摜

//PhaseGame,PhaseCountdown,PhaseFinish�̎�
#define TEXTURE_SCORE00_SIZE_X			(20)					// �e�N�X�`���T�C�Y
#define TEXTURE_SCORE00_SIZE_Y			(30)					// ����

//PhaseResult�̎�
#define RESULT_SCORE00_SIZE_X			(100)					// �e�N�X�`���T�C�Y
#define RESULT_SCORE00_SIZE_Y			(150)					// ����

#define TEXTURE_PATTERN_DIVIDE_SCORE_X	(10)					// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_SCORE_Y	(1)						// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_SCORE		(TEXTURE_PATTERN_DIVIDE_SCORE_X*TEXTURE_PATTERN_DIVIDE_SCORE_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_SCORE			(1)						// �A�j���[�V�����̐؂�ւ��J�E���g

#define SCORE_MAX						(5)						//�X�R�A�̌���

//PhaseGame,PhaseCountdown,PhaseFinish�̎�
#define SCORE_POS_X						(500)					//�X�R�A�̕\���ʒu
#define SCORE_POS_Y						(18)					//�X�R�A�̕\���ʒu

//PhaseResult�̎�
#define RESULT_SCORE_POS_X				(600)					//�X�R�A�̕\���ʒu
#define RESULT_SCORE_POS_Y				(400)					//�X�R�A�̕\���ʒu

//�X�R�A�̍\����
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];				// ���_���i�[���[�N
	D3DXVECTOR3				pos;								// �|���S���̈ړ���
	int						CountAnim;							// �A�j���[�V�����J�E���g
	int						PatternAnim;						// �A�j���[�V�����p�^�[���i���o�[
	bool					use;								// �g�p�����ǂ����̃t���O
}SCORE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitScore(int type);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
HRESULT MakeVertexScore(int sno);
void SetTextureScore(int cntPattern, int sno);
void SetVertexScore(int sno);
SCORE *GetScore(int bno);			// �X�R�A�\���̂̎擾

#endif
