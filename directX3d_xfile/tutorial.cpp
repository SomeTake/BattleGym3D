//=============================================================================
//
// �`���[�g���A������ [evaluation.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "tutorial.h"
#include "input.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTextureTutorial = NULL;		// �e�N�X�`���ւ̃|���S��

TUTORIAL tutorialWk[TUTORIAL_MAX];						// �\����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTutorial(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_TUTORIAL,				// �t�@�C���̖��O
			&g_pD3DTextureTutorial);			// �ǂݍ��ރ������̃|�C���^
	}

	// ����������
	tutorialWk->pos = D3DXVECTOR3(TUTORIAL_POS_X, TUTORIAL_POS_Y, 0.0f);	// ���W�f�[�^��������
	tutorialWk->PatternAnim = 0;									// �A�j���p�^�[���ԍ��������_���ŏ�����
	tutorialWk->CountAnim = 0;									// �A�j���J�E���g��������

	tutorialWk->Texture = g_pD3DTextureTutorial;				// �e�N�X�`�����
	MakeVertexTutorial();										// ���_���̍쐬


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTutorial(void)
{
	if (g_pD3DTextureTutorial != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTextureTutorial->Release();
		g_pD3DTextureTutorial = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTutorial(void)
{
	if (GetKeyboardPress(DIK_SPACE) || IsButtonPressed(0, BUTTON_Z) || IsButtonPressed(1, BUTTON_Z))
	{
		SetTextureTutorial(1);
	}
	else
	{
		SetTextureTutorial(0);
	}
	SetVertexTutorial();	// �ړ���̍��W�Œ��_��ݒ�

	// �^�C�g����ʂ֖߂�
	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_M) || IsButtonTriggered(1, BUTTON_M))
	{
		SetPhase(PhaseTitle);
		StopSound(BGM_TUTORIAL, 0);
		PlaySound(BGM_TITLE, 1, 1);
		ReInit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, tutorialWk->Texture);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, tutorialWk->vertexWk, sizeof(VERTEX_2D));
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	SetVertexTutorial();

	// rhw�̐ݒ�
	tutorialWk->vertexWk[0].rhw =
		tutorialWk->vertexWk[1].rhw =
		tutorialWk->vertexWk[2].rhw =
		tutorialWk->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	tutorialWk->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	tutorialWk->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	tutorialWk->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	tutorialWk->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);

	// �e�N�X�`�����W�̐ݒ�
	tutorialWk->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	tutorialWk->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_TUTORIAL, 0.0f);
	tutorialWk->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_TUTORIAL);
	tutorialWk->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_TUTORIAL, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_TUTORIAL);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureTutorial(int cntPattern)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_TUTORIAL;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_TUTORIAL;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_TUTORIAL;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_TUTORIAL;

	tutorialWk->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	tutorialWk->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	tutorialWk->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	tutorialWk->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexTutorial(void)
{
	// ���_���W�̐ݒ�
	tutorialWk->vertexWk[0].vtx = D3DXVECTOR3(tutorialWk->pos.x, tutorialWk->pos.y, tutorialWk->pos.z);
	tutorialWk->vertexWk[1].vtx = D3DXVECTOR3(tutorialWk->pos.x + TEXTURE_TUTORIAL_SIZE_X, tutorialWk->pos.y, tutorialWk->pos.z);
	tutorialWk->vertexWk[2].vtx = D3DXVECTOR3(tutorialWk->pos.x, tutorialWk->pos.y + TEXTURE_TUTORIAL_SIZE_Y, tutorialWk->pos.z);
	tutorialWk->vertexWk[3].vtx = D3DXVECTOR3(tutorialWk->pos.x + TEXTURE_TUTORIAL_SIZE_X, tutorialWk->pos.y + TEXTURE_TUTORIAL_SIZE_Y, tutorialWk->pos.z);
}
