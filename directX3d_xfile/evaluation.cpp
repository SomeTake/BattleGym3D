//=============================================================================
//
// �X�R�A�]������ [evaluation.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "evaluation.h"
#include "player.h"
#include "enemy.h"
#include "score.h"
#include "escore.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTextureEvaluation = NULL;		// �e�N�X�`���ւ̃|���S��

EVALUATION evaluationWk[EVALUATION_MAX];				// �\����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEvaluation(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_EVALUATION,				// �t�@�C���̖��O
			&g_pD3DTextureEvaluation);			// �ǂݍ��ރ������̃|�C���^
	}

	// ����������
	evaluationWk->pos = D3DXVECTOR3(EVALUATION_POS_X, EVALUATION_POS_Y, 0.0f);	// ���W�f�[�^��������
	evaluationWk->PatternAnim = 0;									// �A�j���p�^�[���ԍ��������_���ŏ�����
	evaluationWk->CountAnim = 0;									// �A�j���J�E���g��������

	evaluationWk->Texture = g_pD3DTextureEvaluation;				// �e�N�X�`�����
	MakeVertexEvaluation();										// ���_���̍쐬

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEvaluation(void)
{
	if (g_pD3DTextureEvaluation != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTextureEvaluation->Release();
		g_pD3DTextureEvaluation = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEvaluation(void)
{
	CHARA *player = GetPlayer();
	CHARA *enemy = GetEnemy();
	ESCORE *escore = GetEScore(0);
	SCORE *score = GetScore(0);

	if (player->HPzan > enemy->HPzan)
	{
		//A�]��
		if (player->score >= SCORE_A)
		{
			SetTextureEvaluation(0);
		}
		//B�]��
		else if (player->score < SCORE_A && player->score >= SCORE_B)
		{
			SetTextureEvaluation(1);
		}
		//C�]��
		else
		{
			SetTextureEvaluation(2);
		}
	}
	else if (player->HPzan < enemy->HPzan)
	{
		//A�]��
		if (enemy->score >= SCORE_A)
		{
			SetTextureEvaluation(0);
		}
		//B�]��
		else if (enemy->score < SCORE_A && enemy->score >= SCORE_B)
		{
			SetTextureEvaluation(1);
		}
		//C�]��
		else
		{
			SetTextureEvaluation(2);
		}

	}
	SetVertexEvaluation();	// �ړ���̍��W�Œ��_��ݒ�

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEvaluation(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, evaluationWk->Texture);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, evaluationWk->vertexWk, sizeof(VERTEX_2D));
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexEvaluation(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	SetVertexEvaluation();

	// rhw�̐ݒ�
	evaluationWk->vertexWk[0].rhw =
		evaluationWk->vertexWk[1].rhw =
		evaluationWk->vertexWk[2].rhw =
		evaluationWk->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	evaluationWk->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	evaluationWk->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	evaluationWk->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	evaluationWk->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	evaluationWk->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	evaluationWk->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_EVALUATION, 0.0f);
	evaluationWk->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_EVALUATION);
	evaluationWk->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_EVALUATION, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_EVALUATION);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureEvaluation(int cntPattern)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_EVALUATION;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_EVALUATION;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_EVALUATION;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_EVALUATION;

	evaluationWk->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	evaluationWk->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	evaluationWk->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	evaluationWk->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexEvaluation(void)
{
	// ���_���W�̐ݒ�
	evaluationWk->vertexWk[0].vtx = D3DXVECTOR3(evaluationWk->pos.x, evaluationWk->pos.y, evaluationWk->pos.z);
	evaluationWk->vertexWk[1].vtx = D3DXVECTOR3(evaluationWk->pos.x + TEXTURE_EVALUATION_SIZE_X, evaluationWk->pos.y, evaluationWk->pos.z);
	evaluationWk->vertexWk[2].vtx = D3DXVECTOR3(evaluationWk->pos.x, evaluationWk->pos.y + TEXTURE_EVALUATION_SIZE_Y, evaluationWk->pos.z);
	evaluationWk->vertexWk[3].vtx = D3DXVECTOR3(evaluationWk->pos.x + TEXTURE_EVALUATION_SIZE_X, evaluationWk->pos.y + TEXTURE_EVALUATION_SIZE_Y, evaluationWk->pos.z);
}

