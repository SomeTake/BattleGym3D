//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "D3DXAnimation.h"
#include "AnimationModel.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
PLAYER playerWk[1];

// �I�u�W�F�N�g�̍쐬
D3DXAnimation* PlayerAnimation;
AnimationModel* PlayerModel;
//UINT Idle;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(int type)
{
	PLAYER *playerWk = GetPlayer(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�I�u�W�F�N�g�̏�����
	PlayerAnimation = new D3DXAnimation(pDevice);
	PlayerModel = new AnimationModel();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	playerWk->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	playerWk->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	playerWk->D3DXMesh = NULL;
	playerWk->D3DXBuffMat = NULL;
	playerWk->NumMat = 0;

	playerWk->jump = false;
	playerWk->speed = PLAYER_JUMP_SPEED;

	if (type == 0)
	{
		//X�t�@�C���̓ǂݍ���
		PlayerAnimation->Load_xFile(PLAYER_XFILE);
		PlayerModel->InitController(PlayerAnimation);

		//�����A�j���[�V����
		//PlayerModel->ChangeAnimation(Idle);

		// �������̓��I�m��
		playerWk->D3DTexture = (LPDIRECT3DTEXTURE9 *)malloc(playerWk->NumMat * sizeof(LPDIRECT3DTEXTURE9));
		playerWk->MeshMaterial = (D3DMATERIAL9 *)malloc(playerWk->NumMat * sizeof(D3DMATERIAL9));

#if 0		
		D3DXMATERIAL *pD3DXMat;

		pD3DXMat = (D3DXMATERIAL*)playerWk->D3DXBuffMat->GetBufferPointer();

		// �e�N�X�`���̓ǂݍ���
		for (int i = 0; i < ((signed)playerWk->NumMat); i++)
		{
			playerWk->D3DTexture[i] = NULL;

			D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
				pD3DXMat[i].pTextureFilename,					// �t�@�C���̖��O
				&playerWk->D3DTexture[i]);						// �ǂݍ��ރ������[
		}
#endif
	}

	playerWk->IdxShadow = CreateShadow(playerWk->pos, SHADOW_SIZE_X, SHADOW_SIZE_Z);
	playerWk->SizeShadow = 25.0f;
	playerWk->ColShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	PLAYER *playerWk = GetPlayer(0);

	SAFE_RELEASE(*playerWk->D3DTexture) { if (*playerWk->D3DTexture) { (*playerWk->D3DTexture)->Release(); (*playerWk->D3DTexture) = NULL; } }
	SAFE_DELETE(playerWk->D3DXMesh) { if (playerWk->D3DXMesh) { delete (playerWk->D3DXMesh); (playerWk->D3DXMesh) = NULL; } }
	SAFE_DELETE(playerWk->D3DXBuffMat) { if (playerWk->D3DXBuffMat) { delete (playerWk->D3DXBuffMat); (playerWk->D3DXBuffMat) = NULL; } }

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	PLAYER *playerWk = GetPlayer(0);
	CAMERA *camera = GetCamera(0);
	float fDiffRotY;

	//���f���̍X�V
	PlayerModel->AdvanceTime(1.0f / 60);

	//���f���̈ړ�
	// D�F�E( �� )
	if (GetKeyboardPress(DIK_D))
	{
		// D + S�F�E��( ���E�� )
		if (GetKeyboardPress(DIK_S))
		{
			playerWk->move.x -= sinf(camera->rot.y + D3DX_PI * 0.75f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y + D3DX_PI * 0.75f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y + D3DX_PI * 0.75f;
		}
		// D + W�F�E��( ���E�� )
		else if (GetKeyboardPress(DIK_W))
		{
			playerWk->move.x -= sinf(camera->rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y + D3DX_PI * 0.25f;
		}
		// D�F�E( �� )
		else
		{
			playerWk->move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y + D3DX_PI * 0.50f;
		}
	}
	// A�F��( �� )
	else if (GetKeyboardPress(DIK_A))
	{
		// A + S�F����( ���E�� )
		if (GetKeyboardPress(DIK_S))
		{
			playerWk->move.x -= sinf(camera->rot.y - D3DX_PI * 0.75f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y - D3DX_PI * 0.75f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y - D3DX_PI * 0.75f;
		}
		// A + W�F����( ���E�� )
		else if (GetKeyboardPress(DIK_W))
		{
			playerWk->move.x -= sinf(camera->rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y - D3DX_PI * 0.25f;
		}
		// A�F��( �� )
		else
		{
			playerWk->move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y - D3DX_PI * 0.50f;
		}
	}
	// S�F��( �� )
	else if (GetKeyboardPress(DIK_S))
	{
		playerWk->move.x -= sinf(D3DX_PI + camera->rot.y) * VALUE_MOVE;
		playerWk->move.z -= cosf(D3DX_PI + camera->rot.y) * VALUE_MOVE;

		playerWk->rotDest.y = D3DX_PI + camera->rot.y;
	}
	// W�F��( �� )
	else if (GetKeyboardPress(DIK_W))
	{
		playerWk->move.x -= sinf(camera->rot.y) * VALUE_MOVE;
		playerWk->move.z -= cosf(camera->rot.y) * VALUE_MOVE;

		playerWk->rotDest.y = camera->rot.y;
	}


	// �ړI�̊p�x�܂ł̍���
	fDiffRotY = playerWk->rotDest.y - playerWk->rot.y;
	if (fDiffRotY > D3DX_PI)
	{
		fDiffRotY -= D3DX_PI * 2.0f;
	}
	if (fDiffRotY < -D3DX_PI)
	{
		fDiffRotY += D3DX_PI * 2.0f;
	}

	// �ړI�̊p�x�܂Ŋ�����������
	playerWk->rot.y += fDiffRotY * RATE_ROTATE_MODEL;
	if (playerWk->rot.y > D3DX_PI)
	{
		playerWk->rot.y -= D3DX_PI * 2.0f;
	}
	if (playerWk->rot.y < -D3DX_PI)
	{
		playerWk->rot.y += D3DX_PI * 2.0f;
	}

	/// �ʒu�ړ�
	playerWk->pos.x += playerWk->move.x;
	playerWk->pos.y += playerWk->move.y;
	playerWk->pos.z += playerWk->move.z;

	// �ړ��ʂɊ�����������
	playerWk->move.x += (0.0f - playerWk->move.x) * RATE_MOVE_MODEL;
	playerWk->move.y += (0.0f - playerWk->move.y) * RATE_MOVE_MODEL;
	playerWk->move.z += (0.0f - playerWk->move.z) * RATE_MOVE_MODEL;

	//���f���̊g��k��
	if (GetKeyboardPress(DIK_C))
	{
		playerWk->scl.x += VALUE_ENLARGEMENT;
		playerWk->scl.y += VALUE_ENLARGEMENT;
		playerWk->scl.z += VALUE_ENLARGEMENT;
	}
	else if (GetKeyboardPress(DIK_Z))
	{
		playerWk->scl.x -= VALUE_ENLARGEMENT;
		playerWk->scl.y -= VALUE_ENLARGEMENT;
		playerWk->scl.z -= VALUE_ENLARGEMENT;
	}

	//���f���̃W�����v
	if (GetKeyboardTrigger(DIK_SPACE) && playerWk->pos.y == 0.0f)
	{
		playerWk->jump = true;
	}
	if (playerWk->jump == true)
	{
		playerWk->pos.y += playerWk->speed;
		playerWk->speed -= 0.98f;
		if (playerWk->pos.y <= 0.0f)
		{
			playerWk->speed = PLAYER_JUMP_SPEED;
			playerWk->pos.y = 0.0f;
			playerWk->jump = false;
		}
	}

	// �e�̈ʒu�ݒ�
	SetPositionShadow(playerWk->IdxShadow, D3DXVECTOR3(playerWk->pos.x, 0.1f, playerWk->pos.z));
	SetVertexShadow(playerWk->IdxShadow, playerWk->SizeShadow, playerWk->SizeShadow);
	SetColorShadow(playerWk->IdxShadow, playerWk->ColShadow);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DMATERIAL9 matDef;
	MATRIX matrix = *GetMatrix();

	PLAYER *playerWk = GetPlayer(0);
	CAMERA *cameraWk = GetCamera(0);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&matrix.world);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&matrix.scale, playerWk->scl.x, playerWk->scl.y, playerWk->scl.z);
	D3DXMatrixMultiply(&matrix.world, &matrix.world, &matrix.scale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&matrix.scale, playerWk->rot.y, playerWk->rot.x, playerWk->rot.z);
	D3DXMatrixMultiply(&matrix.world, &matrix.world, &matrix.scale);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&matrix.translation, playerWk->pos.x, playerWk->pos.y, playerWk->pos.z);
	D3DXMatrixMultiply(&matrix.world, &matrix.world, &matrix.translation);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &matrix.world);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �G�l�~�[�̃����_�����O
	PlayerModel->SetMatrix(&matrix.world);
	PlayerModel->Render();

#if 0
	D3DXMATERIAL *pD3DXMat;

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)playerWk->D3DXBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)playerWk->NumMat; nCntMat++)
	{

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, playerWk->D3DTexture[nCntMat]);

		// �`��
		playerWk->D3DXMesh->DrawSubset(nCntMat);
	}

#endif

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
//�v���C���[�̏����擾����
//=============================================================================
PLAYER *GetPlayer(int pno)
{
	return &playerWk[pno];
}