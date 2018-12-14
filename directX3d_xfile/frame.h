//=============================================================================
//
// �o�g����ʃt���[���\������ [frame.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _FRAME_H_
#define _FRAME_H_

// �}�N����`
#define TEXTURE_GAME_FRAME00	_T("data/TEXTURE/frame000.png")	// �t���[���p�摜
#define TEXTURE_FRAME00_SIZE_X	(1280)							// �e�N�X�`���T�C�Y
#define TEXTURE_FRAME00_SIZE_Y	(720)							// ����

#define TEXTURE_PATTERN_DIVIDE_FRAME_X	(1)						// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_FRAME_Y	(1)						// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_FRAME		(TEXTURE_PATTERN_DIVIDE_FRAME_X*TEXTURE_PATTERN_DIVIDE_FRAME_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_FRAME		(1)							// �A�j���[�V�����̐؂�ւ��J�E���g

#define FRAME_MAX					(1)							//�t���[���̐�

#define SWAY_MAX					(20)						//�h�炷�t���[����

//�t���[���̍\����
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];				// ���_���i�[���[�N
	D3DXVECTOR3				pos;								// �|���S���̈ړ���
	int						CountAnim;							// �A�j���[�V�����J�E���g
	int						PatternAnim;						// �A�j���[�V�����p�^�[���i���o�[
	bool					use;								// �t���[����\�����邩�ǂ����̃t���O
	bool					sway;								// �t���[����h�炷���߂̃t���O
}FRAME;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitFrame(int type);
void UninitFrame(void);
void UpdateFrame(void);
void DrawFrame(void);
void FrameSway(void);					// �t���[����h�炷���߂̃t���O��ON�ɂ���֐�
HRESULT MakeVertexFrame(void);
void SetTextureFrame(int cntPattern);
void SetVertexFrame(void);

#endif