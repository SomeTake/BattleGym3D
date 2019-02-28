//=============================================================================
//
// ���v���C��ʏ��� [replay.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "replay.h"
#include "input.h"
#include "sound.h"
#include "player.h"
#include "enemy.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureReplay = NULL;		// �e�N�X�`���ւ̃|�C���^

VERTEX_2D				g_vertexWkReplay[NUM_VERTEX];		// ���_���i�[���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitReplay(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_REPLAY,				// �t�@�C���̖��O
			&g_pD3DTextureReplay);		// �ǂݍ��ރ������[

	}

	// ���_���̍쐬
	MakeVertexReplay();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitReplay(void)
{
	if (g_pD3DTextureReplay != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureReplay->Release();
		g_pD3DTextureReplay = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateReplay(void)
{
	int Phase = *GetPhase();
	static int ReplayCount = 0;
	CHARA *playerWk = GetPlayer();
	CHARA *enemyWk = GetEnemy();

	// ���v���C��ʂ���^�C�g����ʂ֖߂鏈��
	if (Phase == PhaseReplay)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			SetPhase(PhaseTitle);
			PlaySound(BGM_TITLE);
			ReplayCount = 0;
		}

		for (int i = 0; i < GAMEPADMAX; i++)
		{
			// PS4�R�����{�^���������ꂽ��^�C�g����ʂ֖߂�
			if (IsButtonLockTriggered(i, BUTTON_C))
			{
				SetPhase(PhaseTitle);
				PlaySound(BGM_TITLE);
				ReplayCount = 0;
			}
		}

		ReplayCount++;

		if (ReplayCount == REC_TIME)
		{
			SetPhase(PhaseTitle);
			PlaySound(BGM_TITLE);
			ReplayCount = 0;
		}

		// �����x�̐ݒ�
		static int i = 0, reflect = 0;
		i++;
		if (i == FLASHING_TIME)
		{
			reflect == 0 ? reflect = 1 : reflect = 0;
			i = 0;
		}
		SetReflectReplay((float)reflect);

	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawReplay(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureReplay);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkReplay, sizeof(VERTEX_2D));
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexReplay(void)
{

	// ���_���W�̐ݒ�
	g_vertexWkReplay[0].vtx = D3DXVECTOR3(REPLAY_POS_X, REPLAY_POS_Y, 0.0f);
	g_vertexWkReplay[1].vtx = D3DXVECTOR3(REPLAY_POS_X + REPLAY_SIZE_X, REPLAY_POS_Y, 0.0f);
	g_vertexWkReplay[2].vtx = D3DXVECTOR3(REPLAY_POS_X, REPLAY_POS_Y + REPLAY_SIZE_Y, 0.0f);
	g_vertexWkReplay[3].vtx = D3DXVECTOR3(REPLAY_POS_X + REPLAY_SIZE_X, REPLAY_POS_Y + REPLAY_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkReplay[0].rhw =
		g_vertexWkReplay[1].rhw =
		g_vertexWkReplay[2].rhw =
		g_vertexWkReplay[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkReplay[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkReplay[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkReplay[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkReplay[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkReplay[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkReplay[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkReplay[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkReplay[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// ���ˌ��̐ݒ� ����:int per = �����x�́�
//=============================================================================
void SetReflectReplay(float per)
{
	int clear = (int)(255 * per);

	g_vertexWkReplay[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkReplay[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkReplay[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkReplay[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
}
