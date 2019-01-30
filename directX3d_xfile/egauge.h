//=============================================================================
//
// 2P�pHP�Q�[�W��ʏ��� [egauge.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _EGAUGE_H_
#define _EGAUGE_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_EGAUGE			_T("data/TEXTURE/ehpgauge000.png")			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	EGAUGE_POS_X			(680)										// �Q�[�W�̕\���ʒu
#define	EGAUGE_POS_Y			(50)										// �Q�[�W�̕\���ʒu
#define	EGAUGE_SIZE_X			(500)										// �Q�[�W�̕�
#define	EGAUGE_SIZE_Y			(50)										// �Q�[�W�̍���
#define EGAUGE_PATTERN_DIVIDE_X	(1)											// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define EGAUGE_PATTERN_DIVIDE_Y	(3)											// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_EGAUGE	(EGAUGE_PATTERN_DIVIDE_X*EGAUGE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_EGAUGE	(4)											// �A�j���[�V�����̐؂�ւ��J�E���g
#define EGAUGE_MAX				(1)											// �Q�[�W�̌�

//HP�Q�[�W�̍\����
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// ���_���i�[���[�N
	D3DXVECTOR3				pos;							// �|���S���̈ړ���
	int						CountAnim;						// �A�j���[�V�����J�E���g
	int						PatternAnim;					// �A�j���[�V�����p�^�[���i���o�[
	bool					use;							// �v���C���[�̎g�p����
}EGAUGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEGauge(int type);
void UninitEGauge(void);
void UpdateEGauge(void);
void DrawEGauge(void);
HRESULT MakeVertexEGauge(void);
void SetTextureEGauge(int cntPattern);
void SetVertexEGauge(void);

#endif
