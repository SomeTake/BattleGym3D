//=============================================================================
//
// �󏈗� [skybox.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _SKYBOX_H_
#define _SKYBOX_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_SKYBOX					_T("data/TEXTURE/skybox000.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_SKYBOX_DIVIDE_X			(4)		// �e�N�X�`�����������i��)
#define TEXTURE_SKYBOX_DIVIDE_Y			(3)		// �e�N�X�`�����������i�c)
#define SURFACE_NUM						(6)
#define SKYBOX_LENGTH					(5000.0f)
#define SKYBLUE(Alpha)		D3DCOLOR_RGBA(135, 206, 235, Alpha)

typedef struct
{
	D3DXVECTOR3				pos;			// ��̈ʒu
	D3DXVECTOR3				rot;			// ��̌���(��])
	D3DXVECTOR3				scl;			// ��̑傫��(�X�P�[��)
}SKYBOX;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSkyBox(int type);
void UninitSkyBox(void);
void UpdateSkyBox(void);
void DrawSkyBox(void);

#endif
