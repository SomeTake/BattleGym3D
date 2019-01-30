//=============================================================================
//
// 2P�pSP�Q�[�W��ʏ��� [espgauge.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "espgauge.h"
#include "enemy.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureESpGauge = NULL;		// �e�N�X�`���ւ̃|�C���^

ESPGAUGE espgauge[ESPGAUGE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitESpGauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_ESPGAUGE,				// �t�@�C���̖��O
			&g_pD3DTextureESpGauge);		// �ǂݍ��ރ������[

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// �Q�[�W�̏�����
	ESPGAUGE *espgauge = GetESpGauge(0);
	espgauge->use = true;
	espgauge->pos = D3DXVECTOR3(ESPGAUGE_POS_X, ESPGAUGE_POS_Y, 0.0f);
	espgauge->PatternAnim = 0;
	espgauge->CountAnim = 0;

	// ���_���̍쐬
	MakeVertexESpGauge();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitESpGauge(void)
{
	if (g_pD3DTextureESpGauge != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureESpGauge->Release();
		g_pD3DTextureESpGauge = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateESpGauge(void)
{
	ESPGAUGE *espgauge = GetESpGauge(0);

	if (espgauge->use == true)
	{
		espgauge->PatternAnim = 1;

		//�e�N�X�`�����W���Z�b�g
		SetTextureESpGauge(espgauge->PatternAnim);

		SetVertexESpGauge();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawESpGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ESPGAUGE *espgauge = GetESpGauge(0);

	if (espgauge->use == true)
	{

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureESpGauge);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, espgauge->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexESpGauge(void)
{
	// ���_���W�̐ݒ�
	SetVertexESpGauge();

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	espgauge->vertexWk[0].rhw =
		espgauge->vertexWk[1].rhw =
		espgauge->vertexWk[2].rhw =
		espgauge->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	espgauge->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	espgauge->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	espgauge->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	espgauge->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	espgauge->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	espgauge->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	espgauge->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	espgauge->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureESpGauge(int cntPattern)
{
	int x = cntPattern % ESPGAUGE_PATTERN_DIVIDE_X;
	int y = cntPattern / ESPGAUGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / ESPGAUGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / ESPGAUGE_PATTERN_DIVIDE_Y;

	// �e�N�X�`�����W�̐ݒ�
	espgauge->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	espgauge->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	espgauge->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	espgauge->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexESpGauge(void)
{
	CHARA *enemyWk = GetEnemy();

	// ���_���W�̐ݒ�
	espgauge->vertexWk[0].vtx = D3DXVECTOR3(espgauge->pos.x + ESPGAUGE_SIZE_X - ESPGAUGE_SIZE_X * ((float)enemyWk->SP / (float)FULL_SPGAUGE), espgauge->pos.y, espgauge->pos.z);
	espgauge->vertexWk[1].vtx = D3DXVECTOR3(espgauge->pos.x + ESPGAUGE_SIZE_X, espgauge->pos.y, espgauge->pos.z);
	espgauge->vertexWk[2].vtx = D3DXVECTOR3(espgauge->pos.x + ESPGAUGE_SIZE_X - ESPGAUGE_SIZE_X * ((float)enemyWk->SP / (float)FULL_SPGAUGE), espgauge->pos.y + ESPGAUGE_SIZE_Y, espgauge->pos.z);
	espgauge->vertexWk[3].vtx = D3DXVECTOR3(espgauge->pos.x + ESPGAUGE_SIZE_X, espgauge->pos.y + ESPGAUGE_SIZE_Y, espgauge->pos.z);
}

//=============================================================================
// �G�l�~�[SP�Q�[�W�̃Q�b�^�[
//=============================================================================
ESPGAUGE *GetESpGauge(int gno)
{
	return &espgauge[gno];
}
