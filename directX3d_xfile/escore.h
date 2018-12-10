//=============================================================================
//
// �G�l�~�[�X�R�A���� [escore.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _ESCORE_H_
#define _ESCORE_H_


// �}�N����`
#define TEXTURE_GAME_ESCORE00	_T("data/TEXTURE/score000.png")	// �T���v���p�摜

//PhaseGame,PhaseCountdown,PhaseFinish�̎�
#define TEXTURE_ESCORE00_SIZE_X	(20) // �e�N�X�`���T�C�Y
#define TEXTURE_ESCORE00_SIZE_Y	(30) // ����

//PhaseResult�̎�
#define RESULT_ESCORE00_SIZE_X	(100) // �e�N�X�`���T�C�Y
#define RESULT_ESCORE00_SIZE_Y	(150) // ����

#define TEXTURE_PATTERN_DIVIDE_ESCORE_X	(10)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_ESCORE_Y	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_ESCORE		(TEXTURE_PATTERN_DIVIDE_ESCORE_X*TEXTURE_PATTERN_DIVIDE_ESCORE_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_ESCORE		(1)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define ESCORE_MAX					(5)		//�X�R�A�̌���
#define FULL_ESCORE					(99999)	//�X�R�A�̍ő吔

//PhaseGame,PhaseCountdown,PhaseFinish�̎�
#define ESCORE_POS_X					(683)	//�X�R�A�̕\���ʒu
#define ESCORE_POS_Y					(18)	//�X�R�A�̕\���ʒu

//PhaseResult�̎�
#define RESULT_ESCORE_POS_X			(600)	//�X�R�A�̕\���ʒu
#define RESULT_ESCORE_POS_Y			(400)	//�X�R�A�̕\���ʒu

//�X�R�A�̍\����
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// ���_���i�[���[�N
	D3DXVECTOR3				pos;							// �|���S���̈ړ���
	int						CountAnim;						// �A�j���[�V�����J�E���g
	int						PatternAnim;					// �A�j���[�V�����p�^�[���i���o�[
	bool					use;							// �g�p���t���O
	int						eScore;							// �o�g���̃X�R�A
}ESCORE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEScore(int type);
void UninitEScore(void);
void UpdateEScore(void);
void DrawEScore(void);
ESCORE *GetEScore(int bno);
void AddEScore(int add);

#endif
