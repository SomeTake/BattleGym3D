//=============================================================================
//
// �Q�[������ [debugproc.cpp]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#include "main.h"
#include "game.h"
#include "player.h"
#include "meshfield.h"
#include "camera.h"
#include "enemy.h"
#include "egauge.h"
#include "eredgauge.h"
#include "escore.h"
#include "espgauge.h"
#include "frame.h"
#include "framespgauge.h"
#include "gauge.h"
#include "redgauge.h"
#include "spgauge.h"
#include "spmax.h"
#include "meshwall.h"
#include "skybox.h"
#include "shadow.h"
#include "particle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static bool HitStop;	// �q�b�g�X�g�b�v�t���O

//=============================================================================
// �Q�[�������Ŏg�p����f�[�^�܂Ƃ߂ď�����
//=============================================================================
HRESULT InitGame(void)
{

	return S_OK;
}

//=============================================================================
// �Q�[�������Ŏg�p����f�[�^�܂Ƃ߂ďI������
//=============================================================================
void UninitGame(void)
{
	
}

//=============================================================================
// �Q�[�������Ŏg�p����f�[�^�܂Ƃ߂čX�V
//=============================================================================
void UpdateGame(void)
{
	// �f�[�^�X�V
	UpdatePlayer();
	UpdateEnemy();
	UpdateGauge();
	UpdateEGauge();
	UpdateFrame();
	UpdateSpGauge();
	UpdateESpGauge();
	UpdateFrameSpgauge();
	UpdateSpmax();
	UpdateRedGauge();
	UpdateERedGauge();
	UpdateMeshField();
	UpdateMeshWall();
	UpdateCamera();
	UpdateParticle();
	UpdateSkyBox();
	UpdateShadow();

	// �q�b�g�X�g�b�v����
	static int timer = 0;

	if (HitStop == true)
	{
		timer++;
		if (timer == HITSTOP_TIME)
		{
			timer = 0;
			HitStop = false;
		}
	}
}

//=============================================================================
// �Q�[�������Ŏg�p����f�[�^�܂Ƃ߂ĕ`��
//=============================================================================
void DrawGame(void)
{
	//BG
	DrawSkyBox();
	DrawMeshField();
	DrawMeshWall();

	// �e
	DrawShadow();

	// �L�����N�^�[��
	DrawEnemy();
	DrawPlayer();

	// �G�t�F�N�g
	DrawParticle();

	//��ʏ��UI
	DrawRedGauge();
	DrawERedGauge();
	DrawGauge();
	DrawEGauge();
	DrawFrame();

	//��ʉ���UI
	DrawSpGauge();
	DrawESpGauge();
	DrawFrameSpgauge();
	DrawSpmax();

}

//=============================================================================
// �q�b�g�X�g�b�v�t���O�̃Q�b�^�[
//=============================================================================
bool GetHitStop(void)
{
	return HitStop;
}

//=============================================================================
// �q�b�g�X�g�b�v�t���O�̃Z�b�^�[
//=============================================================================
void SetHitStop(void)
{
	if (HitStop == false)
	{
		HitStop = true;
	}
}