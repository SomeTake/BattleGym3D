//=============================================================================
//
// 2P�p�ԃQ�[�W��ʏ��� [eredguage.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _EREDGUAGE_H_
#define _EREDGUAGE_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_EREDGUAGE			_T("data/TEXTURE/ehpguage000.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	EREDGUAGE_POS_X				(680)									// �Q�[�W�̕\���ʒu
#define	EREDGUAGE_POS_Y				(50)									// �Q�[�W�̕\���ʒu
#define	EREDGUAGE_SIZE_X			(500)									// �Q�[�W�̕�
#define	EREDGUAGE_SIZE_Y			(50)									// �Q�[�W�̍���
#define EREDGUAGE_PATTERN_DIVIDE_X	(1)										// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define EREDGUAGE_PATTERN_DIVIDE_Y	(3)										// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_EREDGUAGE	(EREDGUAGE_PATTERN_DIVIDE_X*EREDGUAGE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_EREDGUAGE	(4)										// �A�j���[�V�����̐؂�ւ��J�E���g
#define EREDGUAGE_MAX				(1)										// �Q�[�W�̌�

//HP�Q�[�W�̍\����
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// ���_���i�[���[�N
	D3DXVECTOR3				pos;							// �|���S���̈ړ���
	int						CountAnim;						// �A�j���[�V�����J�E���g
	int						PatternAnim;					// �A�j���[�V�����p�^�[���i���o�[
	bool					use;							// �v���C���[�̎g�p����
	int						value;							// �ԃQ�[�W��HP�Q�[�W�ƍ��킹��Ƃ��ɑ���������
}EREDGUAGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitERedGuage(int type);
void UninitERedGuage(void);
void UpdateERedGuage(void);
void DrawERedGuage(void);
HRESULT MakeVertexERedGuage(void);
void SetTextureERedGuage(int cntPattern);
void SetVertexERedGuage(void);
EREDGUAGE *GetERedGuage(int eno);		// EREDGAUGE�\���̂̃Q�b�^�[

#endif
