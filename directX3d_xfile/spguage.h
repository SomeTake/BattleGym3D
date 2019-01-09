//=============================================================================
//
// SP�Q�[�W��ʏ��� [spguage.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _SPGUAGE_H_
#define _SPGUAGE_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_SPGUAGE			_T("data/TEXTURE/spguage000.png")			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	SPGUAGE_POS_X			(100)										// �Q�[�W�̕\���ʒu
#define	SPGUAGE_POS_Y			(675)										// �Q�[�W�̕\���ʒu
#define	SPGUAGE_SIZE_X			(300)										// �Q�[�W�̕�
#define	SPGUAGE_SIZE_Y			(30)										// �Q�[�W�̍���
#define SPGUAGE_PATTERN_DIVIDE_X	(1)										// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define SPGUAGE_PATTERN_DIVIDE_Y	(1)										// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_SPGUAGE	(SPGUAGE_PATTERN_DIVIDE_X*SPGUAGE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_SPGUAGE	(4)										// �A�j���[�V�����̐؂�ւ��J�E���g
#define SPGUAGE_MAX				(1)										// �Q�[�W�̌�

//SP�Q�[�W�̍\����
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// ���_���i�[���[�N
	D3DXVECTOR3				pos;							// �|���S���̈ړ���
	int						CountAnim;						// �A�j���[�V�����J�E���g
	int						PatternAnim;					// �A�j���[�V�����p�^�[���i���o�[
	int						sp;								// SP�Q�[�W�̌��݂̒l
	bool					use;							// �v���C���[�̎g�p����
}SPGUAGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSpGuage(int type);
void UninitSpGuage(void);
void UpdateSpGuage(void);
void DrawSpGuage(void);
HRESULT MakeVertexSpGuage(void);
void SetTextureSpGuage(int cntPattern);
void SetVertexSpGuage(void);
SPGUAGE *GetSpGuage(int gno);				// SPGAUGE�\���̂̃Q�b�^�[
void AddSpGuage(int add);					// SP�Q�[�W�𑝂₷�Ƃ��Ɏg�p

#endif
