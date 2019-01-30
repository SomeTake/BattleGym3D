//=============================================================================
//
// �v���C���[�p�ԃQ�[�W��ʏ��� [redgauge.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "redgauge.h"
#include "player.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureRedGauge = NULL;		// �e�N�X�`���ւ̃|�C���^

REDGAUGE redgauge[REDGAUGE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitRedGauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CHARA *player = GetPlayer();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_REDGAUGE,				// �t�@�C���̖��O
			&g_pD3DTextureRedGauge);		// �ǂݍ��ރ������[

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// �Q�[�W�̏�����
	redgauge->use = true;
	redgauge->pos = D3DXVECTOR3(REDGAUGE_POS_X, REDGAUGE_POS_Y, 0.0f);
	redgauge->value = player->HPzan;

	// ���_���̍쐬
	MakeVertexRedGauge();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitRedGauge(void)
{
	if (g_pD3DTextureRedGauge != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureRedGauge->Release();
		g_pD3DTextureRedGauge = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateRedGauge(void)
{
	if (redgauge->use == true)
	{
		//�ԃQ�[�W
		redgauge->PatternAnim = 2;

		//�e�N�X�`�����W���Z�b�g
		SetTextureRedGauge(redgauge->PatternAnim);


		SetVertexRedGauge();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawRedGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (redgauge->use == true)
	{

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureRedGauge);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, redgauge->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexRedGauge(void)
{

	// ���_���W�̐ݒ�
	SetVertexRedGauge();

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	redgauge->vertexWk[0].rhw =
		redgauge->vertexWk[1].rhw =
		redgauge->vertexWk[2].rhw =
		redgauge->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	redgauge->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	redgauge->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	redgauge->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	redgauge->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	redgauge->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	redgauge->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	redgauge->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	redgauge->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureRedGauge(int cntPattern)
{
	CHARA *player = GetPlayer();

	if (redgauge->value > player->HPzan)
	{
		redgauge->value -= 5;
	}

	//�g���[�j���O���[�h�Ȃǂŉ񕜂������p
	if (player->HPzan > redgauge->value)
	{
		redgauge->value = player->HPzan;
	}

	int x = cntPattern % REDGAUGE_PATTERN_DIVIDE_X;
	int y = cntPattern / REDGAUGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / REDGAUGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / REDGAUGE_PATTERN_DIVIDE_Y;

	// �e�N�X�`�����W�̐ݒ�
	redgauge->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)(player->HP - redgauge->value) / (float)player->HP), (float)(y)* sizeY);
	redgauge->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	redgauge->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)(player->HP - redgauge->value) / (float)player->HP), (float)(y)* sizeY + sizeY);
	redgauge->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexRedGauge(void)
{
	CHARA *player = GetPlayer();

	// ���_���W�̐ݒ�
	redgauge->vertexWk[0].vtx = D3DXVECTOR3(redgauge->pos.x + REDGAUGE_SIZE_X * ((float)(player->HP - redgauge->value) / player->HP), redgauge->pos.y, redgauge->pos.z);
	redgauge->vertexWk[1].vtx = D3DXVECTOR3(redgauge->pos.x + REDGAUGE_SIZE_X, redgauge->pos.y, redgauge->pos.z);
	redgauge->vertexWk[2].vtx = D3DXVECTOR3(redgauge->pos.x + REDGAUGE_SIZE_X * ((float)(player->HP - redgauge->value) / player->HP), redgauge->pos.y + REDGAUGE_SIZE_Y, redgauge->pos.z);
	redgauge->vertexWk[3].vtx = D3DXVECTOR3(redgauge->pos.x + REDGAUGE_SIZE_X, redgauge->pos.y + REDGAUGE_SIZE_Y, redgauge->pos.z);
}
