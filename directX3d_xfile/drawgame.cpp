//=============================================================================
//
// ����������ʏ��� [drawgame.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "drawgame.h"
#include "input.h"
#include "sound.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureDrawgame = NULL;		// �e�N�X�`���ւ̃|�C���^

VERTEX_2D				g_vertexWkDrawgame[NUM_VERTEX];		// ���_���i�[���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitDrawgame(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_DRAWGAME,				// �t�@�C���̖��O
			&g_pD3DTextureDrawgame);		// �ǂݍ��ރ������[

	}

	// ���_���̍쐬
	MakeVertexDrawgame();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitDrawgame(void)
{
	if (g_pD3DTextureDrawgame != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureDrawgame->Release();
		g_pD3DTextureDrawgame = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateDrawgame(void)
{
	StopSound(BGM_BATTLE, 0);

	if (GetKeyboardTrigger(DIK_RETURN))
	{// Enter��������A�X�e�[�W��؂�ւ���
	 //�Q�[�����[�v�̂��߂̍ď�����
		ReInit();
		SetPhase(PhaseCountdown);
	}
	// �Q�[���p�b�h�ňړ�����
	else if (IsButtonTriggered(0, BUTTON_M))
	{
		//�Q�[�����[�v�̂��߂̍ď�����
		ReInit();
		SetPhase(PhaseCountdown);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawDrawgame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureDrawgame);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkDrawgame, sizeof(VERTEX_2D));
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexDrawgame(void)
{

	// ���_���W�̐ݒ�
	g_vertexWkDrawgame[0].vtx = D3DXVECTOR3(DRAWGAME_POS_X, DRAWGAME_POS_Y, 0.0f);
	g_vertexWkDrawgame[1].vtx = D3DXVECTOR3(DRAWGAME_POS_X + DRAWGAME_SIZE_X, DRAWGAME_POS_Y, 0.0f);
	g_vertexWkDrawgame[2].vtx = D3DXVECTOR3(DRAWGAME_POS_X, DRAWGAME_POS_Y + DRAWGAME_SIZE_Y, 0.0f);
	g_vertexWkDrawgame[3].vtx = D3DXVECTOR3(DRAWGAME_POS_X + DRAWGAME_SIZE_X, DRAWGAME_POS_Y + DRAWGAME_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkDrawgame[0].rhw =
		g_vertexWkDrawgame[1].rhw =
		g_vertexWkDrawgame[2].rhw =
		g_vertexWkDrawgame[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkDrawgame[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkDrawgame[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkDrawgame[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkDrawgame[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkDrawgame[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkDrawgame[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkDrawgame[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkDrawgame[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}
