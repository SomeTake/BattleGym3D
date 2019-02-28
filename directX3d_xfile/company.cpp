//=============================================================================
//
// ��Ѓ��S��ʕ\������ [blackscreen.cpp]
// Author : HAL���� GP11B341 17 ���J���u
//
//=============================================================================
#include "main.h"
#include "company.h"
#include "sound.h"
#include "input.h"
#include "player.h"
#include "enemy.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureCompany = NULL;	// �e�N�X�`���ւ̃|�C���^

static VERTEX_2D				g_vertexWkCompany[NUM_VERTEX];		// ���_���i�[���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitCompany(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_COMPANY,			// �t�@�C���̖��O
			&g_pD3DTextureCompany);	// �ǂݍ��ރ������[
	}

	// ���_���̍쐬
	MakeVertexCompany();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitCompany(void)
{
	if (g_pD3DTextureCompany != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureCompany->Release();
		g_pD3DTextureCompany = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCompany(void)
{
	CHARA *playerWk = GetPlayer();
	CHARA *enemyWk = GetEnemy();

	playerWk->ReplayPos = FIRST_PLAYER_POS;
	enemyWk->ReplayPos = FIRST_ENEMY_POS;

	static float per = 0.0f;
	static int frame = 0;

	frame++;

	//�����x�̐ݒ�
	//���X�ɕ\��
	if (frame < CLEAR_DOWN)
	{
		per += 0.01f;
	}
	//100%�\�����ꂽ��100�t���[���ԕ\������
	else if (frame >= CLEAR_DOWN && frame < CLEAR_NONE)
	{
		per = 1.00f;
	}
	//���X�ɏ����Ă���
	else if (frame >= CLEAR_NONE && frame < CLEAR_UP)
	{
		per -= 0.01f;
	}

	if (frame >= CLEAR_UP)
	{
		per = 0.00f;
	}

	//���S�ɏ�������^�C�g����ʂ�
	if (frame == CLEAR_UP)
	{
		SetPhase(PhaseTitle);
		PlaySound(BGM_TITLE);
	}

	//�{�^���Ń^�C�g����ʂ�
	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_C) || IsButtonTriggered(0, BUTTON_M)
		|| IsButtonTriggered(1, BUTTON_C) || IsButtonTriggered(1, BUTTON_M))
	{
		SetPhase(PhaseTitle);
		PlaySound(BGM_TITLE);
	}

	SetReflectCompany(per);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCompany(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureCompany);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkCompany, sizeof(VERTEX_2D));
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexCompany(void)
{
	// ���_���W�̐ݒ�
	g_vertexWkCompany[0].vtx = D3DXVECTOR3(COMPANY_POS_X, COMPANY_POS_Y, 0.0f);
	g_vertexWkCompany[1].vtx = D3DXVECTOR3(COMPANY_POS_X + COMPANY_SIZE_X, COMPANY_POS_Y, 0.0f);
	g_vertexWkCompany[2].vtx = D3DXVECTOR3(COMPANY_POS_X, COMPANY_POS_Y + COMPANY_SIZE_Y, 0.0f);
	g_vertexWkCompany[3].vtx = D3DXVECTOR3(COMPANY_POS_X + COMPANY_SIZE_X, COMPANY_POS_Y + COMPANY_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkCompany[0].rhw =
		g_vertexWkCompany[1].rhw =
		g_vertexWkCompany[2].rhw =
		g_vertexWkCompany[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkCompany[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkCompany[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkCompany[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkCompany[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkCompany[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkCompany[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkCompany[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkCompany[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// ���ˌ��̐ݒ� ����:float per = �����x�́�
//=============================================================================
void SetReflectCompany(float per)
{
	int clear = (int)(255 * per);

	g_vertexWkCompany[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkCompany[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkCompany[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkCompany[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
}