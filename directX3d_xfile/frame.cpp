//=============================================================================
//
// �g�\������ [frame.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "frame.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureFrame = NULL;		// �e�N�X�`���ւ̃|���S��

FRAME frame[FRAME_MAX];

//�t���[����h�炷
int fcount = 0;
int fcounttable[SWAY_MAX] = {
	0, 2, -2, 4, -4, 8, -8, 16, -16, 32, -32, 16, -16, 8, -8, 4, -4, 2, -2, 0,
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitFrame(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_FRAME00,				// �t�@�C���̖��O
			&g_pD3DTextureFrame);				// �ǂݍ��ރ������̃|�C���^

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// �t���[���̏�����
	frame->use = true;
	frame->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	frame->CountAnim = 0;
	frame->PatternAnim = 0;
	frame->sway = false;
	
	// ���_���̍쐬
	MakeVertexFrame();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitFrame(void)
{
	if (g_pD3DTextureFrame != NULL)	//
	{	// �e�N�X�`���̊J��
		g_pD3DTextureFrame->Release();
		g_pD3DTextureFrame = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFrame(void)
{
	if (frame->use == true)
	{
		// �A�j���[�V����
		frame->CountAnim++;

		//�A�j���[�V����wait�`�F�b�N
		if ((frame->CountAnim % TIME_ANIMATION_FRAME) == 0)
		{
			//�p�^�[���̐؂�ւ�
			frame->PatternAnim = (frame->PatternAnim + 1) % ANIM_PATTERN_NUM_FRAME;

			//�e�N�X�`�����W���Z�b�g
			SetTextureFrame(frame->PatternAnim);
		}

		//�h�炷�Ƃ�
		if (frame->sway == true)
		{
			fcount++;
			if (fcount == SWAY_MAX)
			{
				fcount = 0;
				frame->sway = false;
			}
		}
		else
		{
			fcount = 0;
		}
	}
	SetVertexFrame();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (frame->use == true)
	{
		// �e�N�X�`���̐ݒ�(�|���S���̕`��O�ɓǂݍ��񂾃e�N�X�`���̃Z�b�g���s��)
		// �e�N�X�`���̃Z�b�g�����Ȃ��ƑO�ɃZ�b�g���ꂽ�e�N�X�`�����\���遨�����͂�Ȃ����Ƃ��w�肷��pDevide->SetTexture(0, NULL);
		pDevice->SetTexture(0, g_pD3DTextureFrame);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, frame->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	SetVertexFrame();

	// rhw�̐ݒ�
	frame->vertexWk[0].rhw =
		frame->vertexWk[1].rhw =
		frame->vertexWk[2].rhw =
		frame->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	frame->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	frame->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	frame->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	frame->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	frame->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	frame->vertexWk[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	frame->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	frame->vertexWk[3].tex = D3DXVECTOR2(0.125f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureFrame(int cntPattern)
{
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_FRAME_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_FRAME_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_FRAME_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_FRAME_Y;

	// �e�N�X�`�����W�̐ݒ�
	frame->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	frame->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	frame->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	frame->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexFrame(void)
{
	int fno = fcount % SWAY_MAX;

	// ���_���W�̐ݒ�
	frame->vertexWk[0].vtx = D3DXVECTOR3(frame->pos.x, fcounttable[fno] + frame->pos.y, frame->pos.z);
	frame->vertexWk[1].vtx = D3DXVECTOR3(frame->pos.x + TEXTURE_FRAME00_SIZE_X, fcounttable[fno] + frame->pos.y, frame->pos.z);
	frame->vertexWk[2].vtx = D3DXVECTOR3(frame->pos.x, fcounttable[fno] + frame->pos.y + TEXTURE_FRAME00_SIZE_Y, frame->pos.z);
	frame->vertexWk[3].vtx = D3DXVECTOR3(frame->pos.x + TEXTURE_FRAME00_SIZE_X, fcounttable[fno] + frame->pos.y + TEXTURE_FRAME00_SIZE_Y, frame->pos.z);
}

//=============================================================================
// �t���[����h�炷���߂̃t���O�Ǘ�
//=============================================================================
void FrameSway(void)
{
	frame->sway = true;
}