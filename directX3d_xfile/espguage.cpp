//=============================================================================
//
// 2P�pSP�Q�[�W��ʏ��� [espguage.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "espguage.h"
#include "player.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexESpGuage(void);
void SetTextureESpGuage(int cntPattern);
void SetVertexESpGuage(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureESpGuage = NULL;		// �e�N�X�`���ւ̃|�C���^

ESPGUAGE espguage[ESPGUAGE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitESpGuage(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_ESPGUAGE,				// �t�@�C���̖��O
			&g_pD3DTextureESpGuage);		// �ǂݍ��ރ������[

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// �Q�[�W�̏�����
	ESPGUAGE *espguage = GetESpGuage(0);
	espguage->use = true;
	espguage->pos = D3DXVECTOR3(ESPGUAGE_POS_X, ESPGUAGE_POS_Y, 0.0f);
	espguage->sp = 0;
	espguage->PatternAnim = 0;
	espguage->CountAnim = 0;

	// ���_���̍쐬
	MakeVertexESpGuage();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitESpGuage(void)
{
	if (g_pD3DTextureESpGuage != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureESpGuage->Release();
		g_pD3DTextureESpGuage = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateESpGuage(void)
{
	PLAYER *player = GetPlayer(0);
	ESPGUAGE *espguage = GetESpGuage(0);

	if (espguage->use == true)
	{
		espguage->PatternAnim = 1;

		//�e�N�X�`�����W���Z�b�g
		SetTextureESpGuage(espguage->PatternAnim);

		SetVertexESpGuage();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawESpGuage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ESPGUAGE *espguage = GetESpGuage(0);

	if (espguage->use == true)
	{

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureESpGuage);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, espguage->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexESpGuage(void)
{

	// ���_���W�̐ݒ�
	SetVertexESpGuage();

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	espguage->vertexWk[0].rhw =
		espguage->vertexWk[1].rhw =
		espguage->vertexWk[2].rhw =
		espguage->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	espguage->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	espguage->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	espguage->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	espguage->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	espguage->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	espguage->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	espguage->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	espguage->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureESpGuage(int cntPattern)
{

	int x = cntPattern % ESPGUAGE_PATTERN_DIVIDE_X;
	int y = cntPattern / ESPGUAGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / ESPGUAGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / ESPGUAGE_PATTERN_DIVIDE_Y;

	// �e�N�X�`�����W�̐ݒ�
	espguage->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	espguage->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	espguage->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	espguage->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexESpGuage(void)
{
	// ���_���W�̐ݒ�
	espguage->vertexWk[0].vtx = D3DXVECTOR3(espguage->pos.x + ESPGUAGE_SIZE_X - ESPGUAGE_SIZE_X * ((float)espguage->sp / (float)FULL_ESPGUAGE), espguage->pos.y, espguage->pos.z);
	espguage->vertexWk[1].vtx = D3DXVECTOR3(espguage->pos.x + ESPGUAGE_SIZE_X, espguage->pos.y, espguage->pos.z);
	espguage->vertexWk[2].vtx = D3DXVECTOR3(espguage->pos.x + ESPGUAGE_SIZE_X - ESPGUAGE_SIZE_X * ((float)espguage->sp / (float)FULL_ESPGUAGE), espguage->pos.y + ESPGUAGE_SIZE_Y, espguage->pos.z);
	espguage->vertexWk[3].vtx = D3DXVECTOR3(espguage->pos.x + ESPGUAGE_SIZE_X, espguage->pos.y + ESPGUAGE_SIZE_Y, espguage->pos.z);
}

//=============================================================================
// �G�l�~�[SP�Q�[�W�̃Q�b�^�[
//=============================================================================
ESPGUAGE *GetESpGuage(int gno)
{
	return &espguage[gno];
}

//=============================================================================
// SP�Q�[�W�̑���
// ����:add :�ǉ�����_���B�}�C�i�X���\�A�������ȂǂɁB
//=============================================================================
void AddESpGuage(int add)
{
	ESPGUAGE *espguage = GetESpGuage(0);
	espguage->sp += add;

	//�J���X�g����
	if (espguage->sp > FULL_ESPGUAGE)
	{
		espguage->sp = FULL_ESPGUAGE;
	}
	else if (espguage->sp < 0)
	{
		espguage->sp = 0;
	}

}
