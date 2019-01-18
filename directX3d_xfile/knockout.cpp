//=============================================================================
//
// �m�b�N�A�E�g�\������ [knockout.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "knockout.h"
#include "player.h"
#include "enemy.h"
#include "input.h"
#include "sound.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureKnockout = NULL;		// �e�N�X�`���ւ̃|���S��
static LPDIRECT3DTEXTURE9		g_pD3DTexturePushbutton = NULL;

KNOCKOUT knockout[1];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitKnockout(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_KNOCKOUT00,				// �t�@�C���̖��O
			&g_pD3DTextureKnockout);				// �ǂݍ��ރ������̃|�C���^

		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_PUSHBUTTON00,
			&g_pD3DTexturePushbutton);
	}

	///////////////////////////////////////////////////////////////////////////////////////
	// �t���[���̏�����
	knockout->use = true;
	knockout->pos = D3DXVECTOR3(KNOCKOUT_POS_X, KNOCKOUT_POS_Y, 0.0f);
	knockout->CountAnim = 0;
	knockout->PatternAnim = 0;
	knockout->pushok = false;

	// ���_���̍쐬
	MakeVertexKnockout();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitKnockout(void)
{
	if (g_pD3DTextureKnockout != NULL)	//
	{	// �e�N�X�`���̊J��
		g_pD3DTextureKnockout->Release();
		g_pD3DTextureKnockout = NULL;
	}

	if (g_pD3DTexturePushbutton != NULL)
	{
		g_pD3DTexturePushbutton->Release();
		g_pD3DTexturePushbutton = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateKnockout(void)
{
	PLAYER *player = GetPlayer();
	ENEMY *enemy = GetEnemy();

	StopSound(BGM_BATTLE, 0);

	static int sceneframe = 0;
	static int pushframe = 0;

	// �V�[���J�ڗL���ɂȂ�܂ł̎���
	if (sceneframe < POSSIBLE_FRAME)
	{
		sceneframe++;
	}
	
	if (sceneframe == POSSIBLE_FRAME)
	{
		knockout->pushok = true;
		knockout->pos = D3DXVECTOR3(PUSHBUTTON_POS_X, PUSHBUTTON_POS_Y, 0.0f);
		if (GetKeyboardTrigger(DIK_RETURN))
		{// Enter��������A�X�e�[�W��؂�ւ���
			SetPhase(PhaseResult);
			PlaySound(SE_WINNER0, 1, 0);
			sceneframe = 0;
			pushframe = 0;
		}
		// �Q�[���p�b�h�ňړ�����
		else if (IsButtonTriggered(0, BUTTON_M) || IsButtonTriggered(0, BUTTON_C) || IsButtonTriggered(1, BUTTON_M) || IsButtonTriggered(1, BUTTON_C))
		{
			SetPhase(PhaseResult);
			PlaySound(SE_WINNER0, 1, 0);
			sceneframe = 0;
			pushframe = 0;
		}
	}

	if (knockout->use == true)
	{
		if (knockout->pushok == true)
		{
			knockout->PatternAnim = 0;
			// �_�ł�����
			pushframe == 60 ? pushframe = 0 : pushframe++;
			if (pushframe >= 30)
			{
				SetReflectKnockout(0.00f);
			}
			else
			{
				SetReflectKnockout(1.00f);
			}
		}
		else
		{
			//�ǂ��炩��KO���ꂽ��
			if (player->HPzan == 0 || enemy->HPzan == 0)
			{
				knockout->PatternAnim = 0;
			}
			//�^�C���A�b�v�Ȃ�
			else
			{
				knockout->PatternAnim = 1;
			}
			SetReflectKnockout(1.00f);
		}
		//�e�N�X�`�����W���Z�b�g
		SetTextureKnockout(knockout->PatternAnim);
	}
	SetVertexKnockout();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawKnockout(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (knockout->use == true)
	{
		if (knockout->pushok == false)
		{
			// �e�N�X�`���̐ݒ�(�|���S���̕`��O�ɓǂݍ��񂾃e�N�X�`���̃Z�b�g���s��)
			// �e�N�X�`���̃Z�b�g�����Ȃ��ƑO�ɃZ�b�g���ꂽ�e�N�X�`�����\���遨�����͂�Ȃ����Ƃ��w�肷��pDevide->SetTexture(0, NULL);
			pDevice->SetTexture(0, g_pD3DTextureKnockout);
		}
		else
		{
			pDevice->SetTexture(0, g_pD3DTexturePushbutton);
		}
		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, knockout->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexKnockout(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	SetVertexKnockout();

	// rhw�̐ݒ�
	knockout->vertexWk[0].rhw =
		knockout->vertexWk[1].rhw =
		knockout->vertexWk[2].rhw =
		knockout->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	knockout->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	knockout->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	knockout->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	knockout->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	knockout->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	knockout->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	knockout->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	knockout->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureKnockout(int cntPattern)
{
	int x, y;
	float sizeX, sizeY;

	if (knockout->pushok == false)
	{
		x = cntPattern % TEXTURE_PATTERN_DIVIDE_KNOCKOUT_X;
		y = cntPattern / TEXTURE_PATTERN_DIVIDE_KNOCKOUT_X;
		sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_KNOCKOUT_X;
		sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_KNOCKOUT_Y;
	}
	else
	{
		x = cntPattern % TEXTURE_PATTERN_DIVIDE_PUSHBUTTON_X;
		y = cntPattern / TEXTURE_PATTERN_DIVIDE_PUSHBUTTON_X;
		sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_PUSHBUTTON_X;
		sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_PUSHBUTTON_Y;
	}

	// �e�N�X�`�����W�̐ݒ�
	knockout->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	knockout->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	knockout->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	knockout->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexKnockout(void)
{
	if (knockout->pushok == false)
	{
		// ���_���W�̐ݒ�
		knockout->vertexWk[0].vtx = D3DXVECTOR3(knockout->pos.x, knockout->pos.y, knockout->pos.z);
		knockout->vertexWk[1].vtx = D3DXVECTOR3(knockout->pos.x + TEXTURE_KNOCKOUT00_SIZE_X, knockout->pos.y, knockout->pos.z);
		knockout->vertexWk[2].vtx = D3DXVECTOR3(knockout->pos.x, knockout->pos.y + TEXTURE_KNOCKOUT00_SIZE_Y, knockout->pos.z);
		knockout->vertexWk[3].vtx = D3DXVECTOR3(knockout->pos.x + TEXTURE_KNOCKOUT00_SIZE_X, knockout->pos.y + TEXTURE_KNOCKOUT00_SIZE_Y, knockout->pos.z);
	}
	else
	{
		knockout->vertexWk[0].vtx = D3DXVECTOR3(knockout->pos.x, knockout->pos.y, knockout->pos.z);
		knockout->vertexWk[1].vtx = D3DXVECTOR3(knockout->pos.x + TEXTURE_PUSHBUTTON00_SIZE_X, knockout->pos.y, knockout->pos.z);
		knockout->vertexWk[2].vtx = D3DXVECTOR3(knockout->pos.x, knockout->pos.y + TEXTURE_PUSHBUTTON00_SIZE_Y, knockout->pos.z);
		knockout->vertexWk[3].vtx = D3DXVECTOR3(knockout->pos.x + TEXTURE_PUSHBUTTON00_SIZE_X, knockout->pos.y + TEXTURE_PUSHBUTTON00_SIZE_Y, knockout->pos.z);
	}
}

//=============================================================================
// ���ˌ��̐ݒ� ����:float per = �����x�́�
//=============================================================================
void SetReflectKnockout(float per)
{
	int clear = (int)(255 * per);

	// ���ˌ��̐ݒ�
	knockout->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	knockout->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	knockout->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	knockout->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
}