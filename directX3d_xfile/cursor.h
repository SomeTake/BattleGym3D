//=============================================================================
//
// �J�[�\������ [cursor.h]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#ifndef _CURSOR_H_
#define _CURSOR_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_CURSOR1P	_T("data/TEXTURE/cursor001.png")			// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_CURSOR2P	_T("data/TEXTURE/cursor002.png")			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	CURSOR_SIZE_X		(320)										// ��
#define	CURSOR_SIZE_Y		(360)										// ����
#define	CURSOR_POS_X_A		(SCREEN_WIDTH / 2 - CURSOR_SIZE_X + 17)		// �\���ʒu
#define CURSOR_POS_X_B		(SCREEN_WIDTH / 2 - 11)
#define	CURSOR_POS_Y		(SCREEN_HEIGHT / 2 - 10)					// �\���ʒu

#define CURSOR_MAX			(2)											// �J�[�\���̐�=�v���C���[��

typedef struct
{
	D3DXVECTOR3				pos;					// �\���ʒu
	LPDIRECT3DTEXTURE9		D3DTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	bool					modeinput;				// ���̓��[�h�̑I����OK���ǂ���
}CURSOR;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitCursor(int type);
void UninitCursor(void);
void UpdateCursor(void);
void DrawCursor(void);
HRESULT MakeVertexCursor(int num);
void SetVertexCursor(int num);
void SetReflectCursor(int num, float per);
CURSOR *GetCursor(int num);

#endif
