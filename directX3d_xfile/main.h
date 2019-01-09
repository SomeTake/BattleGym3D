//=============================================================================
//
// メイン処理 [main.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// インクルードファイル
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

#define DIRECTINPUT_VERSION (0x0800)	// 警告対策
#include "dinput.h"

//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#if 1	// [ここを"0"にした場合、"構成プロパティ" -> "リンカ" -> "入力" -> "追加の依存ファイル"に対象ライブラリを設定する]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// ３Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線 / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define MY_VERTEX_FVF  (D3DFVF_XYZ | D3DFVF_DIFFUSE)

#define SCREEN_WIDTH	(1280)											// ウインドウの幅
#define SCREEN_HEIGHT	(720)											// ウインドウの高さ
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)								// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)								// ウインドウの中心Ｙ座標

#define TIME_PER_FRAME	(1/60.0f)										// 1/60秒

#define	NUM_VERTEX		(4)												// 頂点数
#define	NUM_POLYGON		(2)												// ポリゴン数

#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z		(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z		(1000.0f)										// ビュー平面のFarZ値

#define	VALUE_MOVE		(1.00f)											// 移動量
#define VALUE_FRONTWALK	(0.50f)											// 前歩き移動量
#define VALUE_BACKWALK	(0.20f)											// 後ろ歩き移動量
#define	VALUE_ROTATE	(D3DX_PI * 1.00f)								// 回転量
#define VALUE_ENLARGEMENT	(0.1f)										// 拡大・縮小量
#define	VALUE_ROTATE_MODEL	(D3DX_PI * 0.05f)							// 回転速度

#define	RATE_MOVE_MODEL		(0.20f)										// 移動慣性係数
#define	RATE_ROTATE_MODEL	(0.20f)										// 回転慣性係数

#define MAX_SEPARATE	(4)												// 画面分割数の最大
#define TRANSPARENCY	(0.3f)											// 透明度の設定

// タイトル画面
#define MODESELECT_POS_X					(450)						// 表示位置
#define MODESELECT_POS_Y					(434)						// 表示位置

// ポーズ画面
#define PAUSESELECT_POS_X					(450)						// 表示位置
#define PAUSESELECT_POS_Y					(223)						// 表示位置

// ステータス関係
#define FULL_HP								(1000)						// ゲージの最大値
#define FULL_SPGUAGE						(1000)						// ゲージの最大値
#define JUMP_SPEED							(30.0f)						// ジャンプの初速
#define DAMAGE_PUNCHI						(40)						// ダメージ量
#define DAMAGE_KICK							(50)						// ダメージ量
#define DAMAGE_HADOU						(100)						// ダメージ量
#define DAMAGE_SHORYU						(120)						// ダメージ量
#define DAMAGE_SPATTACK						(400)						// ダメージ量
#define DAMAGE_THROW						(150)						// ダメージ量

// デストラクタ
#define SAFE_DELETE(object)			{if(object){delete (object);		(object) = NULL;}}
#define SAFE_DELETE_ARRAY(object)	{if(object){delete[] (object);		(object) = NULL;}}
#define SAFE_RELEASE(object)		{if(object){(object)->Release();	(object)=NULL;}}

#define GetMonitorRect(rc) SystemParametersInfo(SPI_GETWORKAREA, 0, rc, 0)	// モニター矩形

// 上記２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	float rhw;				// テクスチャのパースペクティブコレクト用
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_2D;

// 上記３Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	D3DXVECTOR3 nor;		// 法線ベクトル
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_3D;

typedef struct {
	D3DXMATRIX scale;		// スケールマトリクス
	D3DXMATRIX rotation;	// ローテーションマトリクス
	D3DXMATRIX translation;	// トランスレーションマトリクス
	D3DXMATRIX view;		// ビューマトリクス
	D3DXMATRIX projection;	// プロジェクションマトリクス
	D3DXMATRIX world;		// ワールドマトリクス
}MATRIX;

//シーン遷移
enum
{
	PhaseCompanyLogo,			// 会社ロゴ画面
	PhaseTitle,					// タイトル画面
	PhaseTutorial,				// チュートリアル
	PhaseTraining,				// トレーニングモード
	PhasePause,					// ポーズ画面
	PhaseTrainingPause,			// トレーニングモードからポーズ画面に遷移した場合
	PhaseCountdown,				// カウントダウン画面
	PhaseGame,					// ゲーム画面
	PhaseFinish,				// KO画面
	PhaseResult,				// リザルト画面
	PhaseExit,					// ゲーム終了
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);	// デバイスを取得する
MATRIX *GetMatrix(void);			// 3D→2D変換に必要なマトリクスをまとめて取得する
int *GetPhase(void);				// 現在のゲームフェーズを取得する
void SetPhase(int phase);			// ゲームフェーズを変更する
void ReInit(void);					// ゲームループのための最初期化処理
float GetAspect(void);				// アスペクト比を取得する

#endif