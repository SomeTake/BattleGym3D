//=============================================================================
//
// ゲーム処理 [debugproc.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
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
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static bool HitStop;	// ヒットストップフラグ

//=============================================================================
// ゲーム部分で使用するデータまとめて初期化
//=============================================================================
HRESULT InitGame(void)
{

	return S_OK;
}

//=============================================================================
// ゲーム部分で使用するデータまとめて終了処理
//=============================================================================
void UninitGame(void)
{
	
}

//=============================================================================
// ゲーム部分で使用するデータまとめて更新
//=============================================================================
void UpdateGame(void)
{
	int Phase = *GetPhase();
	static int ReplayCount = 0;

	// データ更新
	UpdatePlayer();
	UpdateEnemy();
	UpdateMeshField();
	UpdateMeshWall();
	UpdateCamera();
	UpdateParticle();
	UpdateSkyBox();
	UpdateShadow();

	// UIはリプレイでは使用しない
	if (Phase != PhaseReplay)
	{
		UpdateGauge();
		UpdateEGauge();
		UpdateFrame();
		UpdateSpGauge();
		UpdateESpGauge();
		UpdateFrameSpgauge();
		UpdateSpmax();
		UpdateRedGauge();
		UpdateERedGauge();
	}

	// ヒットストップ処理
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

	// リプレイ画面からタイトル画面へ戻る処理
	if (Phase == PhaseReplay)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_X) || IsButtonTriggered(1, BUTTON_X))
		{
			SetPhase(PhaseTitle);
			PlaySound(BGM_TITLE);
			ReplayCount = 0;
		}

		ReplayCount++;

		if (ReplayCount == REC_TIME)
		{
			SetPhase(PhaseTitle);
			PlaySound(BGM_TITLE);
			ReplayCount = 0;
		}

	}

}

//=============================================================================
// ゲーム部分で使用するデータまとめて描画
//=============================================================================
void DrawGame(void)
{
	int Phase = *GetPhase();

	//BG
	DrawSkyBox();
	DrawMeshField();
	DrawMeshWall();

	// 影
	DrawShadow();

	// キャラクター等
	DrawEnemy();
	DrawPlayer();

	// エフェクト
	DrawParticle();

	// UIはリプレイでは使用しない
	if (Phase != PhaseReplay)
	{
		//画面上のUI
		DrawRedGauge();
		DrawERedGauge();
		DrawGauge();
		DrawEGauge();
		DrawFrame();

		//画面下のUI
		DrawSpGauge();
		DrawESpGauge();
		DrawFrameSpgauge();
		DrawSpmax();
	}

}

//=============================================================================
// ヒットストップフラグのゲッター
//=============================================================================
bool GetHitStop(void)
{
	return HitStop;
}

//=============================================================================
// ヒットストップフラグのセッター
//=============================================================================
void SetHitStop(void)
{
	if (HitStop == false)
	{
		HitStop = true;
	}
}