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
#define	TEXTURE_REDGUAGE			_T("data/TEXTURE/hpguage000.png")		// �ԃQ�[�W�p�摜
#define	REDGUAGE_POS_X				(100)									// �ԃQ�[�W�̕\���ʒu
#define	REDGUAGE_POS_Y				(50)									// �ԃQ�[�W�̕\���ʒu
#define	REDGUAGE_SIZE_X				(500)									// �ԃQ�[�W�̕�
#define	REDGUAGE_SIZE_Y				(50)									// �ԃQ�[�W�̍���
#define REDGUAGE_PATTERN_DIVIDE_X	(1)										// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define REDGUAGE_PATTERN_DIVIDE_Y	(3)										// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_REDGUAGE	(REDGUAGE_PATTERN_DIVIDE_X*REDGUAGE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_REDGUAGE		(4)										// �A�j���[�V�����̐؂�ւ��J�E���g
#define REDGUAGE_MAX				(1)										// �ԃQ�[�W�̌�

//HP�Q�[�W�̍\����
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// ���_���i�[���[�N
	D3DXVECTOR3				pos;							// �|���S���̈ړ���
	int						CountAnim;						// �A�j���[�V�����J�E���g
	int						PatternAnim;					// �A�j���[�V�����p�^�[���i���o�[
	bool					use;							// �v���C���[�̎g�p����
	int						value;							// �ԃQ�[�W��HP�Q�[�W�ƍ��킹��Ƃ��ɑ���������
}REDGUAGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitRedGuage(int type);
void UninitRedGuage(void);
void UpdateRedGuage(void);
void DrawRedGuage(void);
HRESULT MakeVertexRedGuage(void);
void SetTextureRedGuage(int cntPattern);
void SetVertexRedGuage(void);

#endif
