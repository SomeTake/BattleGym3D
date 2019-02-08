//=============================================================================
//
// ���[�h�Z���N�g���� [modeselect.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "modeselect.h"
#include "input.h"
#include "sound.h"
#include "cursor.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTextureModeselect = NULL;		// �e�N�X�`���ւ̃|���S��

MODESELECT modeselectWk[MODESELECT_MAX];				// �\����

int titleselect = 0;									// �^�C�g����ʂŃ��[�h�Z���N�g���s�����߂̕ϐ�
int pauseselect = 0;									// �|�[�Y��ʂŃ��[�h�Z���N�g���s�����߂̕ϐ�
int buttoncount = 0;									// �[���I�Ƀ{�^�����s�[�g����邽�߂̃J�E���g
int nextphase;											// �J�E���g�_�E���t�F�[�Y���I������g���[�j���O���[�h�ɂ��邩VS���[�h�ɂ��邩
bool selectok;											// �^�C�g����ʂ̃��[�h�Z���N�g���I�����Ă��邩�ǂ���

//=============================================================================
// ����������
//=============================================================================
HRESULT InitModeselect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MODESELECT *modeselect = modeselectWk;

	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_MODESELECT,				// �t�@�C���̖��O
			&g_pD3DTextureModeselect);			// �ǂݍ��ރ������̃|�C���^
	}

	titleselect = 0;
	pauseselect = 0;
	selectok = false;

	// ����������
	for (int i = 0; i < MODESELECT_MAX; i++, modeselect++)
	{
		modeselectWk->pos = D3DXVECTOR3(MODESELECT_POS_X, MODESELECT_POS_Y, 0.0f);	// ���W�f�[�^��������
		modeselectWk->PatternAnim = 0;									// �A�j���p�^�[���ԍ��������_���ŏ�����
		modeselectWk->CountAnim = 0;									// �A�j���J�E���g��������

		modeselectWk->Texture = g_pD3DTextureModeselect;				// �e�N�X�`�����
		MakeVertexModeselect(i);										// ���_���̍쐬
	}


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitModeselect(void)
{
	if (g_pD3DTextureModeselect != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTextureModeselect->Release();
		g_pD3DTextureModeselect = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateModeselect(void)
{
	int phase = *GetPhase();
	CURSOR *CursorWk = GetCursor(0);

	switch (phase)
	{
	case PhaseTitle:
		if (selectok == false)
		{
			//�J�[�\���㉺�ړ�
			if (GetKeyboardRepeat(DIK_UP) || IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, STICK_UP)
				|| IsButtonTriggered(1, BUTTON_UP) || IsButtonTriggered(1, STICK_UP))
			{
				PlaySound(SE_SELECT0, 0, 0);
				titleselect = (titleselect == 0) ? TITLESELECT_MAX - 1 : titleselect - 1;
				modeselectWk->pos.y = (float)MODESELECT_POS_Y + (titleselect * TEXTURE_MODESELECT_SIZE_Y);
			}
			else if (GetKeyboardRepeat(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN) || IsButtonTriggered(0, STICK_DOWN)
				|| IsButtonTriggered(1, BUTTON_DOWN) || IsButtonTriggered(1, STICK_DOWN))
			{
				PlaySound(SE_SELECT0, 0, 0);
				titleselect = (titleselect == TITLESELECT_MAX - 1) ? 0 : titleselect + 1;
				modeselectWk->pos.y = (float)MODESELECT_POS_Y + (titleselect * TEXTURE_MODESELECT_SIZE_Y);
			}

			//�{�^���̃��s�[�g������[���I�ɍ쐬
			if (IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, STICK_UP)
				|| IsButtonPressed(1, BUTTON_UP) || IsButtonPressed(1, STICK_UP))
			{
				if (buttoncount < BUTTON_TIMER)
				{
					buttoncount++;
				}
				if (buttoncount >= BUTTON_TIMER)
				{
					PlaySound(SE_SELECT0, 0, 0);
					titleselect = (titleselect == 0) ? TITLESELECT_MAX - 1 : titleselect - 1;
					modeselectWk->pos.y = (float)MODESELECT_POS_Y + (titleselect * TEXTURE_MODESELECT_SIZE_Y);
				}
			}
			else if (IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, STICK_DOWN)
				|| IsButtonPressed(1, BUTTON_DOWN) || IsButtonPressed(1, STICK_DOWN))
			{
				if (buttoncount < BUTTON_TIMER)
				{
					buttoncount++;
				}
				if (buttoncount >= BUTTON_TIMER)
				{
					PlaySound(SE_SELECT0, 0, 0);
					titleselect = (titleselect == TITLESELECT_MAX - 1) ? 0 : titleselect + 1;
					modeselectWk->pos.y = (float)MODESELECT_POS_Y + (titleselect * TEXTURE_MODESELECT_SIZE_Y);
				}
			}
			if (!IsButtonPressed(0, BUTTON_UP) && !IsButtonPressed(0, STICK_UP) && !IsButtonPressed(0, BUTTON_DOWN) && !IsButtonPressed(0, STICK_DOWN)
				&& !IsButtonPressed(1, BUTTON_UP) && !IsButtonPressed(1, STICK_UP) && !IsButtonPressed(1, BUTTON_DOWN) && !IsButtonPressed(1, STICK_DOWN))
			{
				buttoncount = 0;
			}

			//���[�h�Z���N�g
			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_C) || IsButtonTriggered(1, BUTTON_C))
			{
				PlaySound(SE_SELECT1, 0, 0);

				switch (titleselect)
				{
				case NextTutorial:
					nextphase = NextTutorial;
					selectok = true;
					break;
				case NextTraining:
					nextphase = NextTraining;
					selectok = true;
					break;
				case NextGame:
					nextphase = NextGame;
					selectok = true;
					break;
				case NextThankyou:
					SetPhase(PhaseThankyou);
					break;
				default:
					break;
				}
			}
		}
		else if (selectok == true && (CursorWk[0].modeinput == false && CursorWk[1].modeinput == false))
		{
			// �O��ʂɖ߂�
			if (GetKeyboardTrigger(DIK_BACK))
			{
				PlaySound(SE_SELECT1, 0, 0);
				selectok = false;
			}
		}
		else if (selectok == true && CursorWk[0].modeinput == false)
		{
			if (IsButtonTriggered(0, BUTTON_B))
			{
				PlaySound(SE_SELECT1, 0, 0);
				selectok = false;
			}
		}
		else if (selectok == true && CursorWk[1].modeinput == false)
		{
			if (IsButtonTriggered(1, BUTTON_B))
			{
				PlaySound(SE_SELECT1, 0, 0);
				selectok = false;
			}
		}
		break;
	case PhaseTrainingPause:
		//�J�[�\���㉺�ړ�
		if (GetKeyboardRepeat(DIK_UP) || IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, STICK_UP)
			|| IsButtonTriggered(1, BUTTON_UP) || IsButtonTriggered(1, STICK_UP))
		{
			PlaySound(SE_SELECT0, 0, 0);
			pauseselect = (pauseselect == 0) ? PAUSESELECT_MAX - 1 : pauseselect - 1;
			modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
		}
		else if (GetKeyboardRepeat(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN) || IsButtonTriggered(0, STICK_DOWN)
			|| IsButtonTriggered(1, BUTTON_DOWN) || IsButtonTriggered(1, STICK_DOWN))
		{
			PlaySound(SE_SELECT0, 0, 0);
			pauseselect = (pauseselect == PAUSESELECT_MAX - 1) ? 0 : pauseselect + 1;
			modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
		}

		//�{�^���̃��s�[�g������[���I�ɍ쐬
		if (IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, STICK_UP)
			|| IsButtonPressed(1, BUTTON_UP) || IsButtonPressed(1, STICK_UP))
		{
			buttoncount++;
			if (buttoncount >= BUTTON_TIMER)
			{
				PlaySound(SE_SELECT0, 0, 0);
				pauseselect = (pauseselect == 0) ? PAUSESELECT_MAX - 1 : pauseselect - 1;
				modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
			}
		}
		else if (IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, STICK_DOWN)
			|| IsButtonPressed(1, BUTTON_DOWN) || IsButtonPressed(1, STICK_DOWN))
		{
			buttoncount++;
			if (buttoncount >= BUTTON_TIMER)
			{
				PlaySound(SE_SELECT0, 0, 0);
				pauseselect = (pauseselect == PAUSESELECT_MAX - 1) ? 0 : pauseselect + 1;
				modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
			}
		}
		if (!IsButtonPressed(0, BUTTON_UP) && !IsButtonPressed(0, STICK_UP) && !IsButtonPressed(0, BUTTON_DOWN) && !IsButtonPressed(0, STICK_DOWN)
			&& !IsButtonPressed(1, BUTTON_UP) && !IsButtonPressed(1, STICK_UP) && !IsButtonPressed(1, BUTTON_DOWN) && !IsButtonPressed(1, STICK_DOWN))
		{
			buttoncount = 0;
		}

		//���[�h�Z���N�g
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_C) || IsButtonTriggered(1, BUTTON_C))
		{
			switch (pauseselect)
			{
			case NextPauseGame:
				PlaySound(SE_SELECT1, 0, 0);
				SetPhase(PhaseTraining);
				PlaySound(BGM_TRAINING, 1, 1);
				break;
			case NextPauseCountdown:
				PlaySound(SE_SELECT1, 0, 0);
				ReInit();
				SetPhase(PhaseCountdown);
				PlaySound(BGM_TRAINING, 0, 1);
				nextphase = NextTraining;
				break;
			case NextPauseTitle:
				PlaySound(SE_SELECT1, 0, 0);
				ReInit();
				SetPhase(PhaseTitle);
				PlaySound(BGM_TITLE, 0, 1);
				break;
			default:
				break;
			}
		}

		// �Q�[����ʂɖ߂�
		if (IsButtonTriggered(0, BUTTON_M) || IsButtonTriggered(1, BUTTON_M))
		{
			PlaySound(SE_SELECT1, 0, 0);
			SetPhase(PhaseTraining);
			PlaySound(BGM_TRAINING, 1, 1);
		}
		break;
	case PhasePause:
		//�J�[�\���㉺�ړ�
		if (GetKeyboardRepeat(DIK_UP) || IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, STICK_UP)
			|| IsButtonTriggered(1, BUTTON_UP) || IsButtonTriggered(1, STICK_UP))
		{
			PlaySound(SE_SELECT0, 0, 0);
			pauseselect = (pauseselect == 0) ? PAUSESELECT_MAX - 1 : pauseselect - 1;
			modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
		}
		else if (GetKeyboardRepeat(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN) || IsButtonTriggered(0, STICK_DOWN)
			|| IsButtonTriggered(1, BUTTON_DOWN) || IsButtonTriggered(1, STICK_DOWN))
		{
			PlaySound(SE_SELECT0, 0, 0);
			pauseselect = (pauseselect == PAUSESELECT_MAX - 1) ? 0 : pauseselect + 1;
			modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
		}

		//�{�^���̃��s�[�g������[���I�ɍ쐬
		if (IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, STICK_UP)
			|| IsButtonPressed(1, BUTTON_UP) || IsButtonPressed(1, STICK_UP))
		{
			buttoncount++;
			if (buttoncount >= BUTTON_TIMER)
			{
				PlaySound(SE_SELECT0, 0, 0);
				pauseselect = (pauseselect == 0) ? PAUSESELECT_MAX - 1 : pauseselect - 1;
				modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
			}
		}
		else if (IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, STICK_DOWN)
			|| IsButtonPressed(1, BUTTON_DOWN) || IsButtonPressed(1, STICK_DOWN))
		{
			buttoncount++;
			if (buttoncount >= BUTTON_TIMER)
			{
				PlaySound(SE_SELECT0, 0, 0);
				pauseselect = (pauseselect == PAUSESELECT_MAX - 1) ? 0 : pauseselect + 1;
				modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
			}
		}
		if (!IsButtonPressed(0, BUTTON_UP) && !IsButtonPressed(0, STICK_UP) && !IsButtonPressed(0, BUTTON_DOWN) && !IsButtonPressed(0, STICK_DOWN)
			&& !IsButtonPressed(1, BUTTON_UP) && !IsButtonPressed(1, STICK_UP) && !IsButtonPressed(1, BUTTON_DOWN) && !IsButtonPressed(1, STICK_DOWN))
		{
			buttoncount = 0;
		}

		//���[�h�Z���N�g
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_C) || IsButtonTriggered(0, BUTTON_C))
		{
			switch (pauseselect)
			{
			case NextPauseGame:
				PlaySound(SE_SELECT1, 0, 0);
				SetPhase(PhaseGame);
				PlaySound(BGM_BATTLE, 1, 1);
				break;
			case NextPauseCountdown:
				PlaySound(SE_SELECT1, 0, 0);
				ReInit();
				SetPhase(PhaseCountdown);
				nextphase = NextGame;
				break;
			case NextPauseTitle:
				PlaySound(SE_SELECT1, 0, 0);
				ReInit();
				SetPhase(PhaseTitle);
				PlaySound(BGM_TITLE, 1, 1);
				break;
			default:
				break;
			}
		}

		// �Q�[����ʂɖ߂�
		if (IsButtonTriggered(0, BUTTON_M) || IsButtonTriggered(1, BUTTON_M))
		{
			PlaySound(SE_SELECT1, 0, 0);
			SetPhase(PhaseGame);
			PlaySound(BGM_BATTLE, 1, 1);
		}
		break;
	default:
		break;
	}

	//�����x�̐ݒ�
	if (phase == PhasePause || phase == PhaseTrainingPause)
	{
		SetReflectModeselect(TRANSPARENCY);
	}
	if (phase == PhaseTitle)
	{
		SetReflectModeselect(1);
	}

	SetTextureModeselect(0);

	SetVertexModeselect();	// �ړ���̍��W�Œ��_��ݒ�

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawModeselect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MODESELECT *modeselect = modeselectWk;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MODESELECT_MAX; i++, modeselect++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, modeselectWk->Texture);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, modeselectWk->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexModeselect(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	SetVertexModeselect();

	// rhw�̐ݒ�
	modeselectWk->vertexWk[0].rhw =
		modeselectWk->vertexWk[1].rhw =
		modeselectWk->vertexWk[2].rhw =
		modeselectWk->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	modeselectWk->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	modeselectWk->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	modeselectWk->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	modeselectWk->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);

	// �e�N�X�`�����W�̐ݒ�
	modeselectWk->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	modeselectWk->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_MODESELECT, 0.0f);
	modeselectWk->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MODESELECT);
	modeselectWk->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_MODESELECT, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MODESELECT);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureModeselect(int cntPattern)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_MODESELECT;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_MODESELECT;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_MODESELECT;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MODESELECT;

	modeselectWk->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	modeselectWk->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	modeselectWk->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	modeselectWk->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexModeselect(void)
{
	// ���_���W�̐ݒ�
	modeselectWk->vertexWk[0].vtx = D3DXVECTOR3(modeselectWk->pos.x, modeselectWk->pos.y, modeselectWk->pos.z);
	modeselectWk->vertexWk[1].vtx = D3DXVECTOR3(modeselectWk->pos.x + TEXTURE_MODESELECT_SIZE_X, modeselectWk->pos.y, modeselectWk->pos.z);
	modeselectWk->vertexWk[2].vtx = D3DXVECTOR3(modeselectWk->pos.x, modeselectWk->pos.y + TEXTURE_MODESELECT_SIZE_Y, modeselectWk->pos.z);
	modeselectWk->vertexWk[3].vtx = D3DXVECTOR3(modeselectWk->pos.x + TEXTURE_MODESELECT_SIZE_X, modeselectWk->pos.y + TEXTURE_MODESELECT_SIZE_Y, modeselectWk->pos.z);
}

//=============================================================================
// ���ˌ��̐ݒ� ����:int per = �����x�́�
//=============================================================================
void SetReflectModeselect(float per)
{
	int clear = (int)(255 * per);

	modeselectWk->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	modeselectWk->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	modeselectWk->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	modeselectWk->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
}

//=============================================================================
// ���[�h�Z���N�g�擾�֐�
//=============================================================================
MODESELECT *GetModeselect(int no)
{
	return(&modeselectWk[no]);
}

//=============================================================================
// ���̃t�F�[�Y���擾����t���O
//=============================================================================
int GetNextPhase(void)
{
	return nextphase;
}

//=============================================================================
// �^�C�g����ʂ̃Z���N�g���I�����Ă��邩�ǂ������擾����t���O
//=============================================================================
bool GetTitleSelect(void)
{
	return selectok;
}