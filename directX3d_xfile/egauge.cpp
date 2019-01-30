//=============================================================================
//
// 2P�pHP�Q�[�W��ʏ��� [egauge.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "egauge.h"
#include "enemy.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEGauge = NULL;		// �e�N�X�`���ւ̃|�C���^

EGAUGE egauge[EGAUGE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEGauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_EGAUGE,				// �t�@�C���̖��O
			&g_pD3DTextureEGauge);		// �ǂݍ��ރ������[

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// �Q�[�W�̏�����
	egauge->use = true;
	egauge->pos = D3DXVECTOR3(EGAUGE_POS_X, EGAUGE_POS_Y, 0.0f);

	// ���_���̍쐬
	MakeVertexEGauge();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEGauge(void)
{
	if (g_pD3DTextureEGauge != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureEGauge->Release();
		g_pD3DTextureEGauge = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEGauge(void)
{
	CHARA *enemy = GetEnemy();

	if (egauge->use == true)
	{
		//HP���^���̏ꍇ
		if (enemy->HPzan == enemy->HP)
		{
			egauge->PatternAnim = 0;
		}
		//100%�����̏ꍇ
		else if (enemy->HPzan < enemy->HP)
		{
			egauge->PatternAnim = 1;
		}

		//�e�N�X�`�����W���Z�b�g
		SetTextureEGauge(egauge->PatternAnim);


		SetVertexEGauge();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (egauge->use == true)
	{

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureEGauge);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, egauge->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexEGauge(void)
{

	// ���_���W�̐ݒ�
	SetVertexEGauge();

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	egauge->vertexWk[0].rhw =
		egauge->vertexWk[1].rhw =
		egauge->vertexWk[2].rhw =
		egauge->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	egauge->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	egauge->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	egauge->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	egauge->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	egauge->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	egauge->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	egauge->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	egauge->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureEGauge(int cntPattern)
{
	CHARA *enemy = GetEnemy();

	int x = cntPattern % EGAUGE_PATTERN_DIVIDE_X;
	int y = cntPattern / EGAUGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / EGAUGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / EGAUGE_PATTERN_DIVIDE_Y;

	// �e�N�X�`�����W�̐ݒ�
	egauge->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	egauge->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)enemy->HPzan / (float)enemy->HP), (float)(y)* sizeY);
	egauge->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	egauge->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)enemy->HPzan / (float)enemy->HP), (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexEGauge(void)
{
	CHARA *enemy = GetEnemy();

	// ���_���W�̐ݒ�
	egauge->vertexWk[0].vtx = D3DXVECTOR3(egauge->pos.x, egauge->pos.y, egauge->pos.z);
	egauge->vertexWk[1].vtx = D3DXVECTOR3(egauge->pos.x + EGAUGE_SIZE_X * ((float)enemy->HPzan / (float)enemy->HP), egauge->pos.y, egauge->pos.z);
	egauge->vertexWk[2].vtx = D3DXVECTOR3(egauge->pos.x, egauge->pos.y + EGAUGE_SIZE_Y, egauge->pos.z);
	egauge->vertexWk[3].vtx = D3DXVECTOR3(egauge->pos.x + EGAUGE_SIZE_X * ((float)enemy->HPzan / (float)enemy->HP), egauge->pos.y + EGAUGE_SIZE_Y, egauge->pos.z);
}
