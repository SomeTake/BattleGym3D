//=============================================================================
//
// 2P�pHP�Q�[�W��ʏ��� [eguage.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _EGUAGE_H_
#define _EGUAGE_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_EGUAGE			_T("data/TEXTURE/ehpguage000.png")			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	EGUAGE_POS_X			(680)										// �Q�[�W�̕\���ʒu
#define	EGUAGE_POS_Y			(50)										// �Q�[�W�̕\���ʒu
#define	EGUAGE_SIZE_X			(500)										// �Q�[�W�̕�
#define	EGUAGE_SIZE_Y			(50)										// �Q�[�W�̍���
#define EGUAGE_PATTERN_DIVIDE_X	(1)											// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define EGUAGE_PATTERN_DIVIDE_Y	(3)											// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_EGUAGE	(EGUAGE_PATTERN_DIVIDE_X*EGUAGE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_EGUAGE	(4)											// �A�j���[�V�����̐؂�ւ��J�E���g
#define EGUAGE_MAX				(1)											// �Q�[�W�̌�

//HP�Q�[�W�̍\����
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// ���_���i�[���[�N
	D3DXVECTOR3				pos;							// �|���S���̈ړ���
	int						CountAnim;						// �A�j���[�V�����J�E���g
	int						PatternAnim;					// �A�j���[�V�����p�^�[���i���o�[
	bool					use;							// �v���C���[�̎g�p����
}EGUAGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEGuage(int type);
void UninitEGuage(void);
void UpdateEGuage(void);
void DrawEGuage(void);
HRESULT MakeVertexEGuage(void);
void SetTextureEGuage(int cntPattern);
void SetVertexEGuage(void);

#endif
