//=============================================================================
//
// 2P�pSP�Q�[�W��ʏ��� [espgauge.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _ESPGAUGE_H_
#define _ESPGAUGE_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_ESPGAUGE		_T("data/TEXTURE/spgauge000.png")			// �G�l�~�[�pSP�Q�[�W�̉摜
#define	ESPGAUGE_POS_X			(880)										// �Q�[�W�̕\���ʒu
#define	ESPGAUGE_POS_Y			(675)										// �Q�[�W�̕\���ʒu
#define	ESPGAUGE_SIZE_X			(300)										// �Q�[�W�̕�
#define	ESPGAUGE_SIZE_Y			(30)										// �Q�[�W�̍���
#define ESPGAUGE_PATTERN_DIVIDE_X	(1)										// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define ESPGAUGE_PATTERN_DIVIDE_Y	(1)										// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_ESPGAUGE	(ESPGAUGE_PATTERN_DIVIDE_X*ESPGAUGE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_ESPGAUGE		(4)										// �A�j���[�V�����̐؂�ւ��J�E���g
#define ESPGAUGE_MAX				(1)										// �Q�[�W�̌�

//SP�Q�[�W�̍\����
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// ���_���i�[���[�N
	D3DXVECTOR3				pos;							// �|���S���̈ړ���
	int						CountAnim;						// �A�j���[�V�����J�E���g
	int						PatternAnim;					// �A�j���[�V�����p�^�[���i���o�[
	bool					use;							// �v���C���[�̎g�p����
}ESPGAUGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitESpGauge(int type);
void UninitESpGauge(void);
void UpdateESpGauge(void);
void DrawESpGauge(void);
HRESULT MakeVertexESpGauge(void);
void SetTextureESpGauge(int cntPattern);
void SetVertexESpGauge(void);
ESPGAUGE *GetESpGauge(int gno);		// ESPGAUGE�\���̂̃Q�b�^�[

#endif
