//=============================================================================
//
// ���C������ [main.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include <windows.h>
#include "d3dx9.h"
#include <time.h>
#include <dshow.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <tchar.h>

#define DIRECTINPUT_VERSION (0x0800)	// �x���΍�
#include "dinput.h"

//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#if 1	// [������"0"�ɂ����ꍇ�A"�\���v���p�e�B" -> "�����J" -> "����" -> "�ǉ��̈ˑ��t�@�C��"�ɑΏۃ��C�u������ݒ肷��]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")
#endif

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// �R�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / �@�� / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define MY_VERTEX_FVF  (D3DFVF_XYZ | D3DFVF_DIFFUSE)

#define SCREEN_WIDTH	(1280)				// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)				// �E�C���h�E�̍���
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// �E�C���h�E�̒��S�x���W

#define	NUM_VERTEX		(4)		// ���_��
#define	NUM_POLYGON		(2)		// �|���S����

#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z		(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(1000.0f)										// �r���[���ʂ�FarZ�l

#define	VALUE_MOVE		(2.00f)											// �ړ���
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)								// ��]��
#define VALUE_ENLARGEMENT	(0.1f)										// �g��E�k����
#define	VALUE_ROTATE_MODEL	(D3DX_PI * 0.05f)							// ��]���x

#define	RATE_MOVE_MODEL		(0.20f)										// �ړ������W��
#define	RATE_ROTATE_MODEL	(0.20f)										// ��]�����W��

#define MAX_SEPARATE	(4)												// ��ʕ������̍ő�
#define TRANSPARENCY	(0.3f)											// �����x�̐ݒ�

//�^�C�g�����
#define MODESELECT_POS_X					(450)						// �\���ʒu
#define MODESELECT_POS_Y					(434)						// �\���ʒu

//�|�[�Y���
#define PAUSESELECT_POS_X					(450)						// �\���ʒu
#define PAUSESELECT_POS_Y					(223)						// �\���ʒu

// �f�X�g���N�^
#define SAFE_DELETE(object)	{if(object){delete (object); (object) = NULL;}}
#define SAFE_DELETE_ARRAY(object)	{if(object){delete[] (object); (object) = NULL;}}
#define SAFE_RELEASE(ptr) {if(ptr){(ptr)->Release();(ptr)=NULL;}}

#define GetMonitorRect(rc) SystemParametersInfo(SPI_GETWORKAREA, 0, rc, 0)	// ���j�^�[��`

// ��L�Q�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;		// ���_���W
	float rhw;				// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR diffuse;		// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
} VERTEX_2D;

// ��L�R�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;		// ���_���W
	D3DXVECTOR3 nor;		// �@���x�N�g��
	D3DCOLOR diffuse;		// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
} VERTEX_3D;

typedef struct {
	D3DXMATRIX scale;		// �X�P�[���}�g���N�X
	D3DXMATRIX rotation;	// ���[�e�[�V�����}�g���N�X
	D3DXMATRIX translation;	// �g�����X���[�V�����}�g���N�X
	D3DXMATRIX view;		// �r���[�}�g���N�X
	D3DXMATRIX projection;	// �v���W�F�N�V�����}�g���N�X
	D3DXMATRIX world;		// ���[���h�}�g���N�X
}MATRIX;

//�V�[���J��
enum
{
	PhaseCompanyLogo,			// ��Ѓ��S���
	PhaseTitle,					// �^�C�g�����
	PhaseTutorial,				// �`���[�g���A��
	PhaseTraining,				// �g���[�j���O���[�h
	PhasePause,					// �|�[�Y���
	PhaseTrainingPause,			// �g���[�j���O���[�h����|�[�Y��ʂɑJ�ڂ����ꍇ
	PhaseCountdown,				// �J�E���g�_�E�����
	PhaseGame,					// �Q�[�����
	PhaseFinish,				// KO���
	PhaseResult,				// ���U���g���
	PhaseExit,					// �Q�[���I��
};

// �X���b�h�����[�h���Ă��邩�ǂ���
enum SWITCH_TYPE {
	SWITCH_TYPE_CREATE_THREAD,
	SWITCH_TYPE_LOADING,
	SWITCH_TYPE_MAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
MATRIX *GetMatrix(void);
int *GetPhase(void);
void SetPhase(int phase);
void ReInit(void);
float GetAspect(void);
CRITICAL_SECTION *GetCriticalSection(void);
bool *GetSwitch(int no);
float *GetAssembly(void);

#endif