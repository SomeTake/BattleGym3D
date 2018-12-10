//=============================================================================
//
// �J�������� [camera.cpp]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#include "main.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "enemy.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

CAMERA cameraWk[MAX_SEPARATE];

//=============================================================================
// �J�����̏���������
//=============================================================================
void InitCamera(void)
{
	PLAYER *playerWk = GetPlayer(0);
	ENEMY *enemyWk = GetEnemy(0);
	CAMERA *cameraWk = GetCamera(0);

	for (int i = 0; i < MAX_SEPARATE; i++, cameraWk++)
	{
		cameraWk->pos = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
		if (i == 0)
		{
			cameraWk->at = playerWk->pos + D3DXVECTOR3(0.0f, POS_Y_CAM, 0.0f);
		}
		else if (i == 1)
		{
			cameraWk->at = enemyWk->pos + D3DXVECTOR3(0.0f, POS_Y_CAM, 0.0f);
		}
		else
		{
			cameraWk->at = playerWk->pos + D3DXVECTOR3(0.0f, POS_Y_CAM, 0.0f);
		}
	
		cameraWk->up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		cameraWk->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		float vx, vz;
		vx = cameraWk->pos.x - cameraWk->at.x;
		vz = cameraWk->pos.z - cameraWk->at.z;
		cameraWk->distance = sqrtf(vx * vx + vz * vz);
	}
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
	PLAYER *playerWk = GetPlayer(0);
	ENEMY *enemyWk = GetEnemy(0);
	CAMERA *cameraWk = GetCamera(0);

	static float x = POS_X_CAM;
	static float y = POS_Y_CAM;
	static float z = POS_Z_CAM;

	for (int i = 0; i < MAX_SEPARATE; i++, cameraWk++)
	{
		// �J������Y����]
		if (GetKeyboardPress(DIK_RIGHT))
		{
			cameraWk->rot.y -= VALUE_ROTATE;
			if (cameraWk->rot.y < -D3DX_PI)
			{
				cameraWk->rot.y = D3DX_PI;
			}
			x = cameraWk->distance * sinf(cameraWk->rot.y);
			z = cameraWk->distance * cosf(cameraWk->rot.y);
		}
		else if (GetKeyboardPress(DIK_LEFT))
		{
			cameraWk->rot.y += VALUE_ROTATE;
			if (cameraWk->rot.y > D3DX_PI)
			{
				cameraWk->rot.y = -D3DX_PI;
			}
			x = cameraWk->distance * sinf(cameraWk->rot.y);
			z = cameraWk->distance * cosf(cameraWk->rot.y);
		}

		// �J�����̒����_�Ǝ��_�̋����ύX
		if (GetKeyboardPress(DIK_UP))
		{
			if (cameraWk->distance > 5.0f)
			{
				cameraWk->distance -= 5.0f;
			}
			else if (cameraWk->distance == 5.0f)
			{
				cameraWk->distance = 1.0f;
			}
			else if (cameraWk->distance < 5.0f)
			{
				cameraWk->distance = 1.0f;
			}
			x = cameraWk->distance * sinf(cameraWk->rot.y);
			z = cameraWk->distance * cosf(cameraWk->rot.y);
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{
			cameraWk->distance += 5.0f;
			x = cameraWk->distance * sinf(cameraWk->rot.y);
			z = cameraWk->distance * cosf(cameraWk->rot.y);
		}

		cameraWk->pos = cameraWk->at + D3DXVECTOR3(x, y, z);				// �J�����̈ʒu�i���_�j
		if (i == 0)
		{
			cameraWk->at = playerWk->pos + D3DXVECTOR3(0.0f, POS_Y_CAM, 0.0f);	// �J�����̒����_�����f���̒��S�_
		}
		else if(i == 1)
		{ 
			cameraWk->at = enemyWk->pos + D3DXVECTOR3(0.0f, POS_Y_CAM, 0.0f);	// �J�����̒����_�����f���̒��S�_
		}
		else
		{
			cameraWk->at = playerWk->pos + D3DXVECTOR3(0.0f, POS_Y_CAM, 0.0f);	// �J�����̒����_�����f���̒��S�_
		}
		cameraWk->up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);						// 3D��Ԃ̏�����͂ǂ���H��Y���{��������
	}
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCamera(int no)
{
	CAMERA *cameraWk = GetCamera(no);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	float asp = GetAspect();

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&cameraWk->mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&cameraWk->mtxView,
		&cameraWk->pos,		// �J�����̎��_
		&cameraWk->at,		// �J�����̒����_
		&cameraWk->up);		// �J�����̏�����x�N�g��

							// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &cameraWk->mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&cameraWk->mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&cameraWk->mtxProjection,
		VIEW_ANGLE,			// �r���[���ʂ̎���p
		asp,				// �r���[���ʂ̃A�X�y�N�g��
		VIEW_NEAR_Z,		// �r���[���ʂ�NearZ�l
		VIEW_FAR_Z);		// �r���[���ʂ�FarZ�l

							// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &cameraWk->mtxProjection);

}

//=============================================================================
//�J�����̏����擾����
//=============================================================================
CAMERA *GetCamera(int cno)
{
	return &cameraWk[cno];
}
