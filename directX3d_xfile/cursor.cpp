//=============================================================================
//
// ���̓��[�h�Z���N�g��ʏ��� [cursor.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "cursor.h"
#include "input.h"
#include "modeselect.h"
#include "player.h"
#include "enemy.h"
#include "sound.h"
#include "debugproc.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CURSOR	CursorWk[CURSOR_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitCursor(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_CURSOR1P,			// �t�@�C���̖��O
			&CursorWk[0].D3DTexture);	// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_CURSOR2P,			// �t�@�C���̖��O
			&CursorWk[1].D3DTexture);	// �ǂݍ��ރ������[

	}

	for (int i = 0; i < CURSOR_MAX; i++)
	{
		CursorWk[i].pos = D3DXVECTOR3(CURSOR_POS_X_A, CURSOR_POS_Y, 0.0f);
		CursorWk[i].modeinput = false;
		// ���_���̍쐬
		MakeVertexCursor(i);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitCursor(void)
{
	for (int i = 0; i < CURSOR_MAX; i++)
	{
		if (CursorWk[i].D3DTexture != NULL)
		{// �e�N�X�`���̊J��
			CursorWk[i].D3DTexture->Release();
			CursorWk[i].D3DTexture = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCursor(void)
{
	CHARA *playerWk = GetPlayer();
	CHARA *enemyWk = GetEnemy();
	int NextPhase = GetNextPhase();

#ifdef _DEBUG
	// �f�o�b�O�p
	PrintDebugProc("1P�̃��[�h�I��%s", CursorWk[0].modeinput ? "����" : "������");
	PrintDebugProc("2P�̃��[�h�I��%s", CursorWk[1].modeinput ? "����" : "������");

#endif

	// �g���[�j���O���[�h�ł�2P�������I��
	if (NextPhase == NextTraining)
	{
		CursorWk[1].modeinput = true;
	}

	int PadCount = GetPadCount();
	// 1P�������Ȃ����Ă��Ȃ������ꍇ�A2P���͎����I��
	if (PadCount <= 1)
	{
		CursorWk[1].modeinput = true;
	}

	// ������������I���I��
	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_C))
	{
		if (CursorWk[0].modeinput == false)
		{
			PlaySound(SE_SELECT1);
			CursorWk[0].modeinput = true;
		}
	}
	else if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(1, BUTTON_C))
	{
		if (CursorWk[1].modeinput == false)
		{
			PlaySound(SE_SELECT1);
			CursorWk[1].modeinput = true;
		}
	}

	// �o�c����������I������
	if (CursorWk[0].modeinput == true || CursorWk[1].modeinput == true)
	{
		if (GetKeyboardTrigger(DIK_BACK))
		{
			PlaySound(SE_SELECT1);
			CursorWk[0].modeinput = false;
			CursorWk[1].modeinput = false;
		}
	}
	for (int i = 0; i < CURSOR_MAX; i++)
	{
		if (CursorWk[i].modeinput == true)
		{
			if (IsButtonTriggered(i, BUTTON_B))
			{
				CursorWk[i].modeinput = false;
			}
		}
	}

	// �������͂��I�������V�[���J��
	if (CursorWk[0].modeinput == true && CursorWk[1].modeinput == true)
	{
		StopSound(BGM_TITLE);
		ReInit();

		switch (NextPhase)
		{
		case NextTutorial:
			SetPhase(PhaseTutorial);
			PlaySound(BGM_TUTORIAL);
			break;
		case NextTraining:
			SetPhase(PhaseCountdown);
			break;
		case NextGame:
			SetPhase(PhaseCountdown);
			break;
		default:
			break;
		}
	}

	// ���E�i�L�[�{�[�h1P:���E�L�[ 2P:AD�L�[�j���͂œ��̓��[�h�̑I��&�J�[�\���ړ�
	// 1P��
	if (CursorWk[0].modeinput == false)
	{
		if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_RIGHT))
		{
			PlaySound(SE_SELECT0);
			CursorWk[0].pos = D3DXVECTOR3(CURSOR_POS_X_B, CURSOR_POS_Y, 0.0f);
			playerWk->CommandInput = true;
		}
		else if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_LEFT))
		{
			PlaySound(SE_SELECT0);
			CursorWk[0].pos = D3DXVECTOR3(CURSOR_POS_X_A, CURSOR_POS_Y, 0.0f);
			playerWk->CommandInput = false;
		}
	}

	// 2P��
	if (CursorWk[1].modeinput == false)
	{
		if (GetKeyboardTrigger(DIK_D) || IsButtonTriggered(1, BUTTON_RIGHT))
		{
			PlaySound(SE_SELECT0);
			CursorWk[1].pos = D3DXVECTOR3(CURSOR_POS_X_B, CURSOR_POS_Y, 0.0f);
			enemyWk->CommandInput = true;
		}
		else if (GetKeyboardTrigger(DIK_A) || IsButtonTriggered(1, BUTTON_LEFT))
		{
			PlaySound(SE_SELECT0);
			CursorWk[1].pos = D3DXVECTOR3(CURSOR_POS_X_A, CURSOR_POS_Y, 0.0f);
			enemyWk->CommandInput = false;
		}
	}

	// ���_���W�̍X�V
	for (int i = 0; i < CURSOR_MAX; i++)
	{
		SetVertexCursor(i);
	}

	float per;
	// �����x�̐ݒ�
	for (int i = 0; i < CURSOR_MAX; i++)
	{
		if (CursorWk[i].modeinput == true)
		{
			per = 1.0f;
		}
		else
		{
			per = 0.8f;
		}
			SetReflectCursor(i, per);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCursor(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int NextPhase = GetNextPhase();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �g���[�j���O���[�h�ł�2P���͑I�����Ȃ�
	if (NextPhase != NextTraining)
	{
		// 2P��
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CursorWk[1].D3DTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, CursorWk[1].vertexWk, sizeof(VERTEX_2D));
	}

	// 1P��
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CursorWk[0].D3DTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, CursorWk[0].vertexWk, sizeof(VERTEX_2D));

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexCursor(int num)
{
	// ���_���W�̐ݒ�
	SetVertexCursor(num);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	CursorWk[num].vertexWk[0].rhw =
		CursorWk[num].vertexWk[1].rhw =
		CursorWk[num].vertexWk[2].rhw =
		CursorWk[num].vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	CursorWk[num].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	CursorWk[num].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	CursorWk[num].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	CursorWk[num].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	CursorWk[num].vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	CursorWk[num].vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	CursorWk[num].vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	CursorWk[num].vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexCursor(int num)
{
	CursorWk[num].vertexWk[0].vtx = D3DXVECTOR3(CursorWk[num].pos.x, CursorWk[num].pos.y, CursorWk[num].pos.z);
	CursorWk[num].vertexWk[1].vtx = D3DXVECTOR3(CursorWk[num].pos.x + CURSOR_SIZE_X, CursorWk[num].pos.y, CursorWk[num].pos.z);
	CursorWk[num].vertexWk[2].vtx = D3DXVECTOR3(CursorWk[num].pos.x, CursorWk[num].pos.y + CURSOR_SIZE_Y, CursorWk[num].pos.z);
	CursorWk[num].vertexWk[3].vtx = D3DXVECTOR3(CursorWk[num].pos.x + CURSOR_SIZE_X, CursorWk[num].pos.y + CURSOR_SIZE_Y, CursorWk[num].pos.z);
}

//=============================================================================
// ���ˌ��̐ݒ� ����:int per = �����x�́�
//=============================================================================
void SetReflectCursor(int num, float per)
{
	int clear = (int)(255 * per);

	CursorWk[num].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	CursorWk[num].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	CursorWk[num].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	CursorWk[num].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
}

//=============================================================================
// �J�[�\���\���̂̃Q�b�^�[
//=============================================================================
CURSOR *GetCursor(int num)
{
	return &CursorWk[num];
}
