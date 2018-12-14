//=============================================================================
//
// �^�C�}�[���� [timer.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_


// �}�N����`
#define TEXTURE_GAME_TIMER00	_T("data/TEXTURE/timer000.png")	// �T���v���p�摜
#define TEXTURE_TIMER00_SIZE_X	(35)							// �e�N�X�`���T�C�Y
#define TEXTURE_TIMER00_SIZE_Y	(70)							// ����

#define TEXTURE_PATTERN_DIVIDE_TIMER_X	(10)					// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_TIMER_Y	(1)						// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_TIMER		(TEXTURE_PATTERN_DIVIDE_TIMER_X*TEXTURE_PATTERN_DIVIDE_TIMER_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_TIMER		(1)							// �A�j���[�V�����̐؂�ւ��J�E���g

#define TIMER_MAX					(2)							//�^�C�}�[�̌���
#define FULL_TIMER					(99)						//�^�C�}�[�̍ő吔

#define TIMER_POS_X					(600)						//�^�C�}�[�̕\���ʒu
#define TIMER_POS_Y					(35)						//�^�C�}�[�̕\���ʒu

//�X�R�A�̍\����
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];				// ���_���i�[���[�N
	D3DXVECTOR3				pos;								// �|���S���̈ړ���
	int						CountAnim;							// �A�j���[�V�����J�E���g
	int						PatternAnim;						// �A�j���[�V�����p�^�[���i���o�[
	bool					use;								// �g�p���Ă��邩�ǂ����̃t���O
}TIMER;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTimer(int type);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);
TIMER *GetTimer(int tno);
HRESULT MakeVertexTimer(int tno);
void SetTextureTimer(int cntPattern, int tno);
void SetVertexTimer(int tno);

#endif
