//=============================================================================
//
// ���̓��[�h�Z���N�g��ʏ��� [inputselect.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "inputselect.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureInputselect = NULL;		// �e�N�X�`���ւ̃|�C���^

VERTEX_2D				g_vertexWkInputselect[NUM_VERTEX];	// ���_���i�[���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitInputselect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_INPUTSELECT,			// �t�@�C���̖��O
			&g_pD3DTextureInputselect);	// �ǂݍ��ރ������[
	}

	// ���_���̍쐬
	MakeVertexInputselect();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitInputselect(void)
{
	if (g_pD3DTextureInputselect != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureInputselect->Release();
		g_pD3DTextureInputselect = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateInputselect(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawInputselect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureInputselect);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkInputselect, sizeof(VERTEX_2D));
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexInputselect(void)
{
	// ���_���W�̐ݒ�
	g_vertexWkInputselect[0].vtx = D3DXVECTOR3(INPUTSELECT_POS_X, INPUTSELECT_POS_Y, 0.0f);
	g_vertexWkInputselect[1].vtx = D3DXVECTOR3(INPUTSELECT_POS_X + INPUTSELECT_SIZE_X, INPUTSELECT_POS_Y, 0.0f);
	g_vertexWkInputselect[2].vtx = D3DXVECTOR3(INPUTSELECT_POS_X, INPUTSELECT_POS_Y + INPUTSELECT_SIZE_Y, 0.0f);
	g_vertexWkInputselect[3].vtx = D3DXVECTOR3(INPUTSELECT_POS_X + INPUTSELECT_SIZE_X, INPUTSELECT_POS_Y + INPUTSELECT_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkInputselect[0].rhw =
		g_vertexWkInputselect[1].rhw =
		g_vertexWkInputselect[2].rhw =
		g_vertexWkInputselect[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkInputselect[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkInputselect[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkInputselect[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkInputselect[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkInputselect[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkInputselect[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkInputselect[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkInputselect[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

