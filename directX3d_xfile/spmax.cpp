//=============================================================================
//
// SP�Q�[�WMAX�\������ [spmax.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "spmax.h"
#include "spgauge.h"
#include "espgauge.h"
#include "player.h"
#include "enemy.h"
#include "sound.h"
#include "battle.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureSpmax = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureESpmax = NULL;				// �e�N�X�`���ւ̃|�C���^

VERTEX_2D				g_vertexWkSpmax[NUM_VERTEX];			// ���_���i�[���[�N
VERTEX_2D				g_vertexWkESpmax[NUM_VERTEX];			// ���_���i�[���[�N

bool					SpmaxUse;								// �g�p����
bool					ESpmaxUse;								// �g�p����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSpmax(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_SPMAX,				// �t�@�C���̖��O
			&g_pD3DTextureSpmax);		// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_SPMAX,			// �t�@�C���̖��O
			&g_pD3DTextureESpmax);	// �ǂݍ��ރ������[
	}

	SpmaxUse = false;
	ESpmaxUse = false;

	// ���_���̍쐬
	MakeVertexSpmax();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSpmax(void)
{
	if (g_pD3DTextureSpmax != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureSpmax->Release();
		g_pD3DTextureSpmax = NULL;
	}

	if (g_pD3DTextureESpmax != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureESpmax->Release();
		g_pD3DTextureESpmax = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSpmax(void)
{
	CHARA *enemyWk = GetEnemy();
	CHARA *playerWk = GetPlayer();

	//����炷���߂̃t���O
	static bool soundflag = false;
	static bool esoundflag = false;

	int phase = *GetPhase();

	//�v���C���[��SP�Q�[�W��MAX�̏ꍇ�g��
	if (playerWk->SP == FULL_SPGAUGE)
	{
		SpmaxUse = true;
		//MAX�ɂȂ����u�ԉ���炷
		if (soundflag == false && phase == PhaseGame)
		{
			PlaySound(SE_EFFECT0, 0, 0);
			soundflag = true;
		}
	}
	else if (playerWk->SP < FULL_SPGAUGE)
	{
		SpmaxUse = false;
		if (phase == PhaseGame)
		{
			soundflag = false;
		}
	}

	//�G�l�~�[��SP�Q�[�W��MAX�̏ꍇ�g��
	if (enemyWk->SP == FULL_SPGAUGE)
	{
		ESpmaxUse = true;
		//MAX�ɂȂ����u�ԉ���炷
		if (esoundflag == false && phase == PhaseGame)
		{
			PlaySound(SE_EFFECT0, 0, 0);
			esoundflag = true;
		}
	}
	else if (enemyWk->SP < FULL_SPGAUGE)
	{
		ESpmaxUse = false;
		if (phase == PhaseGame)
		{
			esoundflag = false;
		}
	}
	SetTextureSpmax();

	SetVertexSpmax();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSpmax(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SPGAUGE *spgauge = GetSpGauge(0);
	ESPGAUGE *espgauge = GetESpGauge(0);

	//�v���C���[��SP�Q�[�W��MAX�Ȃ�`��
	if (SpmaxUse == true)
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureSpmax);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkSpmax, sizeof(VERTEX_2D));
	}

	//�G�l�~�[��SP�Q�[�W��MAX�Ȃ�`��
	if (ESpmaxUse == true)
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureESpmax);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkESpmax, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexSpmax(void)
{
	// �v���C���[�p
	// ���_���W�̐ݒ�
	g_vertexWkSpmax[0].vtx = D3DXVECTOR3(SPMAX_POS_X, SPMAX_POS_Y, 0.0f);
	g_vertexWkSpmax[1].vtx = D3DXVECTOR3(SPMAX_POS_X + SPMAX_SIZE_X, SPMAX_POS_Y, 0.0f);
	g_vertexWkSpmax[2].vtx = D3DXVECTOR3(SPMAX_POS_X, SPMAX_POS_Y + SPMAX_SIZE_Y, 0.0f);
	g_vertexWkSpmax[3].vtx = D3DXVECTOR3(SPMAX_POS_X + SPMAX_SIZE_X, SPMAX_POS_Y + SPMAX_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkSpmax[0].rhw =
		g_vertexWkSpmax[1].rhw =
		g_vertexWkSpmax[2].rhw =
		g_vertexWkSpmax[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkSpmax[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkSpmax[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkSpmax[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkSpmax[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkSpmax[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkSpmax[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkSpmax[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkSpmax[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// �G�l�~�[�p
	// ���_���W�̐ݒ�
	g_vertexWkESpmax[0].vtx = D3DXVECTOR3(ESPMAX_POS_X, ESPMAX_POS_Y, 0.0f);
	g_vertexWkESpmax[1].vtx = D3DXVECTOR3(ESPMAX_POS_X + SPMAX_SIZE_X, ESPMAX_POS_Y, 0.0f);
	g_vertexWkESpmax[2].vtx = D3DXVECTOR3(ESPMAX_POS_X, ESPMAX_POS_Y + SPMAX_SIZE_Y, 0.0f);
	g_vertexWkESpmax[3].vtx = D3DXVECTOR3(ESPMAX_POS_X + SPMAX_SIZE_X, ESPMAX_POS_Y + SPMAX_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkESpmax[0].rhw =
		g_vertexWkESpmax[1].rhw =
		g_vertexWkESpmax[2].rhw =
		g_vertexWkESpmax[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkESpmax[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkESpmax[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkESpmax[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkESpmax[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkESpmax[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkESpmax[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkESpmax[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkESpmax[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}


//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureSpmax(void)
{
	// �v���C���[�p
	g_vertexWkSpmax[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkSpmax[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkSpmax[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkSpmax[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// �G�l�~�[�p
	g_vertexWkESpmax[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkESpmax[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkESpmax[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkESpmax[3].tex = D3DXVECTOR2(1.0f, 1.0f);

}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexSpmax(void)
{
	// �v���C���[�p
	g_vertexWkSpmax[0].vtx = D3DXVECTOR3(SPMAX_POS_X, SPMAX_POS_Y, 0.0f);
	g_vertexWkSpmax[1].vtx = D3DXVECTOR3(SPMAX_POS_X + SPMAX_SIZE_X, SPMAX_POS_Y, 0.0f);
	g_vertexWkSpmax[2].vtx = D3DXVECTOR3(SPMAX_POS_X, SPMAX_POS_Y + SPMAX_SIZE_Y, 0.0f);
	g_vertexWkSpmax[3].vtx = D3DXVECTOR3(SPMAX_POS_X + SPMAX_SIZE_X, SPMAX_POS_Y + SPMAX_SIZE_Y, 0.0f);

	// �G�l�~�[�p
	g_vertexWkESpmax[0].vtx = D3DXVECTOR3(ESPMAX_POS_X, ESPMAX_POS_Y, 0.0f);
	g_vertexWkESpmax[1].vtx = D3DXVECTOR3(ESPMAX_POS_X + SPMAX_SIZE_X, ESPMAX_POS_Y, 0.0f);
	g_vertexWkESpmax[2].vtx = D3DXVECTOR3(ESPMAX_POS_X, ESPMAX_POS_Y + SPMAX_SIZE_Y, 0.0f);
	g_vertexWkESpmax[3].vtx = D3DXVECTOR3(ESPMAX_POS_X + SPMAX_SIZE_X, ESPMAX_POS_Y + SPMAX_SIZE_Y, 0.0f);
}