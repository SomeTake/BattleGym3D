//=============================================================================
//
// �v���C���[�p�ԃQ�[�W��ʏ��� [redgauge.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _REDGAUGE_H_
#define _REDGAUGE_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_REDGAUGE			_T("data/TEXTURE/hpgauge000.png")		// �ԃQ�[�W�p�摜
#define	REDGAUGE_POS_X				(100)									// �ԃQ�[�W�̕\���ʒu
#define	REDGAUGE_POS_Y				(50)									// �ԃQ�[�W�̕\���ʒu
#define	REDGAUGE_SIZE_X				(500)									// �ԃQ�[�W�̕�
#define	REDGAUGE_SIZE_Y				(50)									// �ԃQ�[�W�̍���
#define REDGAUGE_PATTERN_DIVIDE_X	(1)										// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define REDGAUGE_PATTERN_DIVIDE_Y	(3)										// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_REDGAUGE	(REDGAUGE_PATTERN_DIVIDE_X*REDGAUGE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_REDGAUGE		(4)										// �A�j���[�V�����̐؂�ւ��J�E���g
#define REDGAUGE_MAX				(1)										// �ԃQ�[�W�̌�

//HP�Q�[�W�̍\����
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// ���_���i�[���[�N
	D3DXVECTOR3				pos;							// �|���S���̈ړ���
	int						CountAnim;						// �A�j���[�V�����J�E���g
	int						PatternAnim;					// �A�j���[�V�����p�^�[���i���o�[
	bool					use;							// �v���C���[�̎g�p����
	int						value;							// �ԃQ�[�W��HP�Q�[�W�ƍ��킹��Ƃ��ɑ���������
}REDGAUGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitRedGauge(int type);
void UninitRedGauge(void);
void UpdateRedGauge(void);
void DrawRedGauge(void);
HRESULT MakeVertexRedGauge(void);
void SetTextureRedGauge(int cntPattern);
void SetVertexRedGauge(void);
REDGAUGE *GetRedGauge(void);

#endif
