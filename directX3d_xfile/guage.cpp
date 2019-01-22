//=============================================================================
//
// HP�Q�[�W��ʏ��� [guage.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "guage.h"
#include "player.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureGuage = NULL;		// �e�N�X�`���ւ̃|�C���^

GUAGE guage[GUAGE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGuage(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_GUAGE,				// �t�@�C���̖��O
			&g_pD3DTextureGuage);		// �ǂݍ��ރ������[

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// �Q�[�W�̏�����
	guage->use = true;
	guage->pos = D3DXVECTOR3(GUAGE_POS_X, GUAGE_POS_Y, 0.0f);

	// ���_���̍쐬
	MakeVertexGuage();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGuage(void)
{
	if (g_pD3DTextureGuage != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureGuage->Release();
		g_pD3DTextureGuage = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGuage(void)
{
	CHARA *player = GetPlayer();

	if (guage->use == true)
	{
		//HP���^���̏ꍇ
		if (player->HPzan == player->HP)
		{
			guage->PatternAnim = 0;
		}
		//100%�����̏ꍇ
		else if (player->HPzan < player->HP)
		{
			guage->PatternAnim = 1;
		}

		//�e�N�X�`�����W���Z�b�g
		SetTextureGuage(guage->PatternAnim);

		SetVertexGuage();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGuage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (guage->use == true)
	{

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureGuage);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, guage->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexGuage(void)
{

	// ���_���W�̐ݒ�
	SetVertexGuage();

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	guage->vertexWk[0].rhw =
		guage->vertexWk[1].rhw =
		guage->vertexWk[2].rhw =
		guage->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	guage->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	guage->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	guage->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	guage->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	guage->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	guage->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	guage->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	guage->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureGuage(int cntPattern)
{
	CHARA *player = GetPlayer();

	int x = cntPattern % GUAGE_PATTERN_DIVIDE_X;
	int y = cntPattern / GUAGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / GUAGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / GUAGE_PATTERN_DIVIDE_Y;

	// �e�N�X�`�����W�̐ݒ�
	guage->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)(player->HP - player->HPzan) / (float)player->HP), (float)(y)* sizeY);
	guage->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	guage->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)(player->HP - player->HPzan) / (float)player->HP), (float)(y)* sizeY + sizeY);
	guage->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexGuage(void)
{
	CHARA *player = GetPlayer();

	// ���_���W�̐ݒ�
	guage->vertexWk[0].vtx = D3DXVECTOR3(guage->pos.x + GUAGE_SIZE_X * ((float)(player->HP - player->HPzan) / player->HP), guage->pos.y, guage->pos.z);
	guage->vertexWk[1].vtx = D3DXVECTOR3(guage->pos.x + GUAGE_SIZE_X, guage->pos.y, guage->pos.z);
	guage->vertexWk[2].vtx = D3DXVECTOR3(guage->pos.x + GUAGE_SIZE_X * ((float)(player->HP - player->HPzan) / player->HP), guage->pos.y + GUAGE_SIZE_Y, guage->pos.z);
	guage->vertexWk[3].vtx = D3DXVECTOR3(guage->pos.x + GUAGE_SIZE_X, guage->pos.y + GUAGE_SIZE_Y, guage->pos.z);
}
