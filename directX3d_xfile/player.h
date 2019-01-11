//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "D3DXAnimation.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	PLAYER_XFILE		"data/MODEL/Boy.x"			// 読み込むモデル名(ファイルパス名)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// プレイヤーのデータを管理する構造体
typedef struct {	
	D3DXVECTOR3			pos;				// モデルの位置
	D3DXVECTOR3			move;				// モデルの移動量
	D3DXVECTOR3			rot;				// 現在の向き
	D3DXVECTOR3			rotDest;			// 目的の向き
	D3DXVECTOR3			scl;				// モデルの大きさ(スケール)
	int					IdxShadow;			// 影ID
	float				SizeShadow;			// 影のサイズ
	D3DXCOLOR			ColShadow;			// 影の色
	bool				jump;				// ジャンプフラグ
	float				speed;				// ジャンプのスピード
	int					HP;					// 体力
	int					HPzan;				// 残り体力

	D3DXANIMATION		*Animation;			// アニメーション
	int					Action;				// 現在のアニメーション
	int					NextAction;			// 次のアニメーション
}PLAYER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(int type);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);
void EasyInputPlayer(void);		// 簡単入力
void MovePlayer(void);			// 座標移動

#endif
