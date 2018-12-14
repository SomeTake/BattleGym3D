//=============================================================================
//
// �E�B�i�[�\������ [winner.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "winner.h"
#include "player.h"
#include "enemy.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureWinner = NULL;		// �e�N�X�`���ւ̃|�C���^

static VERTEX_2D				g_vertexWkWinner[NUM_VERTEX];			// ���_���i�[���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitWinner(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_WINNER,				// �t�@�C���̖��O
			&g_pD3DTextureWinner);		// �ǂݍ��ރ������[

	}

	// ���_���̍쐬
	MakeVertexWinner();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitWinner(void)
{
	if (g_pD3DTextureWinner != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureWinner->Release();
		g_pD3DTextureWinner = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateWinner(void)
{
	PLAYER *player = GetPlayer(0);
	ENEMY *enemy = GetEnemy(0);

	if (player->HPzan > enemy->HPzan)
	{
		SetTextureWinner(0);
	}
	else if (player->HPzan < enemy->HPzan)
	{
		SetTextureWinner(1);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawWinner(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureWinner);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkWinner, sizeof(VERTEX_2D));
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexWinner(void)
{

	// ���_���W�̐ݒ�
	SetVertexWinner();

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkWinner[0].rhw =
		g_vertexWkWinner[1].rhw =
		g_vertexWkWinner[2].rhw =
		g_vertexWkWinner[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkWinner[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkWinner[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkWinner[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkWinner[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkWinner[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkWinner[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkWinner[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkWinner[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureWinner(int cntPattern)
{
	int x = cntPattern % TEXTURE_PATTERN_WINNER_X;
	int y = cntPattern / TEXTURE_PATTERN_WINNER_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_WINNER_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_WINNER_Y;

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkWinner[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	g_vertexWkWinner[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	g_vertexWkWinner[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	g_vertexWkWinner[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexWinner(void)
{
	g_vertexWkWinner[0].vtx = D3DXVECTOR3(WINNER_POS_X, WINNER_POS_Y, 0.0f);
	g_vertexWkWinner[1].vtx = D3DXVECTOR3(WINNER_POS_X + WINNER_SIZE_X, WINNER_POS_Y, 0.0f);
	g_vertexWkWinner[2].vtx = D3DXVECTOR3(WINNER_POS_X, WINNER_POS_Y + WINNER_SIZE_Y, 0.0f);
	g_vertexWkWinner[3].vtx = D3DXVECTOR3(WINNER_POS_X + WINNER_SIZE_X, WINNER_POS_Y + WINNER_SIZE_Y, 0.0f);
}