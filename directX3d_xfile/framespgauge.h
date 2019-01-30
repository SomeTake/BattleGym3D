//=============================================================================
//
// SP�Q�[�W�p�t���[���\������ [framespgauge.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _FRAMESPGAUGE_H_
#define _FRAMESPGAUGE_H_


// �}�N����`
#define TEXTURE_GAME_FRAMESPGAUGE00	_T("data/TEXTURE/framespgauge000.png")	// SP�Q�[�W�̃t���[���p�摜
#define TEXTURE_FRAMESPGAUGE00_SIZE_X	(1280)								// �e�N�X�`���T�C�Y
#define TEXTURE_FRAMESPGAUGE00_SIZE_Y	(720)								// ����

#define TEXTURE_PATTERN_DIVIDE_FRAMESPGAUGE_X	(1)							// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_FRAMESPGAUGE_Y	(1)							// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_FRAMESPGAUGE		(TEXTURE_PATTERN_DIVIDE_FRAMESPGAUGE_X*TEXTURE_PATTERN_DIVIDE_FRAMESPGAUGE_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_FRAMESPGAUGE				(1)							// �A�j���[�V�����̐؂�ւ��J�E���g

#define FRAMESPGAUGE_MAX						(1)							//�t���[���̐�

//�t���[���̍\����
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];							// ���_���i�[���[�N
	D3DXVECTOR3				pos;											// �|���S���̈ړ���
	int						CountAnim;										// �A�j���[�V�����J�E���g
	int						PatternAnim;									// �A�j���[�V�����p�^�[���i���o�[
	bool					use;											// �t���[����\�����邩�ǂ����̃t���O
}FRAMESPGAUGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitFrameSpgauge(int type);
void UninitFrameSpgauge(void);
void UpdateFrameSpgauge(void);
void DrawFrameSpgauge(void);
HRESULT MakeVertexFrameSpgauge(void);
void SetTextureFrameSpgauge(int cntPattern);
void SetVertexFrameSpgauge(void);

#endif