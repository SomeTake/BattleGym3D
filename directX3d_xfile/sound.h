//=============================================================================
//
// サウンド処理 [sound.h]
// Author : GP11B341 24　中込和輝
//
//=============================================================================
#ifndef ___SOUND_H___
#define ___SOUND_H___


//*****************************************************************************
// インクルード
//*****************************************************************************
#include <windows.h>
#include <tchar.h>
#include <dsound.h>
#include <mmsystem.h>


//*****************************************************************************
// マクロ＆列挙型定義
//*****************************************************************************
enum SOUND_NAME//サウンドテーブル --------------------------------------------
{	// SE（サウンドエフェクト）
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
	SOUND_MAX	// サウンドの数
};

// 使うこと無さそうだが念のため定義 ※ME_01とBGM_01の名前が変わったら要反映
#define SE_MAX	(ME_WIN)								// SEの数
#define ME_MAX	(BGM_TITLE - SE_MAX)					// MEの数
#define BGM_MAX	(SOUND_MAX - SE_MAX - ME_MAX)			// BGMの数

enum
{	// 再生用フラグ
	E_DS8_FLAG_NONE,
	E_DS8_FLAG_LOOP,	// DSBPLAY_LOOPING=1
	E_DS8_FLAG_MAX
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT					Init_Sound( HWND hWnd );			// サウンドの初期化処理
void					Uninit_Sound();						// サウンドの終了処理
LPDIRECTSOUNDBUFFER8	Load_Sound( int no );				// サウンドのロード
// 音ごとに再生する
void Play_Sound( int no, int type, int flag);

// 音を止める
void Stop_Sound(int no, int type);

// 再生中かどうか調べる
bool Is_Playing( LPDIRECTSOUNDBUFFER8 pBuffer );
// サウンドを取得する
LPDIRECTSOUNDBUFFER8 Get_Sound(SOUND_NAME sound_name);

void Reset_Sound(SOUND_NAME sound_name);
#endif