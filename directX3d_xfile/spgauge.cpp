//=============================================================================
//
// SP�Q�[�W��ʏ��� [spgauge.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "spgauge.h"
#include "player.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureSpGauge = NULL;		// �e�N�X�`���ւ̃|�C���^

SPGAUGE spgauge[SPGAUGE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSpGauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_SPGAUGE,				// �t�@�C���̖��O
			&g_pD3DTextureSpGauge);		// �ǂݍ��ރ������[

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// �Q�[�W�̏�����

	SPGAUGE *spgauge = GetSpGauge(0);
	spgauge->use = true;
	spgauge->pos = D3DXVECTOR3(SPGAUGE_POS_X, SPGAUGE_POS_Y, 0.0f);
	spgauge->PatternAnim = 0;
	spgauge->CountAnim = 0;

	// ���_���̍쐬
	MakeVertexSpGauge();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSpGauge(void)
{
	if (g_pD3DTextureSpGauge != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureSpGauge->Release();
		g_pD3DTextureSpGauge = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSpGauge(void)
{
	CHARA *player = GetPlayer();
	SPGAUGE *spgauge = GetSpGauge(0);

	if (spgauge->use == true)
	{
		spgauge->PatternAnim = 1;

		//�e�N�X�`�����W���Z�b�g
		SetTextureSpGauge(spgauge->PatternAnim);

		SetVertexSpGauge();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSpGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SPGAUGE *spgauge = GetSpGauge(0);

	if (spgauge->use == true)
	{

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureSpGauge);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, spgauge->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexSpGauge(void)
{

	// ���_���W�̐ݒ�
	SetVertexSpGauge();

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	spgauge->vertexWk[0].rhw =
		spgauge->vertexWk[1].rhw =
		spgauge->vertexWk[2].rhw =
		spgauge->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	spgauge->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	spgauge->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	spgauge->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	spgauge->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	spgauge->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	spgauge->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	spgauge->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	spgauge->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureSpGauge(int cntPattern)
{

	int x = cntPattern % SPGAUGE_PATTERN_DIVIDE_X;
	int y = cntPattern / SPGAUGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / SPGAUGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / SPGAUGE_PATTERN_DIVIDE_Y;

	// �e�N�X�`�����W�̐ݒ�
	spgauge->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	spgauge->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	spgauge->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	spgauge->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexSpGauge(void)
{
	CHARA *playerWk = GetPlayer();

	// ���_���W�̐ݒ�
	spgauge->vertexWk[0].vtx = D3DXVECTOR3(spgauge->pos.x, spgauge->pos.y, spgauge->pos.z);
	spgauge->vertexWk[1].vtx = D3DXVECTOR3(spgauge->pos.x + SPGAUGE_SIZE_X * ((float)playerWk->SP / (float)FULL_SPGAUGE), spgauge->pos.y, spgauge->pos.z);
	spgauge->vertexWk[2].vtx = D3DXVECTOR3(spgauge->pos.x, spgauge->pos.y + SPGAUGE_SIZE_Y, spgauge->pos.z);
	spgauge->vertexWk[3].vtx = D3DXVECTOR3(spgauge->pos.x + SPGAUGE_SIZE_X * ((float)playerWk->SP / (float)FULL_SPGAUGE), spgauge->pos.y + SPGAUGE_SIZE_Y, spgauge->pos.z);
}

//=============================================================================
// �v���C���[SP�Q�[�W�̃Q�b�^�[
//=============================================================================
SPGAUGE *GetSpGauge(int gno)
{
	return &spgauge[gno];
}