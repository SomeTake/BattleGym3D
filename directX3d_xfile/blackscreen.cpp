//=============================================================================
//
// ����ʏ��� [blackscreen.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "blackscreen.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBlackscreen(void);
void SetReflectBlackscreen(float per);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureBlackscreen = NULL;	// �e�N�X�`���ւ̃|�C���^

static VERTEX_2D				g_vertexWkBlackscreen[NUM_VERTEX];		// ���_���i�[���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBlackscreen(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BLACKSCREEN,			// �t�@�C���̖��O
			&g_pD3DTextureBlackscreen);	// �ǂݍ��ރ������[
	}

	// ���_���̍쐬
	MakeVertexBlackscreen();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBlackscreen(void)
{
	if (g_pD3DTextureBlackscreen != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureBlackscreen->Release();
		g_pD3DTextureBlackscreen = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBlackscreen(void)
{
	int phase = *GetPhase();

	//�����x�̐ݒ�
	if (phase == PhasePause || phase == PhaseTrainingPause)
	{
		SetReflectBlackscreen(TRANSPARENCY);
	}
	if (phase == PhaseTitle || phase == PhaseCompanyLogo)
	{
		SetReflectBlackscreen(1.0f);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBlackscreen(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureBlackscreen);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkBlackscreen, sizeof(VERTEX_2D));
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexBlackscreen(void)
{
	// ���_���W�̐ݒ�
	g_vertexWkBlackscreen[0].vtx = D3DXVECTOR3(BLACKSCREEN_POS_X, BLACKSCREEN_POS_Y, 0.0f);
	g_vertexWkBlackscreen[1].vtx = D3DXVECTOR3(BLACKSCREEN_POS_X + BLACKSCREEN_SIZE_X, BLACKSCREEN_POS_Y, 0.0f);
	g_vertexWkBlackscreen[2].vtx = D3DXVECTOR3(BLACKSCREEN_POS_X, BLACKSCREEN_POS_Y + BLACKSCREEN_SIZE_Y, 0.0f);
	g_vertexWkBlackscreen[3].vtx = D3DXVECTOR3(BLACKSCREEN_POS_X + BLACKSCREEN_SIZE_X, BLACKSCREEN_POS_Y + BLACKSCREEN_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkBlackscreen[0].rhw =
		g_vertexWkBlackscreen[1].rhw =
		g_vertexWkBlackscreen[2].rhw =
		g_vertexWkBlackscreen[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkBlackscreen[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkBlackscreen[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkBlackscreen[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkBlackscreen[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkBlackscreen[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkBlackscreen[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkBlackscreen[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkBlackscreen[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// ���ˌ��̐ݒ� ����:int per = �����x�́�
//=============================================================================
void SetReflectBlackscreen(float per)
{
	int clear = (int)(255 * per);

	g_vertexWkBlackscreen[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkBlackscreen[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkBlackscreen[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkBlackscreen[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
}