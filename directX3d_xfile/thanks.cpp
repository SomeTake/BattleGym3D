//=============================================================================
//
// �I����ʏ��� [thanks.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "thanks.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureThanks = NULL;		// �e�N�X�`���ւ̃|�C���^

VERTEX_2D				g_vertexWkThanks[NUM_VERTEX];	// ���_���i�[���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitThanks(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_THANKS,			// �t�@�C���̖��O
			&g_pD3DTextureThanks);	// �ǂݍ��ރ������[
	}

	// ���_���̍쐬
	MakeVertexThanks();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitThanks(void)
{
	if (g_pD3DTextureThanks != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureThanks->Release();
		g_pD3DTextureThanks = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateThanks(void)
{
	static float per = 0.0f;
	static int frame = 0;

	frame++;

	//�����x�̐ݒ�
	//���X�ɕ\��
	if (frame < 100)
	{
		per += 0.01f;
	}
	//���X�ɏ����Ă���
	else if (frame >= 100 && frame < 200)
	{
		per -= 0.01f;
	}

	//���S�ɏ�������^�C�g����ʂ�
	if (frame == 200)
	{
		SetPhase(PhaseExit);
	}
	SetReflectThanks(per);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawThanks(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureThanks);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkThanks, sizeof(VERTEX_2D));
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexThanks(void)
{
	// ���_���W�̐ݒ�
	g_vertexWkThanks[0].vtx = D3DXVECTOR3(THANKS_POS_X, THANKS_POS_Y, 0.0f);
	g_vertexWkThanks[1].vtx = D3DXVECTOR3(THANKS_POS_X + THANKS_SIZE_X, THANKS_POS_Y, 0.0f);
	g_vertexWkThanks[2].vtx = D3DXVECTOR3(THANKS_POS_X, THANKS_POS_Y + THANKS_SIZE_Y, 0.0f);
	g_vertexWkThanks[3].vtx = D3DXVECTOR3(THANKS_POS_X + THANKS_SIZE_X, THANKS_POS_Y + THANKS_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkThanks[0].rhw =
		g_vertexWkThanks[1].rhw =
		g_vertexWkThanks[2].rhw =
		g_vertexWkThanks[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkThanks[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkThanks[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkThanks[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkThanks[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkThanks[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkThanks[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkThanks[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkThanks[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// ���ˌ��̐ݒ� ����:float per = �����x�́�
//=============================================================================
void SetReflectThanks(float per)
{
	int clear = (int)(255 * per);

	g_vertexWkThanks[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkThanks[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkThanks[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkThanks[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
}