//=============================================================================
//
// 2P�p�ԃQ�[�W��ʏ��� [eredgauge.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _EREDGAUGE_H_
#define _EREDGAUGE_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_EREDGAUGE			_T("data/TEXTURE/ehpgauge000.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	EREDGAUGE_POS_X				(680)									// �Q�[�W�̕\���ʒu
#define	EREDGAUGE_POS_Y				(50)									// �Q�[�W�̕\���ʒu
#define	EREDGAUGE_SIZE_X			(500)									// �Q�[�W�̕�
#define	EREDGAUGE_SIZE_Y			(50)									// �Q�[�W�̍���
#define EREDGAUGE_PATTERN_DIVIDE_X	(1)										// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define EREDGAUGE_PATTERN_DIVIDE_Y	(3)										// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_EREDGAUGE	(EREDGAUGE_PATTERN_DIVIDE_X*EREDGAUGE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_EREDGAUGE	(4)										// �A�j���[�V�����̐؂�ւ��J�E���g
#define EREDGAUGE_MAX				(1)										// �Q�[�W�̌�

//HP�Q�[�W�̍\����
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// ���_���i�[���[�N
	D3DXVECTOR3				pos;							// �|���S���̈ړ���
	int						CountAnim;						// �A�j���[�V�����J�E���g
	int						PatternAnim;					// �A�j���[�V�����p�^�[���i���o�[
	bool					use;							// �v���C���[�̎g�p����
	int						value;							// �ԃQ�[�W��HP�Q�[�W�ƍ��킹��Ƃ��ɑ���������
}EREDGAUGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitERedGauge(int type);
void UninitERedGauge(void);
void UpdateERedGauge(void);
void DrawERedGauge(void);
HRESULT MakeVertexERedGauge(void);
void SetTextureERedGauge(int cntPattern);
void SetVertexERedGauge(void);
EREDGAUGE *GetERedGauge(int eno);		// EREDGAUGE�\���̂̃Q�b�^�[

#endif
