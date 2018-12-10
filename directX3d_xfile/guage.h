//=============================================================================
//
// HP�Q�[�W��ʏ��� [guage.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _GUAGE_H_
#define _GUAGE_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_GUAGE			_T("data/TEXTURE/hpguage000.png")			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	GUAGE_POS_X				(100)										// �Q�[�W�̕\���ʒu
#define	GUAGE_POS_Y				(50)										// �Q�[�W�̕\���ʒu
#define	GUAGE_SIZE_X			(500)									// �Q�[�W�̕�
#define	GUAGE_SIZE_Y			(50)									// �Q�[�W�̍���
#define GUAGE_PATTERN_DIVIDE_X	(1)										// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define GUAGE_PATTERN_DIVIDE_Y	(3)										// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_GUAGE	(GUAGE_PATTERN_DIVIDE_X*GUAGE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_GUAGE	(4)										// �A�j���[�V�����̐؂�ւ��J�E���g
#define GUAGE_MAX				(1)										// �Q�[�W�̌�

//HP�Q�[�W�̍\����
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// ���_���i�[���[�N
	D3DXVECTOR3				pos;							// �|���S���̈ړ���
	int						CountAnim;						// �A�j���[�V�����J�E���g
	int						PatternAnim;					// �A�j���[�V�����p�^�[���i���o�[
	bool					use;							// �v���C���[�̎g�p����
}GUAGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGuage(int type);
void UninitGuage(void);
void UpdateGuage(void);
void DrawGuage(void);

#endif
