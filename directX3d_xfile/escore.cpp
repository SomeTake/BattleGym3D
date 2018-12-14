//=============================================================================
//
// �G�l�~�[�̃X�R�A���� [escore.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
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
static LPDIRECT3DTEXTURE9		g_pD3DTextureEScore = NULL;		// �e�N�X�`���ւ̃|���S��

ESCORE EScore[ESCORE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEScore(int type)
{
	int i;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ESCORE *escore = GetEScore(0);

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_ESCORE00,				// �t�@�C���̖��O
			&g_pD3DTextureEScore);				// �ǂݍ��ރ������̃|�C���^

	}

	for (i = 0; i < ESCORE_MAX; i++, escore++)
	{
		escore->use = true;
		escore->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		escore->CountAnim = 0;
		escore->PatternAnim = 0;

		escore->eScore = 0;

		// ���_���̍쐬
		MakeVertexEScore(i);

	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEScore(void)
{
	int i;
	ESCORE *escore = GetEScore(0);

	for (i = 0; i < ESCORE_MAX; i++, escore++)
	{
		if (g_pD3DTextureEScore != NULL)	//
		{	// �e�N�X�`���̊J��
			g_pD3DTextureEScore->Release();
			g_pD3DTextureEScore = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEScore(void)
{
	int i;
	ESCORE *escore = GetEScore(0);

	int kariScore[ESCORE_MAX];
	int num = escore->eScore;

	//�X�R�A���ꌅ���o����
	for (i = 0; i < ESCORE_MAX; i++)
	{
		kariScore[i] = num % 10;
		num = num / 10;
	}

	for (i = 0; i < ESCORE_MAX; i++, escore++)
	{
		if (escore->use == true)
		{
			// �A�j���[�V����
			escore->CountAnim++;

			//�A�j���[�V����wait�`�F�b�N
			if ((escore->CountAnim % TIME_ANIMATION_ESCORE) == 0)
			{
				//�p�^�[���̐؂�ւ�
				escore->PatternAnim = kariScore[ESCORE_MAX - i - 1];

				//�e�N�X�`�����W���Z�b�g
				SetTextureEScore(escore->PatternAnim, i);
			}


		}
		SetVertexEScore(i);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	int i;
	ESCORE *escore = GetEScore(0);
	for (i = 0; i < ESCORE_MAX; i++, escore++)
	{
		if (escore->use == true)
		{
			// �e�N�X�`���̐ݒ�(�|���S���̕`��O�ɓǂݍ��񂾃e�N�X�`���̃Z�b�g���s��)
			// �e�N�X�`���̃Z�b�g�����Ȃ��ƑO�ɃZ�b�g���ꂽ�e�N�X�`�����\���遨�����͂�Ȃ����Ƃ��w�肷��pDevide->SetTexture(0, NULL);
			pDevice->SetTexture(0, g_pD3DTextureEScore);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, escore->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexEScore(int sno)
{
	ESCORE *escore = GetEScore(sno);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	SetVertexEScore(sno);

	// rhw�̐ݒ�
	escore->vertexWk[0].rhw =
		escore->vertexWk[1].rhw =
		escore->vertexWk[2].rhw =
		escore->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	escore->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	escore->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	escore->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	escore->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	escore->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	escore->vertexWk[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	escore->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	escore->vertexWk[3].tex = D3DXVECTOR2(0.125f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureEScore(int cntPattern, int sno)
{
	ESCORE *escore = GetEScore(sno);
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_ESCORE_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_ESCORE_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_ESCORE_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_ESCORE_Y;

	// �e�N�X�`�����W�̐ݒ�
	escore->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	escore->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	escore->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	escore->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexEScore(int sno)
{
	ESCORE *escore = GetEScore(sno);

	int phase = *GetPhase();

	//���_���W�̐ݒ�
	if (phase == PhaseCountdown || phase == PhaseGame || phase == PhaseFinish)
	{
		escore->vertexWk[0].vtx = D3DXVECTOR3((float)ESCORE_POS_X + (TEXTURE_ESCORE00_SIZE_X * sno), (float)ESCORE_POS_Y, escore->pos.z);
		escore->vertexWk[1].vtx = D3DXVECTOR3((float)ESCORE_POS_X + (TEXTURE_ESCORE00_SIZE_X * (sno + 1)), (float)ESCORE_POS_Y, escore->pos.z);
		escore->vertexWk[2].vtx = D3DXVECTOR3((float)ESCORE_POS_X + (TEXTURE_ESCORE00_SIZE_X * sno), (float)ESCORE_POS_Y + TEXTURE_ESCORE00_SIZE_Y, escore->pos.z);
		escore->vertexWk[3].vtx = D3DXVECTOR3((float)ESCORE_POS_X + (TEXTURE_ESCORE00_SIZE_X * (sno + 1)), (float)ESCORE_POS_Y + TEXTURE_ESCORE00_SIZE_Y, escore->pos.z);
	}
	else if (phase == PhaseResult)
	{
		escore->vertexWk[0].vtx = D3DXVECTOR3((float)RESULT_ESCORE_POS_X + (RESULT_ESCORE00_SIZE_X * sno), (float)RESULT_ESCORE_POS_Y, escore->pos.z);
		escore->vertexWk[1].vtx = D3DXVECTOR3((float)RESULT_ESCORE_POS_X + (RESULT_ESCORE00_SIZE_X * (sno + 1)), (float)RESULT_ESCORE_POS_Y, escore->pos.z);
		escore->vertexWk[2].vtx = D3DXVECTOR3((float)RESULT_ESCORE_POS_X + (RESULT_ESCORE00_SIZE_X * sno), (float)RESULT_ESCORE_POS_Y + RESULT_ESCORE00_SIZE_Y, escore->pos.z);
		escore->vertexWk[3].vtx = D3DXVECTOR3((float)RESULT_ESCORE_POS_X + (RESULT_ESCORE00_SIZE_X * (sno + 1)), (float)RESULT_ESCORE_POS_Y + RESULT_ESCORE00_SIZE_Y, escore->pos.z);
	}
}

//=============================================================================
//�X�R�A�̏����擾����
//=============================================================================
ESCORE *GetEScore(int sno)
{
	return &EScore[sno];
}

//=============================================================================
// �X�R�A�̒ǉ�
// ����:add :�ǉ�����_���B�}�C�i�X���\�A�������ȂǂɁB
//=============================================================================
void AddEScore(int add)
{
	ESCORE *escore = GetEScore(0);
	escore->eScore += add;

	//�J���X�g����
	if (escore->eScore > FULL_ESCORE)
	{
		escore->eScore = FULL_ESCORE;
	}
	else if (escore->eScore < 0)
	{
		escore->eScore = 0;
	}

}
