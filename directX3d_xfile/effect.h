//=============================================================================
//
// 1P2P�\������ [effect.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_HIT00		"data/EFFECT/hit000.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_HIT01		"data/EFFECT/hit001.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_GUARD00		"data/EFFECT/guard000.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_CHARGE00	"data/EFFECT/charge000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	EFFECT_WIDTH			(20.0f)						// ���a����
#define	EFFECT_HEIGHT			(20.0f)						// ���a��

#define HIT00_PATTERN_DIVIDE_X	(2)										// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define HIT00_PATTERN_DIVIDE_Y	(5)										// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_HIT00	(HIT00_PATTERN_DIVIDE_X*HIT00_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define HIT01_PATTERN_DIVIDE_X	(3)										// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define HIT01_PATTERN_DIVIDE_Y	(5)										// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_HIT01	(HIT01_PATTERN_DIVIDE_X*HIT01_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define GUARD_PATTERN_DIVIDE_X	(2)										// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define GUARD_PATTERN_DIVIDE_Y	(5)										// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_GUARD	(GUARD_PATTERN_DIVIDE_X*GUARD_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define CHARGE_PATTERN_DIVIDE_X	(3)										// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define CHARGE_PATTERN_DIVIDE_Y	(5)										// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_CHARGE	(CHARGE_PATTERN_DIVIDE_X*CHARGE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����

#define EFFECT_MAX				(4)	// �G�t�F�N�g�̎��

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9		D3DTexture[EFFECT_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 D3DVtxBuff = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	D3DXMATRIX				mtxWorld;				// ���[���h�}�g���b�N�X
	D3DXVECTOR3				pos;					// �ʒu
	D3DXVECTOR3				scl;					// �X�P�[��
	float					width;					// ��
	float					height;					// ����
	int						cntPattern;				// �A�j���[�V�����ԍ�
	int						num;					// �g�p����e�N�X�`���̔ԍ�
	bool					use;					// �g�p���Ă��邩�ǂ���
} EFFECT;

// �G�t�F�N�g�ԍ��iEFFECT�\���̂�num�ɓ���Ďg�p�j
enum EffecuNum
{
	hit00,
	hit01,
	guard00,
	charge00
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEffect(int type, EFFECT *EffectWk);
void UninitEffect(EFFECT *EffectWk);
void UpdateEffect(EFFECT *EffectWk);
void DrawEffect(EFFECT *EffectWk);
int SetEffect(EFFECT *EffectWk, D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col, int num);
void SetTextureEffect(EFFECT *EffectWk, int cntPattern);

#endif
