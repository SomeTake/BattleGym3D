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
#include "debugproc.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
#define VIEW_DIST			(4)		// �Œ���̃L�����N�^�[�ƃJ�����̋���
#define VIEW_DIST_RATE		(0.3f)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CAMERA cameraWk[MAX_SEPARATE];

D3DXVECTOR3 CenterPos;				// �v���C���[�ƃG�l�~�[�̊Ԃ̈ʒu
float PEdistance;					// �v���C���[�ƃG�l�~�[�̋���
D3DXVECTOR3 NewCenterPos;			// �X�V��̐V�����v���C���[�ƃG�l�~�[�̊Ԃ̈ʒu

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

	// P��E�̒��S�ʒu
	CenterPos = (playerWk->pos + enemyWk->pos) * 0.5f;

	// P��E�̋����v��
	D3DXVECTOR3 unit = playerWk->pos - enemyWk->pos;	// PE�Ԃ̃x�N�g��
	PEdistance = D3DXVec3Length(&unit);					// PE�Ԃ̋���
	D3DXVec3Normalize(&unit, &unit);					// ���K������

	// �J�����̒����_�ƈʒu�̋����w��
	if (PEdistance <= POS_Z_CAM)
	{
		cameraWk->distance = POS_Z_CAM;
	}
	else
	{
		cameraWk->distance = PEdistance;
	}

	// P��E�̍��W�̒��_����AP��E�̍��W�����ꂼ�ꌋ�񂾐����ɑ΂��Đ����Ɉړ������ʒu���J�����ʒu�Ƃ���
	NewCenterPos = D3DXVECTOR3(CenterPos.x - unit.z * cameraWk->distance, AT_Y_CAM + POS_Y_CAM, CenterPos.z + unit.x * cameraWk->distance);
	D3DXVECTOR3 DistVec = NewCenterPos - cameraWk->pos;	// �X�V�O�̃J�����ʒu�ƁA�V�����J�����ʒu�̍����x�N�g���ɂ���
	// �L�����Ԃ̒��S�ʒu�͏�Ɏ擾��������悤�ɂ�����
	cameraWk->pos += DistVec * CAMERA_SPD;				// ���X�ɐV�����J�����ʒu�ɋ߂Â���

	cameraWk->rot.y = atan2f(cameraWk->at.x - cameraWk->pos.x, cameraWk->at.z - cameraWk->pos.z) * D3DX_PI;	// �J�����̉�]�i��ɒ����_������������j
	cameraWk->at = CenterPos + D3DXVECTOR3(AT_X_CAM, AT_Y_CAM, AT_Z_CAM);									// �J�����̒����_�����f���̒��S�_
	cameraWk->up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);															// 3D��Ԃ̏�����͂ǂ���H��Y���{��������

#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("�J�������W X:%f Y:%f Z:%f\n", cameraWk->pos.x, cameraWk->pos.y, cameraWk->pos.z);
	PrintDebugProc("�J�����p�x X:%f Y:%f Z:%f\n", cameraWk->rot.x, cameraWk->rot.y, cameraWk->rot.z);
	PrintDebugProc("�v���C���[�Ԃ̋��� %f\n", PEdistance);
#endif
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
//P��E�̊Ԃ̈ʒu���擾����
//=============================================================================
D3DXVECTOR3 GetCenterPos(void)
{
	return CenterPos;
}

//=============================================================================
//P��E�̋������擾����
//=============================================================================
float GetPEdistance(void)
{
	return PEdistance;
}