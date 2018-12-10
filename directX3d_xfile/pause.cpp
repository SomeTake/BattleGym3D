//=============================================================================
//
// �|�[�Y��ʏ��� [pause.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "pause.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexPause(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTexturePause = NULL;	// �e�N�X�`���ւ̃|�C���^

VERTEX_2D				g_vertexWkPause[NUM_VERTEX];		// ���_���i�[���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPause(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_PAUSE,			// �t�@�C���̖��O
			&g_pD3DTexturePause);	// �ǂݍ��ރ������[
	}

	// ���_���̍쐬
	MakeVertexPause();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPause(void)
{
	if (g_pD3DTexturePause != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTexturePause->Release();
		g_pD3DTexturePause = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePause(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTexturePause);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkPause, sizeof(VERTEX_2D));
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexPause(void)
{
	// ���_���W�̐ݒ�
	g_vertexWkPause[0].vtx = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y, 0.0f);
	g_vertexWkPause[1].vtx = D3DXVECTOR3(PAUSE_POS_X + PAUSE_SIZE_X, PAUSE_POS_Y, 0.0f);
	g_vertexWkPause[2].vtx = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y + PAUSE_SIZE_Y, 0.0f);
	g_vertexWkPause[3].vtx = D3DXVECTOR3(PAUSE_POS_X + PAUSE_SIZE_X, PAUSE_POS_Y + PAUSE_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkPause[0].rhw =
		g_vertexWkPause[1].rhw =
		g_vertexWkPause[2].rhw =
		g_vertexWkPause[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkPause[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkPause[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkPause[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkPause[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkPause[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkPause[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkPause[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkPause[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

