//=============================================================================
//
// ���[�h�Z���N�g���� [modeselect.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _MODESELECT_H_
#define _MODESELECT_H_

// �}�N����`
#define TEXTURE_GAME_MODESELECT		_T("data/TEXTURE/select000.png")	// �摜
#define TEXTURE_MODESELECT_SIZE_X	(380)			// �e�N�X�`���T�C�Y
#define TEXTURE_MODESELECT_SIZE_Y	(56)			// ����
#define TEXTURE_PATTERN_DIVIDE_X_MODESELECT	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_MODESELECT	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_MODESELECT			(TEXTURE_PATTERN_DIVIDE_X_MODESELECT*TEXTURE_PATTERN_DIVIDE_Y_MODESELECT)	// �A�j���[�V�����p�^�[����
#define MODESELECT_MAX						(1)		// �e�N�X�`���̐�
#define TITLESELECT_MAX						(4)		// �^�C�g����ʂ���I�ׂ�I�����̐�
#define PAUSESELECT_MAX						(3)		// �|�[�Y��ʂ���I�ׂ�I�����̐�
#define BUTTON_TIMER						(15)	// �{�^���̃��s�[�g��L���ɂ��邽�߂̑҂�����

//*****************************************************************************
// �\���̐錾
//*****************************************************************************

typedef struct	// ���[�h�Z���N�g�\����
{
	D3DXVECTOR3		pos;						// �|���S���̈ړ���
	int				PatternAnim;				// �A�j���[�V�����p�^�[���i���o�[
	int				CountAnim;					// �A�j���[�V�����J�E���g
	LPDIRECT3DTEXTURE9	Texture;				// �e�N�X�`�����
	VERTEX_2D		vertexWk[NUM_VERTEX];		// ���_���i�[���[�N

} MODESELECT;

// �^�C�g����ʂ̃Z���N�g�ԍ�
enum TitleNum
{
	NextTutorial,
	NextTraining,
	NextGame,
	NextThankyou
};

// �|�[�Y��ʂ̃Z���N�g�ԍ�
enum PauseNum
{
	NextPauseGame,
	NextPauseCountdown,
	NextPauseTitle,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitModeselect(int type);
void UninitModeselect(void);
void UpdateModeselect(void);
void DrawModeselect(void);
MODESELECT *GetModeselect(int no);
HRESULT MakeVertexModeselect(int no);
void SetTextureModeselect(int cntPattern);
void SetVertexModeselect(void);
void SetReflectModeselect(float per);		// �����x�̐ݒ�
int GetNextPhase(void);						// ���̃t�F�[�Y��I������ԍ�
bool GetTitleSelect(void);					// �^�C�g����ʂ̃Z���N�g���I�����Ă��邩�ǂ����擾

#endif
