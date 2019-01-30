//=============================================================================
//
// HP�Q�[�W��ʏ��� [gauge.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "gauge.h"
#include "player.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureGauge = NULL;		// �e�N�X�`���ւ̃|�C���^

GAUGE gauge[GAUGE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_GAUGE,				// �t�@�C���̖��O
			&g_pD3DTextureGauge);		// �ǂݍ��ރ������[

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// �Q�[�W�̏�����
	gauge->use = true;
	gauge->pos = D3DXVECTOR3(GAUGE_POS_X, GAUGE_POS_Y, 0.0f);

	// ���_���̍쐬
	MakeVertexGauge();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGauge(void)
{
	if (g_pD3DTextureGauge != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureGauge->Release();
		g_pD3DTextureGauge = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGauge(void)
{
	CHARA *player = GetPlayer();

	if (gauge->use == true)
	{
		//HP���^���̏ꍇ
		if (player->HPzan == player->HP)
		{
			gauge->PatternAnim = 0;
		}
		//100%�����̏ꍇ
		else if (player->HPzan < player->HP)
		{
			gauge->PatternAnim = 1;
		}

		//�e�N�X�`�����W���Z�b�g
		SetTextureGauge(gauge->PatternAnim);

		SetVertexGauge();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (gauge->use == true)
	{

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureGauge);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, gauge->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexGauge(void)
{

	// ���_���W�̐ݒ�
	SetVertexGauge();

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	gauge->vertexWk[0].rhw =
		gauge->vertexWk[1].rhw =
		gauge->vertexWk[2].rhw =
		gauge->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	gauge->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	gauge->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	gauge->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	gauge->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	gauge->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	gauge->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	gauge->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	gauge->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureGauge(int cntPattern)
{
	CHARA *player = GetPlayer();

	int x = cntPattern % GAUGE_PATTERN_DIVIDE_X;
	int y = cntPattern / GAUGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / GAUGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / GAUGE_PATTERN_DIVIDE_Y;

	// �e�N�X�`�����W�̐ݒ�
	gauge->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)(player->HP - player->HPzan) / (float)player->HP), (float)(y)* sizeY);
	gauge->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	gauge->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)(player->HP - player->HPzan) / (float)player->HP), (float)(y)* sizeY + sizeY);
	gauge->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexGauge(void)
{
	CHARA *player = GetPlayer();

	// ���_���W�̐ݒ�
	gauge->vertexWk[0].vtx = D3DXVECTOR3(gauge->pos.x + GAUGE_SIZE_X * ((float)(player->HP - player->HPzan) / player->HP), gauge->pos.y, gauge->pos.z);
	gauge->vertexWk[1].vtx = D3DXVECTOR3(gauge->pos.x + GAUGE_SIZE_X, gauge->pos.y, gauge->pos.z);
	gauge->vertexWk[2].vtx = D3DXVECTOR3(gauge->pos.x + GAUGE_SIZE_X * ((float)(player->HP - player->HPzan) / player->HP), gauge->pos.y + GAUGE_SIZE_Y, gauge->pos.z);
	gauge->vertexWk[3].vtx = D3DXVECTOR3(gauge->pos.x + GAUGE_SIZE_X, gauge->pos.y + GAUGE_SIZE_Y, gauge->pos.z);
}
