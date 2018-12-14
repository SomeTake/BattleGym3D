//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#include "main.h"
#include "score.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureScore = NULL;		// �e�N�X�`���ւ̃|���S��

SCORE score[SCORE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitScore(int type)
{
	int i;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SCORE *score = GetScore(0);

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_SCORE00,				// �t�@�C���̖��O
			&g_pD3DTextureScore);				// �ǂݍ��ރ������̃|�C���^

	}

	for (i = 0; i < SCORE_MAX; i++, score++)
	{
		score->use = true;
		score->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		score->CountAnim = 0;
		score->PatternAnim = 0;

		score->pScore = 0;

		// ���_���̍쐬
		MakeVertexScore(i);

	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitScore(void)
{
	int i;
	SCORE *score = GetScore(0);

	for (i = 0; i < SCORE_MAX; i++, score++)
	{
		if (g_pD3DTextureScore != NULL)	//
		{	// �e�N�X�`���̊J��
			g_pD3DTextureScore->Release();
			g_pD3DTextureScore = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateScore(void)
{
	int i;
	SCORE *score = GetScore(0);

	int kariScore[SCORE_MAX];
	int num = score->pScore;
	
	//�X�R�A���ꌅ���o����
	for (i = 0; i < SCORE_MAX; i++)
	{
		kariScore[i] = num % 10;
		num = num / 10;
	}

	for (i = 0; i < SCORE_MAX; i++, score++)
	{
		if (score->use == true)
		{
			// �A�j���[�V����
			score->CountAnim++;

			//�A�j���[�V����wait�`�F�b�N
			if ((score->CountAnim % TIME_ANIMATION_SCORE) == 0)
			{
				//�p�^�[���̐؂�ւ�
				score->PatternAnim = kariScore[SCORE_MAX - i - 1];

				//�e�N�X�`�����W���Z�b�g
				SetTextureScore(score->PatternAnim, i);
			}


		}
		SetVertexScore(i);
	}

	if (GetKeyboardPress(DIK_SPACE))
	{
		AddScore(1);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	int i;
	SCORE *score = GetScore(0);
	for (i = 0; i < SCORE_MAX; i++, score++)
	{
		if (score->use == true)
		{
			// �e�N�X�`���̐ݒ�(�|���S���̕`��O�ɓǂݍ��񂾃e�N�X�`���̃Z�b�g���s��)
			// �e�N�X�`���̃Z�b�g�����Ȃ��ƑO�ɃZ�b�g���ꂽ�e�N�X�`�����\���遨�����͂�Ȃ����Ƃ��w�肷��pDevide->SetTexture(0, NULL);
			pDevice->SetTexture(0, g_pD3DTextureScore);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, score->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexScore(int sno)
{
	SCORE *score = GetScore(sno);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	SetVertexScore(sno);

	// rhw�̐ݒ�
	score->vertexWk[0].rhw =
		score->vertexWk[1].rhw =
		score->vertexWk[2].rhw =
		score->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	score->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	score->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	score->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	score->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	score->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	score->vertexWk[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	score->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	score->vertexWk[3].tex = D3DXVECTOR2(0.125f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureScore(int cntPattern, int sno)
{
	SCORE *score = GetScore(sno);
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_SCORE_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_SCORE_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_SCORE_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_SCORE_Y;

	// �e�N�X�`�����W�̐ݒ�
	score->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	score->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	score->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	score->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexScore(int sno)
{
	SCORE *score = GetScore(sno);
	// ���_���W�̐ݒ�

	score->vertexWk[0].vtx = D3DXVECTOR3((float)SCORE_POS_X + (TEXTURE_SCORE00_SIZE_X * sno), (float)SCORE_POS_Y, score->pos.z);
	score->vertexWk[1].vtx = D3DXVECTOR3((float)SCORE_POS_X + (TEXTURE_SCORE00_SIZE_X * (sno + 1)), (float)SCORE_POS_Y, score->pos.z);
	score->vertexWk[2].vtx = D3DXVECTOR3((float)SCORE_POS_X + (TEXTURE_SCORE00_SIZE_X * sno), (float)SCORE_POS_Y + TEXTURE_SCORE00_SIZE_Y, score->pos.z);
	score->vertexWk[3].vtx = D3DXVECTOR3((float)SCORE_POS_X + (TEXTURE_SCORE00_SIZE_X * (sno + 1)), (float)SCORE_POS_Y + TEXTURE_SCORE00_SIZE_Y, score->pos.z);
}

//=============================================================================
//�X�R�A�̏����擾����
//=============================================================================
SCORE *GetScore(int sno)
{
	return &score[sno];
}

//=============================================================================
// �X�R�A�̒ǉ�
// ����:add :�ǉ�����_���B�}�C�i�X���\�A�������ȂǂɁB
//=============================================================================
void AddScore(int add)
{
	SCORE *score = GetScore(0);

	score->pScore += add;

	//�J���X�g����
	if (score->pScore > FULL_SCORE)
	{
		score->pScore = FULL_SCORE;
	}
	else if (score->pScore < 0)
	{
		score->pScore = 0;
	}

}
