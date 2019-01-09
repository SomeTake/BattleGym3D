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

D3DXVECTOR3 CenterPos;				// �v���C���[�ƃG�l�~�[�̊Ԃ̈ʒu

//=============================================================================
// �J�����̏���������
//=============================================================================
void InitCamera(void)
{
	PLAYER *playerWk = GetPlayer();
	ENEMY *enemyWk = GetEnemy();
	CAMERA *cameraWk = GetCamera(0);

	CenterPos = (playerWk->pos + enemyWk->pos) * 0.5f;
	cameraWk->pos = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
	cameraWk->at = CenterPos + D3DXVECTOR3(AT_X_CAM, AT_Y_CAM, AT_Z_CAM);

	cameraWk->up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	cameraWk->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �����_����J�����̈ʒu�܂ł̋����v��
	float vx, vz;
	vx = cameraWk->pos.x - cameraWk->at.x;
	vz = cameraWk->pos.z - cameraWk->at.z;
	cameraWk->distance = sqrtf(vx * vx + vz * vz);

}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
	PLAYER *playerWk = GetPlayer();
	ENEMY *enemyWk = GetEnemy();
	CAMERA *cameraWk = GetCamera(0);

	static float x = POS_X_CAM;
	static float y = POS_Y_CAM;
	static float z = POS_Z_CAM;

	// P��E�̒��S�ʒu
	CenterPos = (playerWk->pos + enemyWk->pos) * 0.5f;

	// P��E�̋����v��
	D3DXVECTOR3 unit = playerWk->pos - enemyWk->pos;	// PE�Ԃ̃x�N�g��
	float PEdistance = D3DXVec3Length(&unit);			// PE�Ԃ̋���
	D3DXVec3Normalize(&unit, &unit);					// ���K������

	// P��E�̍��W�̒��_����AP��E�̍��W�����ꂼ�ꌋ�񂾐����ɑ΂��Đ����Ɉړ������ʒu���J�����ʒu�Ƃ���
	D3DXVECTOR3 ViewFrom = D3DXVECTOR3(CenterPos.x - unit.z * PEdistance, AT_Y_CAM + POS_Y_CAM, CenterPos.z + unit.x * PEdistance);
	cameraWk->pos = ViewFrom;

	// �J�����̒����_�ƈʒu�̋����w��
	if (PEdistance <= POS_Z_CAM)
	{
		cameraWk->distance = POS_Z_CAM;
	}
	else
	{
		cameraWk->distance = PEdistance;
	}

	//x = cameraWk->distance * sinf(cameraWk->rot.y);
	//z = cameraWk->distance * cosf(cameraWk->rot.y);
	//x = unit.z * PEdistance;
	//z = unit.x * PEdistance;

	cameraWk->rot.y = atan2f(cameraWk->at.x - cameraWk->pos.x, cameraWk->at.z - cameraWk->pos.z) + D3DX_PI;	// �J�����̉�]�i��ɒ����_������������j
	//cameraWk->pos = cameraWk->at + D3DXVECTOR3(x, y, z);													// �J�����̈ʒu�i���_�j
	cameraWk->at = CenterPos + D3DXVECTOR3(AT_X_CAM, AT_Y_CAM, AT_Z_CAM);									// �J�����̒����_�����f���̒��S�_
	cameraWk->up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);															// 3D��Ԃ̏�����͂ǂ���H��Y���{��������
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

//=============================================================================
//P��E�̂̒��S�ʒu���擾����
//=============================================================================
D3DXVECTOR3 GetCenterPos(void)
{
	return CenterPos;
}