//=============================================================================
//
// ���U���g��ʂ̃X�^�[�\������ [resultstar.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _RESULTSTAR_H_
#define _RESULTSTAR_H_


// �}�N����`
#define STAR00	_T("data/TEXTURE/star000.png")				// �X�^�[�p�摜
#define STAR_SIZE_X	(300)									// �e�N�X�`���T�C�Y
#define STAR_SIZE_Y	(300)									// ����

#define TEXTURE_PATTERN_DIVIDE_STAR_X	(1)					// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_STAR_Y	(1)					// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_STAR		(TEXTURE_PATTERN_DIVIDE_STAR_X*TEXTURE_PATTERN_DIVIDE_STAR_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_STAR				(4)					// �A�j���[�V�����̐؂�ւ��J�E���g

#define STAR_MAX			(1)								//�X�^�[�̐�

#define STAR_POS_X			(970)							// �\���ʒu
#define STAR_POS_Y			(400)							// �\���ʒu

//�o���b�g�̍\����
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// ���_���i�[���[�N
	D3DXVECTOR3				pos;							// �|���S���̈ړ���
	D3DXVECTOR3				rot;							// �|���S���̉�]��
	int						CountAnim;						// �A�j���[�V�����J�E���g
	int						PatternAnim;					// �A�j���[�V�����p�^�[���i���o�[
	bool					use;							// �g�p���t���O
	float					angle;							// ���S����̊p�x
	float					radius;							// ���S����̋���
}RESULTSTAR;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitResultstar(int type);
void UninitResultstar(void);
void UpdateResultstar(void);
void DrawResultstar(void);
HRESULT MakeVertexResultstar(void);
void SetTextureResultstar(int cntPattern);
void SetVertexResultstar(void);

#endif
