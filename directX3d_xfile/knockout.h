//=============================================================================
//
// �m�b�N�A�E�g�\������ [knockout.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _KNOCKOUT_H_
#define _KNOCKOUT_H_


// �}�N����`
#define TEXTURE_GAME_KNOCKOUT00	_T("data/TEXTURE/knockout000.png")	// �T���v���p�摜
#define TEXTURE_KNOCKOUT00_SIZE_X	(1000)							// �e�N�X�`���T�C�Y
#define TEXTURE_KNOCKOUT00_SIZE_Y	(500)							// ����

#define TEXTURE_PATTERN_DIVIDE_KNOCKOUT_X	(1)						// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_KNOCKOUT_Y	(2)						// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_KNOCKOUT		(TEXTURE_PATTERN_DIVIDE_KNOCKOUT_X*TEXTURE_PATTERN_DIVIDE_KNOCKOUT_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_KNOCKOUT				(1)						// �A�j���[�V�����̐؂�ւ��J�E���g

#define KNOCKOUT_POS_X					(SCREEN_WIDTH / 2 - TEXTURE_KNOCKOUT00_SIZE_X / 2)						//�J�E���g�_�E���̕\���ʒu
#define KNOCKOUT_POS_Y					(SCREEN_HEIGHT / 2 - TEXTURE_KNOCKOUT00_SIZE_Y / 2)						//�J�E���g�_�E���̕\���ʒu


//�t���[���̍\����
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];				// ���_���i�[���[�N
	D3DXVECTOR3				pos;								// �|���S���̈ړ���
	int						CountAnim;							// �A�j���[�V�����J�E���g
	int						PatternAnim;						// �A�j���[�V�����p�^�[���i���o�[
	bool					use;								// �t���[����\�����邩�ǂ����̃t���O
}KNOCKOUT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitKnockout(int type);
void UninitKnockout(void);
void UpdateKnockout(void);
void DrawKnockout(void);

#endif