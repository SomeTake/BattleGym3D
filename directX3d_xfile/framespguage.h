//=============================================================================
//
// SP�Q�[�W�p�t���[���\������ [framespguage.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _FRAMESPGUAGE_H_
#define _FRAMESPGUAGE_H_


// �}�N����`
#define TEXTURE_GAME_FRAMESPGUAGE00	_T("data/TEXTURE/framespguage000.png")	// �T���v���p�摜
#define TEXTURE_FRAMESPGUAGE00_SIZE_X	(1280)								// �e�N�X�`���T�C�Y
#define TEXTURE_FRAMESPGUAGE00_SIZE_Y	(720)								// ����

#define TEXTURE_PATTERN_DIVIDE_FRAMESPGUAGE_X	(1)							// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_FRAMESPGUAGE_Y	(1)							// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_FRAMESPGUAGE		(TEXTURE_PATTERN_DIVIDE_FRAMESPGUAGE_X*TEXTURE_PATTERN_DIVIDE_FRAMESPGUAGE_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_FRAMESPGUAGE		(1)									// �A�j���[�V�����̐؂�ւ��J�E���g

#define FRAMESPGUAGE_MAX					(1)								//�t���[���̐�

//�t���[���̍\����
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];							// ���_���i�[���[�N
	D3DXVECTOR3				pos;											// �|���S���̈ړ���
	int						CountAnim;										// �A�j���[�V�����J�E���g
	int						PatternAnim;									// �A�j���[�V�����p�^�[���i���o�[
	bool					use;											// �t���[����\�����邩�ǂ����̃t���O
}FRAMESPGUAGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitFrameSpguage(int type);
void UninitFrameSpguage(void);
void UpdateFrameSpguage(void);
void DrawFrameSpguage(void);

#endif