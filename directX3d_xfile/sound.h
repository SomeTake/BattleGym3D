//=============================================================================
//
// サウンド処理 [sound.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

// サウンドファイル
typedef enum
{
	// SE（サウンドエフェクト）
	SE_DEFEND0,
	SE_HIT0,
	SE_HIT1,
	SE_SWING0,
	SE_SELECT0,
	SE_SELECT1,
	SE_WINNER0,
	SE_KO,
	SE_EFFECT0,
	SE_CUTIN0,
	SE_COUNTDOWN,
	// BGM（音楽）
	BGM_TITLE,
	BGM_TUTORIAL,
	BGM_TRAINING,
	BGM_BATTLE,

	//-----------------------------------------------------------------------------
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
