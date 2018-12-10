//=============================================================================
//
// �v���C���[�p�ԃQ�[�W��ʏ��� [redguage.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "redguage.h"
#include "player.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexRedGuage(void);
void SetTextureRedGuage(int cntPattern);
void SetVertexRedGuage(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureRedGuage = NULL;		// �e�N�X�`���ւ̃|�C���^

REDGUAGE redguage[REDGUAGE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitRedGuage(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_REDGUAGE,				// �t�@�C���̖��O
			&g_pD3DTextureRedGuage);		// �ǂݍ��ރ������[

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// �Q�[�W�̏�����
	redguage->use = true;
	redguage->pos = D3DXVECTOR3(REDGUAGE_POS_X, REDGUAGE_POS_Y, 0.0f);
	redguage->red = player->HPzan;

	// ���_���̍쐬
	MakeVertexRedGuage();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitRedGuage(void)
{
	if (g_pD3DTextureRedGuage != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureRedGuage->Release();
		g_pD3DTextureRedGuage = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateRedGuage(void)
{
	if (redguage->use == true)
	{
		//�ԃQ�[�W
		redguage->PatternAnim = 2;

		//�e�N�X�`�����W���Z�b�g
		SetTextureRedGuage(redguage->PatternAnim);


		SetVertexRedGuage();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawRedGuage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (redguage->use == true)
	{

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureRedGuage);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, redguage->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexRedGuage(void)
{

	// ���_���W�̐ݒ�
	SetVertexRedGuage();

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	redguage->vertexWk[0].rhw =
		redguage->vertexWk[1].rhw =
		redguage->vertexWk[2].rhw =
		redguage->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	redguage->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	redguage->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	redguage->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	redguage->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	redguage->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	redguage->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	redguage->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	redguage->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureRedGuage(int cntPattern)
{
	PLAYER *player = GetPlayer(0);

	if (redguage->red > player->HPzan)
	{
		redguage->red -= 5;
	}

	//�g���[�j���O���[�h�Ȃǂŉ񕜂������p
	if (player->HPzan > redguage->red)
	{
		redguage->red = player->HPzan;
	}

	int x = cntPattern % REDGUAGE_PATTERN_DIVIDE_X;
	int y = cntPattern / REDGUAGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / REDGUAGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / REDGUAGE_PATTERN_DIVIDE_Y;

	// �e�N�X�`�����W�̐ݒ�
	redguage->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)(player->HP - redguage->red) / (float)player->HP), (float)(y)* sizeY);
	redguage->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	redguage->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)(player->HP - redguage->red) / (float)player->HP), (float)(y)* sizeY + sizeY);
	redguage->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexRedGuage(void)
{
	PLAYER *player = GetPlayer(0);

	// ���_���W�̐ݒ�
	redguage->vertexWk[0].vtx = D3DXVECTOR3(redguage->pos.x + REDGUAGE_SIZE_X * ((float)(player->HP - redguage->red) / player->HP), redguage->pos.y, redguage->pos.z);
	redguage->vertexWk[1].vtx = D3DXVECTOR3(redguage->pos.x + REDGUAGE_SIZE_X, redguage->pos.y, redguage->pos.z);
	redguage->vertexWk[2].vtx = D3DXVECTOR3(redguage->pos.x + REDGUAGE_SIZE_X * ((float)(player->HP - redguage->red) / player->HP), redguage->pos.y + REDGUAGE_SIZE_Y, redguage->pos.z);
	redguage->vertexWk[3].vtx = D3DXVECTOR3(redguage->pos.x + REDGUAGE_SIZE_X, redguage->pos.y + REDGUAGE_SIZE_Y, redguage->pos.z);
}

REDGUAGE *GetRedGuage(int eno)
{
	return &redguage[eno];
}