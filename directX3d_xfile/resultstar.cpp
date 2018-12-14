//=============================================================================
//
// ���U���g��ʂ̃X�^�[�\������ [resultstar.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "resultstar.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureResultstar = NULL;		// �e�N�X�`���ւ̃|���S��

RESULTSTAR resultstar[STAR_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitResultstar(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			STAR00,				// �t�@�C���̖��O
			&g_pD3DTextureResultstar);				// �ǂݍ��ރ������̃|�C���^

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// ���U���g�X�^�[�̏�����
	resultstar->use = true;
	resultstar->pos = D3DXVECTOR3(STAR_SIZE_X / 2, STAR_SIZE_Y / 2, 0.0f);
	resultstar->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	resultstar->angle = atan2f(resultstar->pos.y - 0.0f, resultstar->pos.x - 0.0f);
	D3DXVECTOR2 temp = D3DXVECTOR2(STAR_SIZE_X / 2, STAR_SIZE_Y / 2);
	resultstar->radius = D3DXVec2Length(&temp);
	resultstar->CountAnim = 0;
	resultstar->PatternAnim = 0;

	// ���_���̍쐬
	MakeVertexResultstar();

	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitResultstar(void)
{
	if (g_pD3DTextureResultstar != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTextureResultstar->Release();
		g_pD3DTextureResultstar = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResultstar(void)
{
	if (resultstar->use == true)
	{
		// �A�j���[�V����
		resultstar->CountAnim++;

		//�A�j���[�V����wait�`�F�b�N
		if ((resultstar->CountAnim % TIME_ANIMATION_STAR) == 0)
		{
			//�p�^�[���̐؂�ւ�
			resultstar->PatternAnim = 1;

			//�e�N�X�`�����W���Z�b�g
			SetTextureResultstar(resultstar->PatternAnim);
		}

		//��]����
		resultstar->rot.z += 0.01f;

	}
	SetVertexResultstar();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResultstar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (resultstar->use == true)
	{
		// �e�N�X�`���̐ݒ�(�|���S���̕`��O�ɓǂݍ��񂾃e�N�X�`���̃Z�b�g���s��)
		// �e�N�X�`���̃Z�b�g�����Ȃ��ƑO�ɃZ�b�g���ꂽ�e�N�X�`�����\���遨�����͂�Ȃ����Ƃ��w�肷��pDevide->SetTexture(0, NULL);
		pDevice->SetTexture(0, g_pD3DTextureResultstar);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, resultstar->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexResultstar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	resultstar->vertexWk[0].vtx = D3DXVECTOR3(resultstar->pos.x, resultstar->pos.y, resultstar->pos.z);
	resultstar->vertexWk[1].vtx = D3DXVECTOR3(resultstar->pos.x + STAR_SIZE_X, resultstar->pos.y, resultstar->pos.z);
	resultstar->vertexWk[2].vtx = D3DXVECTOR3(resultstar->pos.x, resultstar->pos.y + STAR_SIZE_Y, resultstar->pos.z);
	resultstar->vertexWk[3].vtx = D3DXVECTOR3(resultstar->pos.x + STAR_SIZE_X, resultstar->pos.y + STAR_SIZE_Y, resultstar->pos.z);

	// rhw�̐ݒ�
	resultstar->vertexWk[0].rhw =
		resultstar->vertexWk[1].rhw =
		resultstar->vertexWk[2].rhw =
		resultstar->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	resultstar->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	resultstar->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	resultstar->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	resultstar->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);

	// �e�N�X�`�����W�̐ݒ�
	resultstar->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	resultstar->vertexWk[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	resultstar->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	resultstar->vertexWk[3].tex = D3DXVECTOR2(0.125f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureResultstar(int cntPattern)
{
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_STAR_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_STAR_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_STAR_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_STAR_Y;

	// �e�N�X�`�����W�̐ݒ�
	resultstar->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	resultstar->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	resultstar->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	resultstar->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexResultstar(void)
{
	// ���_���W�̐ݒ�
	resultstar->vertexWk[0].vtx.x = resultstar->pos.x - cosf(resultstar->angle + resultstar->rot.z) * resultstar->radius + STAR_POS_X;
	resultstar->vertexWk[0].vtx.y = resultstar->pos.y - sinf(resultstar->angle + resultstar->rot.z) * resultstar->radius + STAR_POS_Y;

	resultstar->vertexWk[1].vtx.x = resultstar->pos.x + cosf(resultstar->angle - resultstar->rot.z) * resultstar->radius + STAR_POS_X;
	resultstar->vertexWk[1].vtx.y = resultstar->pos.y - sinf(resultstar->angle - resultstar->rot.z) * resultstar->radius + STAR_POS_Y;

	resultstar->vertexWk[2].vtx.x = resultstar->pos.x - cosf(resultstar->angle - resultstar->rot.z) * resultstar->radius + STAR_POS_X;
	resultstar->vertexWk[2].vtx.y = resultstar->pos.y + sinf(resultstar->angle - resultstar->rot.z) * resultstar->radius + STAR_POS_Y;

	resultstar->vertexWk[3].vtx.x = resultstar->pos.x + cosf(resultstar->angle + resultstar->rot.z) * resultstar->radius + STAR_POS_X;
	resultstar->vertexWk[3].vtx.y = resultstar->pos.y + sinf(resultstar->angle + resultstar->rot.z) * resultstar->radius + STAR_POS_Y;

}