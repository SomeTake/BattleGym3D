//=============================================================================
//
// �v���C���[�p�ԃQ�[�W��ʏ��� [redguage.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _REDGUAGE_H_
#define _REDGUAGE_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_REDGUAGE			_T("data/TEXTURE/hpguage000.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	REDGUAGE_POS_X				(100)									// �Q�[�W�̕\���ʒu
#define	REDGUAGE_POS_Y				(50)									// �Q�[�W�̕\���ʒu
#define	REDGUAGE_SIZE_X				(500)									// �Q�[�W�̕�
#define	REDGUAGE_SIZE_Y				(50)									// �Q�[�W�̍���
#define REDGUAGE_PATTERN_DIVIDE_X	(1)										// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define REDGUAGE_PATTERN_DIVIDE_Y	(3)										// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_REDGUAGE	(REDGUAGE_PATTERN_DIVIDE_X*REDGUAGE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_REDGUAGE		(4)										// �A�j���[�V�����̐؂�ւ��J�E���g
#define REDGUAGE_MAX				(1)										// �Q�[�W�̌�

//HP�Q�[�W�̍\����
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// ���_���i�[���[�N
	D3DXVECTOR3				pos;							// �|���S���̈ړ���
	int						CountAnim;						// �A�j���[�V�����J�E���g
	int						PatternAnim;					// �A�j���[�V�����p�^�[���i���o�[
	bool					use;							// �v���C���[�̎g�p����
	int						red;							// �ԃQ�[�W
}REDGUAGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitRedGuage(int type);
void UninitRedGuage(void);
void UpdateRedGuage(void);
void DrawRedGuage(void);
REDGUAGE *GetRedGuage(int eno);

#endif
