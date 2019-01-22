//=============================================================================
//
// 2P�p�ԃQ�[�W��ʏ��� [eredguage.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "eredguage.h"
#include "enemy.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureERedGuage = NULL;		// �e�N�X�`���ւ̃|�C���^

EREDGUAGE eredguage[EREDGUAGE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitERedGuage(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CHARA *enemy = GetEnemy();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_EREDGUAGE,				// �t�@�C���̖��O
			&g_pD3DTextureERedGuage);		// �ǂݍ��ރ������[

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// �Q�[�W�̏�����
	eredguage->use = true;
	eredguage->pos = D3DXVECTOR3(EREDGUAGE_POS_X, EREDGUAGE_POS_Y, 0.0f);
	eredguage->value = enemy->HPzan;

	// ���_���̍쐬
	MakeVertexERedGuage();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitERedGuage(void)
{
	if (g_pD3DTextureERedGuage != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureERedGuage->Release();
		g_pD3DTextureERedGuage = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateERedGuage(void)
{
	if (eredguage->use == true)
	{
		//�ԃQ�[�W
		eredguage->PatternAnim = 2;
		
		//�e�N�X�`�����W���Z�b�g
		SetTextureERedGuage(eredguage->PatternAnim);


		SetVertexERedGuage();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawERedGuage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (eredguage->use == true)
	{

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureERedGuage);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, eredguage->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexERedGuage(void)
{

	// ���_���W�̐ݒ�
	SetVertexERedGuage();

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	eredguage->vertexWk[0].rhw =
		eredguage->vertexWk[1].rhw =
		eredguage->vertexWk[2].rhw =
		eredguage->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	eredguage->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	eredguage->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	eredguage->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	eredguage->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	eredguage->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	eredguage->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	eredguage->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	eredguage->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureERedGuage(int cntPattern)
{
	CHARA *enemy = GetEnemy();

	if (eredguage->value > enemy->HPzan)
	{
		eredguage->value -= 5;
	}

	//�g���[�j���O���[�h�Ȃǂŉ񕜂������p
	if (enemy->HPzan > eredguage->value)
	{
		eredguage->value = enemy->HPzan;
	}

	int x = cntPattern % EREDGUAGE_PATTERN_DIVIDE_X;
	int y = cntPattern / EREDGUAGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / EREDGUAGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / EREDGUAGE_PATTERN_DIVIDE_Y;

	// �e�N�X�`�����W�̐ݒ�
	eredguage->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	eredguage->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)eredguage->value / (float)enemy->HP), (float)(y)* sizeY);
	eredguage->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	eredguage->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)eredguage->value / (float)enemy->HP), (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexERedGuage(void)
{
	CHARA *enemy = GetEnemy();

	// ���_���W�̐ݒ�
	eredguage->vertexWk[0].vtx = D3DXVECTOR3(eredguage->pos.x, eredguage->pos.y, eredguage->pos.z);
	eredguage->vertexWk[1].vtx = D3DXVECTOR3(eredguage->pos.x + EREDGUAGE_SIZE_X * ((float)eredguage->value / (float)enemy->HP), eredguage->pos.y, eredguage->pos.z);
	eredguage->vertexWk[2].vtx = D3DXVECTOR3(eredguage->pos.x, eredguage->pos.y + EREDGUAGE_SIZE_Y, eredguage->pos.z);
	eredguage->vertexWk[3].vtx = D3DXVECTOR3(eredguage->pos.x + EREDGUAGE_SIZE_X * ((float)eredguage->value / (float)enemy->HP), eredguage->pos.y + EREDGUAGE_SIZE_Y, eredguage->pos.z);
}

EREDGUAGE *GetERedGuage(int eno)
{
	return &eredguage[eno];
}
