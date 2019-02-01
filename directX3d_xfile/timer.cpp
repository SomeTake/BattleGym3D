//=============================================================================
//
// �^�C�}�[���� [timer.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "timer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureTimer = NULL;		// �e�N�X�`���ւ̃|���S��

int second;														// �o�ߕb��
int TimerFrame;													// �o�g�����̃t���[���o��

TIMER timer[TIMER_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTimer(int type)
{
	int i;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TIMER *timer = GetTimer(0);

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_TIMER00,				// �t�@�C���̖��O
			&g_pD3DTextureTimer);				// �ǂݍ��ރ������̃|�C���^

	}

	//�b���ƃt���[�����̃��Z�b�g
	second = 0;
	TimerFrame = 0;

	for (i = 0; i < TIMER_MAX; i++, timer++)
	{
		timer->use = true;
		timer->pos = D3DXVECTOR3(TIMER_POS_X, TIMER_POS_Y, 0.0f);
		timer->CountAnim = 0;
		timer->PatternAnim = 0;

		// ���_���̍쐬
		MakeVertexTimer(i);

	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTimer(void)
{
	int i;
	TIMER *timer = GetTimer(0);

	for (i = 0; i < TIMER_MAX; i++, timer++)
	{
		if (g_pD3DTextureTimer != NULL)	//
		{	// �e�N�X�`���̊J��
			g_pD3DTextureTimer->Release();
			g_pD3DTextureTimer = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTimer(void)
{
	int i;
	TIMER *timer = GetTimer(0);

	int phase = *GetPhase();

	//60�t���[����1�b�o�߂�����
	if (phase == PhaseGame || phase == PhaseTraining)
	{
		TimerFrame++;
	}
	second = TimerFrame / 60;

	int kariTimer[TIMER_MAX];
	int num = FULL_TIMER - second;
	
	// �b����0�ɂȂ�����o�g���I��
	if (num < 0)
	{
		num = 0;
		SetPhase(PhaseFinish);
	}

	//�^�C�����ꌅ���o����
	for (i = 0; i < TIMER_MAX; i++)
	{
		kariTimer[i] = num % 10;
		num = num / 10;
	}

	for (i = 0; i < TIMER_MAX; i++, timer++)
	{
		if (timer->use == true)
		{
			// �A�j���[�V����
			timer->CountAnim++;

			//�A�j���[�V����wait�`�F�b�N
			if ((timer->CountAnim % TIME_ANIMATION_TIMER) == 0)
			{
				//�p�^�[���̐؂�ւ�
				timer->PatternAnim = kariTimer[TIMER_MAX - i - 1];

				//�e�N�X�`�����W���Z�b�g
				SetTextureTimer(timer->PatternAnim, i);
			}


		}
		SetVertexTimer(i);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	int i;
	TIMER *timer = GetTimer(0);
	for (i = 0; i < TIMER_MAX; i++, timer++)
	{
		if (timer->use == true)
		{
			// �e�N�X�`���̐ݒ�(�|���S���̕`��O�ɓǂݍ��񂾃e�N�X�`���̃Z�b�g���s��)
			// �e�N�X�`���̃Z�b�g�����Ȃ��ƑO�ɃZ�b�g���ꂽ�e�N�X�`�����\���遨�����͂�Ȃ����Ƃ��w�肷��pDevide->SetTexture(0, NULL);
			pDevice->SetTexture(0, g_pD3DTextureTimer);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, timer->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexTimer(int tno)
{
	TIMER *timer = GetTimer(tno);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	SetVertexTimer(tno);

	// rhw�̐ݒ�
	timer->vertexWk[0].rhw =
		timer->vertexWk[1].rhw =
		timer->vertexWk[2].rhw =
		timer->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	timer->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	timer->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	timer->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	timer->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	timer->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	timer->vertexWk[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	timer->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	timer->vertexWk[3].tex = D3DXVECTOR2(0.125f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureTimer(int cntPattern, int tno)
{
	TIMER *timer = GetTimer(tno);
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_TIMER_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_TIMER_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_TIMER_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_TIMER_Y;

	// �e�N�X�`�����W�̐ݒ�
	timer->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	timer->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	timer->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	timer->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexTimer(int tno)
{
	TIMER *timer = GetTimer(tno);
	// ���_���W�̐ݒ�
	timer->vertexWk[0].vtx = D3DXVECTOR3(timer->pos.x + (TEXTURE_TIMER00_SIZE_X * tno), timer->pos.y, timer->pos.z);
	timer->vertexWk[1].vtx = D3DXVECTOR3(timer->pos.x + (TEXTURE_TIMER00_SIZE_X * (tno + 1)), timer->pos.y, timer->pos.z);
	timer->vertexWk[2].vtx = D3DXVECTOR3(timer->pos.x + (TEXTURE_TIMER00_SIZE_X * tno), timer->pos.y + TEXTURE_TIMER00_SIZE_Y, timer->pos.z);
	timer->vertexWk[3].vtx = D3DXVECTOR3(timer->pos.x + (TEXTURE_TIMER00_SIZE_X * (tno + 1)), timer->pos.y + TEXTURE_TIMER00_SIZE_Y, timer->pos.z);
}

//=============================================================================
//�^�C�}�[�̏����擾����
//=============================================================================
TIMER *GetTimer(int tno)
{
	return &timer[tno];
}
