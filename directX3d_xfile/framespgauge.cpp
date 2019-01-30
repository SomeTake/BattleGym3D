//=============================================================================
//
// SP�Q�[�W�p�t���[���\������ [frame.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "framespgauge.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureFrameSpgauge = NULL;		// �e�N�X�`���ւ̃|���S��

FRAMESPGAUGE framespgauge[FRAMESPGAUGE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitFrameSpgauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_FRAMESPGAUGE00,				// �t�@�C���̖��O
			&g_pD3DTextureFrameSpgauge);				// �ǂݍ��ރ������̃|�C���^

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// �t���[���̏�����
	framespgauge->use = true;
	framespgauge->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	framespgauge->CountAnim = 0;
	framespgauge->PatternAnim = 0;

	// ���_���̍쐬
	MakeVertexFrameSpgauge();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitFrameSpgauge(void)
{
	if (g_pD3DTextureFrameSpgauge != NULL)	//
	{	// �e�N�X�`���̊J��
		g_pD3DTextureFrameSpgauge->Release();
		g_pD3DTextureFrameSpgauge = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFrameSpgauge(void)
{
	if (framespgauge->use == true)
	{
		// �A�j���[�V����
		framespgauge->CountAnim++;

		//�A�j���[�V����wait�`�F�b�N
		if ((framespgauge->CountAnim % TIME_ANIMATION_FRAMESPGAUGE) == 0)
		{
			//�p�^�[���̐؂�ւ�
			framespgauge->PatternAnim = (framespgauge->PatternAnim + 1) % ANIM_PATTERN_NUM_FRAMESPGAUGE;

			//�e�N�X�`�����W���Z�b�g
			SetTextureFrameSpgauge(framespgauge->PatternAnim);
		}


	}
	SetVertexFrameSpgauge();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawFrameSpgauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (framespgauge->use == true)
	{
		// �e�N�X�`���̐ݒ�(�|���S���̕`��O�ɓǂݍ��񂾃e�N�X�`���̃Z�b�g���s��)
		// �e�N�X�`���̃Z�b�g�����Ȃ��ƑO�ɃZ�b�g���ꂽ�e�N�X�`�����\���遨�����͂�Ȃ����Ƃ��w�肷��pDevide->SetTexture(0, NULL);
		pDevice->SetTexture(0, g_pD3DTextureFrameSpgauge);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, framespgauge->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexFrameSpgauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	SetVertexFrameSpgauge();

	// rhw�̐ݒ�
	framespgauge->vertexWk[0].rhw =
		framespgauge->vertexWk[1].rhw =
		framespgauge->vertexWk[2].rhw =
		framespgauge->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	framespgauge->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	framespgauge->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	framespgauge->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	framespgauge->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	framespgauge->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	framespgauge->vertexWk[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	framespgauge->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	framespgauge->vertexWk[3].tex = D3DXVECTOR2(0.125f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureFrameSpgauge(int cntPattern)
{
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_FRAMESPGAUGE_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_FRAMESPGAUGE_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_FRAMESPGAUGE_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_FRAMESPGAUGE_Y;

	// �e�N�X�`�����W�̐ݒ�
	framespgauge->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	framespgauge->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	framespgauge->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	framespgauge->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexFrameSpgauge(void)
{
	// ���_���W�̐ݒ�
	framespgauge->vertexWk[0].vtx = D3DXVECTOR3(framespgauge->pos.x, framespgauge->pos.y, framespgauge->pos.z);
	framespgauge->vertexWk[1].vtx = D3DXVECTOR3(framespgauge->pos.x + TEXTURE_FRAMESPGAUGE00_SIZE_X, framespgauge->pos.y, framespgauge->pos.z);
	framespgauge->vertexWk[2].vtx = D3DXVECTOR3(framespgauge->pos.x, framespgauge->pos.y + TEXTURE_FRAMESPGAUGE00_SIZE_Y, framespgauge->pos.z);
	framespgauge->vertexWk[3].vtx = D3DXVECTOR3(framespgauge->pos.x + TEXTURE_FRAMESPGAUGE00_SIZE_X, framespgauge->pos.y + TEXTURE_FRAMESPGAUGE00_SIZE_Y, framespgauge->pos.z);
}

