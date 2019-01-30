//=============================================================================
//
// HP�Q�[�W��ʏ��� [gauge.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_GAUGE			_T("data/TEXTURE/hpgauge000.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	GAUGE_POS_X				(100)									// �Q�[�W�̕\���ʒu
#define	GAUGE_POS_Y				(50)									// �Q�[�W�̕\���ʒu
#define	GAUGE_SIZE_X			(500)									// �Q�[�W�̕�
#define	GAUGE_SIZE_Y			(50)									// �Q�[�W�̍���
#define GAUGE_PATTERN_DIVIDE_X	(1)										// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define GAUGE_PATTERN_DIVIDE_Y	(3)										// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_GAUGE	(GAUGE_PATTERN_DIVIDE_X*GAUGE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_GAUGE	(4)										// �A�j���[�V�����̐؂�ւ��J�E���g
#define GAUGE_MAX				(1)										// �Q�[�W�̌�

//HP�Q�[�W�̍\����
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// ���_���i�[���[�N
	D3DXVECTOR3				pos;							// �|���S���̈ړ���
	int						CountAnim;						// �A�j���[�V�����J�E���g
	int						PatternAnim;					// �A�j���[�V�����p�^�[���i���o�[
	bool					use;							// �v���C���[�̎g�p����
}GAUGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGauge(int type);
void UninitGauge(void);
void UpdateGauge(void);
void DrawGauge(void);
HRESULT MakeVertexGauge(void);
void SetTextureGauge(int cntPattern);
void SetVertexGauge(void);

#endif
