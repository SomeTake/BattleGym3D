//=============================================================================
//
// 2P�pSP�Q�[�W��ʏ��� [espguage.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _ESPGUAGE_H_
#define _ESPGUAGE_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_ESPGUAGE			_T("data/TEXTURE/spguage000.png")			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	ESPGUAGE_POS_X			(880)										// �Q�[�W�̕\���ʒu
#define	ESPGUAGE_POS_Y			(675)										// �Q�[�W�̕\���ʒu
#define	ESPGUAGE_SIZE_X			(300)										// �Q�[�W�̕�
#define	ESPGUAGE_SIZE_Y			(30)										// �Q�[�W�̍���
#define ESPGUAGE_PATTERN_DIVIDE_X	(1)										// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define ESPGUAGE_PATTERN_DIVIDE_Y	(1)										// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_ESPGUAGE	(ESPGUAGE_PATTERN_DIVIDE_X*ESPGUAGE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_ESPGUAGE	(4)										// �A�j���[�V�����̐؂�ւ��J�E���g
#define ESPGUAGE_MAX				(1)										// �Q�[�W�̌�
#define FULL_ESPGUAGE			(1000)									// �Q�[�W�̍ő�l

//SP�Q�[�W�̍\����
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// ���_���i�[���[�N
	D3DXVECTOR3				pos;							// �|���S���̈ړ���
	int						CountAnim;						// �A�j���[�V�����J�E���g
	int						PatternAnim;					// �A�j���[�V�����p�^�[���i���o�[
	int						sp;								// SP�Q�[�W�̌��݂̒l
	bool					use;							// �v���C���[�̎g�p����
}ESPGUAGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitESpGuage(int type);
void UninitESpGuage(void);
void UpdateESpGuage(void);
void DrawESpGuage(void);
ESPGUAGE *GetESpGuage(int gno);
void AddESpGuage(int add);

#endif
