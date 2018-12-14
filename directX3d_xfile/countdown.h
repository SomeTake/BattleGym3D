//=============================================================================
//
// �o�g���O�̃J�E���g�_�E������ [countdown.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _COUNTDOWN_H_
#define _COUNTDOWN_H_


// �}�N����`
#define TEXTURE_GAME_COUNTDOWN00	_T("data/TEXTURE/countdown000.png")	// �T���v���p�摜
#define TEXTURE_COUNTDOWN00_SIZE_X	(1000)							// �e�N�X�`���T�C�Y
#define TEXTURE_COUNTDOWN00_SIZE_Y	(250)							// ����

#define TEXTURE_PATTERN_DIVIDE_COUNTDOWN_X	(1)					// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_COUNTDOWN_Y	(4)						// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_COUNTDOWN		(TEXTURE_PATTERN_DIVIDE_COUNTDOWN_X*TEXTURE_PATTERN_DIVIDE_COUNTDOWN_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_COUNTDOWN		(1)							// �A�j���[�V�����̐؂�ւ��J�E���g

#define COUNTDOWN_MAX					(1)							//�J�E���g�_�E���̌���
#define FULL_COUNTDOWN					(4)							//�J�E���g�_�E���̍ő吔

#define COUNTDOWN_POS_X					(SCREEN_WIDTH / 2 - TEXTURE_COUNTDOWN00_SIZE_X / 2)						//�J�E���g�_�E���̕\���ʒu
#define COUNTDOWN_POS_Y					(SCREEN_HEIGHT / 2 - TEXTURE_COUNTDOWN00_SIZE_Y / 2)					//�J�E���g�_�E���̕\���ʒu

//�X�R�A�̍\����
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];				// ���_���i�[���[�N
	D3DXVECTOR3				pos;								// �|���S���̈ړ���
	int						CountAnim;							// �A�j���[�V�����J�E���g
	int						PatternAnim;						// �A�j���[�V�����p�^�[���i���o�[
	bool					use;								// �g�p���Ă��邩�ǂ����̃t���O
}COUNTDOWN;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitCountdown(int type);
void UninitCountdown(void);
void UpdateCountdown(void);
void DrawCountdown(void);
HRESULT MakeVertexCountdown(void);
void SetTextureCountdown(int cntPattern);
void SetVertexCountdown(void);

#endif
