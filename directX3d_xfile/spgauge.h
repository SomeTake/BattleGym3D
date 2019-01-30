//=============================================================================
//
// SP�Q�[�W��ʏ��� [spgauge.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _SPGAUGE_H_
#define _SPGAUGE_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_SPGAUGE			_T("data/TEXTURE/spgauge000.png")			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	SPGAUGE_POS_X			(100)										// �Q�[�W�̕\���ʒu
#define	SPGAUGE_POS_Y			(675)										// �Q�[�W�̕\���ʒu
#define	SPGAUGE_SIZE_X			(300)										// �Q�[�W�̕�
#define	SPGAUGE_SIZE_Y			(30)										// �Q�[�W�̍���
#define SPGAUGE_PATTERN_DIVIDE_X	(1)										// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define SPGAUGE_PATTERN_DIVIDE_Y	(1)										// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_SPGAUGE	(SPGAUGE_PATTERN_DIVIDE_X*SPGAUGE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_SPGAUGE	(4)										// �A�j���[�V�����̐؂�ւ��J�E���g
#define SPGAUGE_MAX				(1)										// �Q�[�W�̌�

//SP�Q�[�W�̍\����
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// ���_���i�[���[�N
	D3DXVECTOR3				pos;							// �|���S���̈ړ���
	int						CountAnim;						// �A�j���[�V�����J�E���g
	int						PatternAnim;					// �A�j���[�V�����p�^�[���i���o�[
	bool					use;							// �v���C���[�̎g�p����
}SPGAUGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSpGauge(int type);
void UninitSpGauge(void);
void UpdateSpGauge(void);
void DrawSpGauge(void);
HRESULT MakeVertexSpGauge(void);
void SetTextureSpGauge(int cntPattern);
void SetVertexSpGauge(void);
SPGAUGE *GetSpGauge(int gno);				// SPGAUGE�\���̂̃Q�b�^�[

#endif
