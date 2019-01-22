//=============================================================================
//
// SP�Q�[�W��ʏ��� [spguage.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "spguage.h"
#include "player.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureSpGuage = NULL;		// �e�N�X�`���ւ̃|�C���^

SPGUAGE spguage[SPGUAGE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSpGuage(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_SPGUAGE,				// �t�@�C���̖��O
			&g_pD3DTextureSpGuage);		// �ǂݍ��ރ������[

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// �Q�[�W�̏�����

	SPGUAGE *spguage = GetSpGuage(0);
	spguage->use = true;
	spguage->pos = D3DXVECTOR3(SPGUAGE_POS_X, SPGUAGE_POS_Y, 0.0f);
	spguage->PatternAnim = 0;
	spguage->CountAnim = 0;

	// ���_���̍쐬
	MakeVertexSpGuage();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSpGuage(void)
{
	if (g_pD3DTextureSpGuage != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureSpGuage->Release();
		g_pD3DTextureSpGuage = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSpGuage(void)
{
	CHARA *player = GetPlayer();
	SPGUAGE *spguage = GetSpGuage(0);

	if (spguage->use == true)
	{
		spguage->PatternAnim = 1;

		//�e�N�X�`�����W���Z�b�g
		SetTextureSpGuage(spguage->PatternAnim);

		SetVertexSpGuage();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSpGuage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SPGUAGE *spguage = GetSpGuage(0);

	if (spguage->use == true)
	{

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureSpGuage);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, spguage->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexSpGuage(void)
{

	// ���_���W�̐ݒ�
	SetVertexSpGuage();

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	spguage->vertexWk[0].rhw =
		spguage->vertexWk[1].rhw =
		spguage->vertexWk[2].rhw =
		spguage->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	spguage->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	spguage->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	spguage->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	spguage->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	spguage->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	spguage->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	spguage->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	spguage->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureSpGuage(int cntPattern)
{

	int x = cntPattern % SPGUAGE_PATTERN_DIVIDE_X;
	int y = cntPattern / SPGUAGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / SPGUAGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / SPGUAGE_PATTERN_DIVIDE_Y;

	// �e�N�X�`�����W�̐ݒ�
	spguage->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	spguage->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	spguage->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	spguage->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexSpGuage(void)
{
	CHARA *playerWk = GetPlayer();

	// ���_���W�̐ݒ�
	spguage->vertexWk[0].vtx = D3DXVECTOR3(spguage->pos.x, spguage->pos.y, spguage->pos.z);
	spguage->vertexWk[1].vtx = D3DXVECTOR3(spguage->pos.x + SPGUAGE_SIZE_X * ((float)playerWk->SP / (float)FULL_SPGUAGE), spguage->pos.y, spguage->pos.z);
	spguage->vertexWk[2].vtx = D3DXVECTOR3(spguage->pos.x, spguage->pos.y + SPGUAGE_SIZE_Y, spguage->pos.z);
	spguage->vertexWk[3].vtx = D3DXVECTOR3(spguage->pos.x + SPGUAGE_SIZE_X * ((float)playerWk->SP / (float)FULL_SPGUAGE), spguage->pos.y + SPGUAGE_SIZE_Y, spguage->pos.z);
}

//=============================================================================
// �v���C���[SP�Q�[�W�̃Q�b�^�[
//=============================================================================
SPGUAGE *GetSpGuage(int gno)
{
	return &spguage[gno];
}