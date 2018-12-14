//=============================================================================
//
// SP�Q�[�W�p�t���[���\������ [frame.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "framespguage.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureFrameSpguage = NULL;		// �e�N�X�`���ւ̃|���S��

FRAMESPGUAGE framespguage[FRAMESPGUAGE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitFrameSpguage(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_FRAMESPGUAGE00,				// �t�@�C���̖��O
			&g_pD3DTextureFrameSpguage);				// �ǂݍ��ރ������̃|�C���^

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// �t���[���̏�����
	framespguage->use = true;
	framespguage->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	framespguage->CountAnim = 0;
	framespguage->PatternAnim = 0;

	// ���_���̍쐬
	MakeVertexFrameSpguage();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitFrameSpguage(void)
{
	if (g_pD3DTextureFrameSpguage != NULL)	//
	{	// �e�N�X�`���̊J��
		g_pD3DTextureFrameSpguage->Release();
		g_pD3DTextureFrameSpguage = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFrameSpguage(void)
{
	if (framespguage->use == true)
	{
		// �A�j���[�V����
		framespguage->CountAnim++;

		//�A�j���[�V����wait�`�F�b�N
		if ((framespguage->CountAnim % TIME_ANIMATION_FRAMESPGUAGE) == 0)
		{
			//�p�^�[���̐؂�ւ�
			framespguage->PatternAnim = (framespguage->PatternAnim + 1) % ANIM_PATTERN_NUM_FRAMESPGUAGE;

			//�e�N�X�`�����W���Z�b�g
			SetTextureFrameSpguage(framespguage->PatternAnim);
		}


	}
	SetVertexFrameSpguage();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawFrameSpguage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (framespguage->use == true)
	{
		// �e�N�X�`���̐ݒ�(�|���S���̕`��O�ɓǂݍ��񂾃e�N�X�`���̃Z�b�g���s��)
		// �e�N�X�`���̃Z�b�g�����Ȃ��ƑO�ɃZ�b�g���ꂽ�e�N�X�`�����\���遨�����͂�Ȃ����Ƃ��w�肷��pDevide->SetTexture(0, NULL);
		pDevice->SetTexture(0, g_pD3DTextureFrameSpguage);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, framespguage->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexFrameSpguage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	SetVertexFrameSpguage();

	// rhw�̐ݒ�
	framespguage->vertexWk[0].rhw =
		framespguage->vertexWk[1].rhw =
		framespguage->vertexWk[2].rhw =
		framespguage->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	framespguage->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	framespguage->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	framespguage->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	framespguage->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	framespguage->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	framespguage->vertexWk[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	framespguage->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	framespguage->vertexWk[3].tex = D3DXVECTOR2(0.125f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureFrameSpguage(int cntPattern)
{
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_FRAMESPGUAGE_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_FRAMESPGUAGE_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_FRAMESPGUAGE_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_FRAMESPGUAGE_Y;

	// �e�N�X�`�����W�̐ݒ�
	framespguage->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	framespguage->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	framespguage->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	framespguage->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexFrameSpguage(void)
{
	// ���_���W�̐ݒ�
	framespguage->vertexWk[0].vtx = D3DXVECTOR3(framespguage->pos.x, framespguage->pos.y, framespguage->pos.z);
	framespguage->vertexWk[1].vtx = D3DXVECTOR3(framespguage->pos.x + TEXTURE_FRAMESPGUAGE00_SIZE_X, framespguage->pos.y, framespguage->pos.z);
	framespguage->vertexWk[2].vtx = D3DXVECTOR3(framespguage->pos.x, framespguage->pos.y + TEXTURE_FRAMESPGUAGE00_SIZE_Y, framespguage->pos.z);
	framespguage->vertexWk[3].vtx = D3DXVECTOR3(framespguage->pos.x + TEXTURE_FRAMESPGUAGE00_SIZE_X, framespguage->pos.y + TEXTURE_FRAMESPGUAGE00_SIZE_Y, framespguage->pos.z);
}

