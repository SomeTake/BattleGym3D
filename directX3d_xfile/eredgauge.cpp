//=============================================================================
//
// 2P�p�ԃQ�[�W��ʏ��� [eredgauge.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "eredgauge.h"
#include "enemy.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureERedGauge = NULL;		// �e�N�X�`���ւ̃|�C���^

EREDGAUGE eredgauge[EREDGAUGE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitERedGauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CHARA *enemy = GetEnemy();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_EREDGAUGE,				// �t�@�C���̖��O
			&g_pD3DTextureERedGauge);		// �ǂݍ��ރ������[

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// �Q�[�W�̏�����
	eredgauge->use = true;
	eredgauge->pos = D3DXVECTOR3(EREDGAUGE_POS_X, EREDGAUGE_POS_Y, 0.0f);
	eredgauge->value = enemy->HPzan;

	// ���_���̍쐬
	MakeVertexERedGauge();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitERedGauge(void)
{
	if (g_pD3DTextureERedGauge != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureERedGauge->Release();
		g_pD3DTextureERedGauge = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateERedGauge(void)
{
	if (eredgauge->use == true)
	{
		//�ԃQ�[�W
		eredgauge->PatternAnim = 2;
		
		//�e�N�X�`�����W���Z�b�g
		SetTextureERedGauge(eredgauge->PatternAnim);


		SetVertexERedGauge();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawERedGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (eredgauge->use == true)
	{

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureERedGauge);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, eredgauge->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexERedGauge(void)
{

	// ���_���W�̐ݒ�
	SetVertexERedGauge();

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	eredgauge->vertexWk[0].rhw =
		eredgauge->vertexWk[1].rhw =
		eredgauge->vertexWk[2].rhw =
		eredgauge->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	eredgauge->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	eredgauge->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	eredgauge->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	eredgauge->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	eredgauge->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	eredgauge->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	eredgauge->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	eredgauge->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureERedGauge(int cntPattern)
{
	CHARA *enemy = GetEnemy();

	if (eredgauge->value > enemy->HPzan)
	{
		eredgauge->value -= 5;
	}

	//�g���[�j���O���[�h�Ȃǂŉ񕜂������p
	if (enemy->HPzan > eredgauge->value)
	{
		eredgauge->value = enemy->HPzan;
	}

	int x = cntPattern % EREDGAUGE_PATTERN_DIVIDE_X;
	int y = cntPattern / EREDGAUGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / EREDGAUGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / EREDGAUGE_PATTERN_DIVIDE_Y;

	// �e�N�X�`�����W�̐ݒ�
	eredgauge->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	eredgauge->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)eredgauge->value / (float)enemy->HP), (float)(y)* sizeY);
	eredgauge->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	eredgauge->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)eredgauge->value / (float)enemy->HP), (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexERedGauge(void)
{
	CHARA *enemy = GetEnemy();

	// ���_���W�̐ݒ�
	eredgauge->vertexWk[0].vtx = D3DXVECTOR3(eredgauge->pos.x, eredgauge->pos.y, eredgauge->pos.z);
	eredgauge->vertexWk[1].vtx = D3DXVECTOR3(eredgauge->pos.x + EREDGAUGE_SIZE_X * ((float)eredgauge->value / (float)enemy->HP), eredgauge->pos.y, eredgauge->pos.z);
	eredgauge->vertexWk[2].vtx = D3DXVECTOR3(eredgauge->pos.x, eredgauge->pos.y + EREDGAUGE_SIZE_Y, eredgauge->pos.z);
	eredgauge->vertexWk[3].vtx = D3DXVECTOR3(eredgauge->pos.x + EREDGAUGE_SIZE_X * ((float)eredgauge->value / (float)enemy->HP), eredgauge->pos.y + EREDGAUGE_SIZE_Y, eredgauge->pos.z);
}

EREDGAUGE *GetERedGauge(int eno)
{
	return &eredgauge[eno];
}
