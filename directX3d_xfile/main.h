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
#include <stdio.h>

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

#define SCREEN_WIDTH	(1280)											// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)											// �E�C���h�E�̍���
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)								// �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)								// �E�C���h�E�̒��S�x���W

#define TIME_PER_FRAME	(1/60.0f)										// 1/60�b

#define	NUM_VERTEX		(4)												// ���_��
#define	NUM_POLYGON		(2)												// �|���S����

#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z		(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(1000.0f)										// �r���[���ʂ�FarZ�l

#define	VALUE_MOVE		(1.00f)											// �ړ���
#define VALUE_FRONTWALK	(1.50f)											// �O�����ړ���
#define VALUE_BACKWALK	(1.00f)											// �������ړ���
#define	VALUE_ROTATE	(D3DX_PI * 0.80f)								// ��]��
#define VALUE_ENLARGEMENT	(0.1f)										// �g��E�k����
#define	VALUE_ROTATE_MODEL	(D3DX_PI * 0.05f)							// ��]���x

#define	RATE_MOVE_MODEL		(0.20f)										// �ړ������W��
#define	RATE_ROTATE_MODEL	(0.20f)										// ��]�����W��

#define MAX_SEPARATE	(4)												// ��ʕ������̍ő�
#define TRANSPARENCY	(0.3f)											// �����x�̐ݒ�

#define POSSIBLE_FRAME						(120)						// KO��ʂȂǂ̃V�[���J�ڂ��L���ɂȂ�܂ł̎���

// �^�C�g�����
#define MODESELECT_POS_X					(450)						// �\���ʒu
#define MODESELECT_POS_Y					(434)						// �\���ʒu

// �|�[�Y���
#define PAUSESELECT_POS_X					(450)						// �\���ʒu
#define PAUSESELECT_POS_Y					(223)						// �\���ʒu

// �t�B�[���h�֌W
#define FIELD_SIZE							(50.0f)
#define WALL_BLOCK_NUM_X					(16)
#define WALL_BLOCK_NUM_Y					(2)
#define WALL_SIZE_HEIGHT					(100.0f)
#define WALL_SIZE_WIDTH						(50.0f)
#define WALL_POS							(400.0f)

// �o�g���֌W
#define MAX_DISTANCE						(250.0f)					// �v���C���[�Ԃ̍ő勗���i����ȏ�͉�������Ȃ��j
#define MIN_DISTANCE						(30.0f)						// �v���C���[�Ԃ̍ŏ������i����ȏ�͋߂Â��Ȃ��j
#define FULL_HP								(1000)						// �Q�[�W�̍ő�l
#define FULL_SPGUAGE						(1000)						// �Q�[�W�̍ő�l
#define DAMAGE_PUNCHI						(40)						// �_���[�W��
#define DAMAGE_KICK							(50)						// �_���[�W��
#define DAMAGE_HADOU						(100)						// �_���[�W��
#define DAMAGE_SHORYU						(120)						// �_���[�W��
#define DAMAGE_SPATTACK						(400)						// �_���[�W��
#define DAMAGE_THROW						(150)						// �_���[�W��

// �f�X�g���N�^
#define SAFE_DELETE(object)			{if(object){delete (object);		(object) = NULL;}}
#define SAFE_DELETE_ARRAY(object)	{if(object){delete[] (object);		(object) = NULL;}}
#define SAFE_RELEASE(object)		{if(object){(object)->Release();	(object)=NULL;}}

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
	PhaseThankyou,				// �Q�[���I���O�̈��A
	PhaseExit,					// �Q�[���I��
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);	// �f�o�C�X���擾����
MATRIX *GetMatrix(void);			// 3D��2D�ϊ��ɕK�v�ȃ}�g���N�X���܂Ƃ߂Ď擾����
int *GetPhase(void);				// ���݂̃Q�[���t�F�[�Y���擾����
void SetPhase(int phase);			// �Q�[���t�F�[�Y��ύX����
void ReInit(void);					// �Q�[�����[�v�̂��߂̍ŏ���������
float GetAspect(void);				// �A�X�y�N�g����擾����
HRESULT LoadTexture(LPCSTR SrcFile, LPDIRECT3DTEXTURE9* TexturePtr, const char* ErrorSrc);	// �e�N�X�`����ǂݍ���

#endif