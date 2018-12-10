//=============================================================================
//
// �J�E���g�_�E������ [countdown.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "countdown.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexCountdown(int cno);
void SetTextureCountdown(int cntPattern, int cno);
void SetVertexCountdown(int cno);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureCountdown = NULL;		// �e�N�X�`���ւ̃|���S��

int CountdownSecond;												// �o�ߕb��
int CountdownFrame;													// �J�E���g�_�E�����̃t���[���o��

COUNTDOWN countdown[COUNTDOWN_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitCountdown(int type)
{
	int i;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	COUNTDOWN *countdown = GetCountdown(0);

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_COUNTDOWN00,				// �t�@�C���̖��O
			&g_pD3DTextureCountdown);				// �ǂݍ��ރ������̃|�C���^

	}

	//�b���ƃt���[�����̃��Z�b�g
	CountdownSecond = 0;
	CountdownFrame = 0;

	for (i = 0; i < COUNTDOWN_MAX; i++, countdown++)
	{
		countdown->use = true;
		countdown->pos = D3DXVECTOR3(COUNTDOWN_POS_X, COUNTDOWN_POS_Y, 0.0f);
		countdown->CountAnim = 0;
		countdown->PatternAnim = 0;

		// ���_���̍쐬
		MakeVertexCountdown(i);

	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitCountdown(void)
{
	int i;
	COUNTDOWN *countdown = GetCountdown(0);

	for (i = 0; i < COUNTDOWN_MAX; i++, countdown++)
	{
		if (g_pD3DTextureCountdown != NULL)	//
		{	// �e�N�X�`���̊J��
			g_pD3DTextureCountdown->Release();
			g_pD3DTextureCountdown = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCountdown(void)
{
	int i;
	COUNTDOWN *countdown = GetCountdown(0);

	static bool flag = false;

	//�J�E���g�_�E����
	if (flag == false)
	{
		Play_Sound(SE_COUNTDOWN, 0, 0);
		flag = true;
	}

	//60�t���[����1�b�o�߂�����
	CountdownFrame++;
	CountdownSecond = CountdownFrame / 60;

	for (i = 0; i < COUNTDOWN_MAX; i++, countdown++)
	{
		if (countdown->use == true)
		{
			// �A�j���[�V����
			countdown->CountAnim++;

			//�A�j���[�V����wait�`�F�b�N
			if ((countdown->CountAnim % TIME_ANIMATION_COUNTDOWN) == 0)
			{
				//�p�^�[���̐؂�ւ�
				countdown->PatternAnim = CountdownSecond;

				//�e�N�X�`�����W���Z�b�g
				SetTextureCountdown(countdown->PatternAnim, i);
			}

		}
		SetVertexCountdown(i);
	}

	// �b����0�ɂȂ�����o�g���J�n
	if (CountdownSecond > FULL_COUNTDOWN - 1)
	{
		Play_Sound(BGM_BATTLE, 1, 1);
		SetPhase(PhaseGame);
		flag = false;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCountdown(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	int i;
	COUNTDOWN *countdown = GetCountdown(0);
	for (i = 0; i < COUNTDOWN_MAX; i++, countdown++)
	{
		if (countdown->use == true)
		{
			// �e�N�X�`���̐ݒ�(�|���S���̕`��O�ɓǂݍ��񂾃e�N�X�`���̃Z�b�g���s��)
			// �e�N�X�`���̃Z�b�g�����Ȃ��ƑO�ɃZ�b�g���ꂽ�e�N�X�`�����\���遨�����͂�Ȃ����Ƃ��w�肷��pDevide->SetTexture(0, NULL);
			pDevice->SetTexture(0, g_pD3DTextureCountdown);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, countdown->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexCountdown(int cno)
{
	COUNTDOWN *countdown = GetCountdown(cno);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	SetVertexCountdown(cno);

	// rhw�̐ݒ�
	countdown->vertexWk[0].rhw =
		countdown->vertexWk[1].rhw =
		countdown->vertexWk[2].rhw =
		countdown->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	countdown->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	countdown->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	countdown->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	countdown->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	countdown->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	countdown->vertexWk[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	countdown->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	countdown->vertexWk[3].tex = D3DXVECTOR2(0.125f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureCountdown(int cntPattern, int cno)
{
	COUNTDOWN *countdown = GetCountdown(cno);
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_COUNTDOWN_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_COUNTDOWN_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_COUNTDOWN_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_COUNTDOWN_Y;

	// �e�N�X�`�����W�̐ݒ�
	countdown->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	countdown->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	countdown->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	countdown->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexCountdown(int cno)
{
	COUNTDOWN *countdown = GetCountdown(cno);
	// ���_���W�̐ݒ�
	countdown->vertexWk[0].vtx = D3DXVECTOR3(countdown->pos.x + (TEXTURE_COUNTDOWN00_SIZE_X * cno), countdown->pos.y, countdown->pos.z);
	countdown->vertexWk[1].vtx = D3DXVECTOR3(countdown->pos.x + (TEXTURE_COUNTDOWN00_SIZE_X * (cno + 1)), countdown->pos.y, countdown->pos.z);
	countdown->vertexWk[2].vtx = D3DXVECTOR3(countdown->pos.x + (TEXTURE_COUNTDOWN00_SIZE_X * cno), countdown->pos.y + TEXTURE_COUNTDOWN00_SIZE_Y, countdown->pos.z);
	countdown->vertexWk[3].vtx = D3DXVECTOR3(countdown->pos.x + (TEXTURE_COUNTDOWN00_SIZE_X * (cno + 1)), countdown->pos.y + TEXTURE_COUNTDOWN00_SIZE_Y, countdown->pos.z);
}

//=============================================================================
//�J�E���g�_�E���̏����擾����
//=============================================================================
COUNTDOWN *GetCountdown(int cno)
{
	return &countdown[cno];
}
