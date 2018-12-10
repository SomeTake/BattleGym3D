//=============================================================================
//
// 2P�pHP�Q�[�W��ʏ��� [eguage.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "eguage.h"
#include "enemy.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexEGuage(void);
void SetTextureEGuage(int cntPattern);
void SetVertexEGuage(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEGuage = NULL;		// �e�N�X�`���ւ̃|�C���^

EGUAGE eguage[EGUAGE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEGuage(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_EGUAGE,				// �t�@�C���̖��O
			&g_pD3DTextureEGuage);		// �ǂݍ��ރ������[

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// �Q�[�W�̏�����
	eguage->use = true;
	eguage->pos = D3DXVECTOR3(EGUAGE_POS_X, EGUAGE_POS_Y, 0.0f);

	// ���_���̍쐬
	MakeVertexEGuage();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEGuage(void)
{
	if (g_pD3DTextureEGuage != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureEGuage->Release();
		g_pD3DTextureEGuage = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEGuage(void)
{
	ENEMY *enemy = GetEnemy(0);

	if (eguage->use == true)
	{
		//HP���^���̏ꍇ
		if (enemy->HPzan == enemy->HP)
		{
			eguage->PatternAnim = 0;
		}
		//100%�����̏ꍇ
		else if (enemy->HPzan < enemy->HP)
		{
			eguage->PatternAnim = 1;
		}

		//�e�N�X�`�����W���Z�b�g
		SetTextureEGuage(eguage->PatternAnim);


		SetVertexEGuage();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEGuage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (eguage->use == true)
	{

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureEGuage);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, eguage->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexEGuage(void)
{

	// ���_���W�̐ݒ�
	SetVertexEGuage();

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	eguage->vertexWk[0].rhw =
		eguage->vertexWk[1].rhw =
		eguage->vertexWk[2].rhw =
		eguage->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	eguage->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	eguage->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	eguage->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	eguage->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	eguage->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	eguage->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	eguage->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	eguage->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureEGuage(int cntPattern)
{
	ENEMY *enemy = GetEnemy(0);

	int x = cntPattern % EGUAGE_PATTERN_DIVIDE_X;
	int y = cntPattern / EGUAGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / EGUAGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / EGUAGE_PATTERN_DIVIDE_Y;

	// �e�N�X�`�����W�̐ݒ�
	eguage->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	eguage->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)enemy->HPzan / (float)enemy->HP), (float)(y)* sizeY);
	eguage->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	eguage->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)enemy->HPzan / (float)enemy->HP), (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexEGuage(void)
{
	ENEMY *enemy = GetEnemy(0);

	// ���_���W�̐ݒ�
	eguage->vertexWk[0].vtx = D3DXVECTOR3(eguage->pos.x, eguage->pos.y, eguage->pos.z);
	eguage->vertexWk[1].vtx = D3DXVECTOR3(eguage->pos.x + EGUAGE_SIZE_X * ((float)enemy->HPzan / (float)enemy->HP), eguage->pos.y, eguage->pos.z);
	eguage->vertexWk[2].vtx = D3DXVECTOR3(eguage->pos.x, eguage->pos.y + EGUAGE_SIZE_Y, eguage->pos.z);
	eguage->vertexWk[3].vtx = D3DXVECTOR3(eguage->pos.x + EGUAGE_SIZE_X * ((float)enemy->HPzan / (float)enemy->HP), eguage->pos.y + EGUAGE_SIZE_Y, eguage->pos.z);
}
