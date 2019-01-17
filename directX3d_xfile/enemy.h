//=============================================================================
//
// エネミー処理 [enemy.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "D3DXAnimation.h"
#include "CapsuleMesh.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_XFILE		"data/MODEL/Boy.x"				// 読み込むモデル
#define FIRST_ENEMY_POS	D3DXVECTOR3(-50.0f, 0.0f, 0.0f)	// 初期位置

//*****************************************************************************
// グローバル変数
//*****************************************************************************
typedef struct {
	D3DXVECTOR3			pos;			// モデルの位置
	D3DXVECTOR3			move;			// モデルの移動量
	D3DXVECTOR3			rot;			// 現在の向き
	D3DXVECTOR3			scl;			// モデルの大きさ(スケール)
	int					IdxShadow;		// 影ID
	float				SizeShadow;		// 影のサイズ
	D3DXCOLOR			ColShadow;		// 影の色
	int					HP;				// 体力
	int					HPzan;			// 残り体力
	D3DXANIMATION		*Animation;		// アニメーション
	CAPSULE				HitCapsule;		// 当たり判定用カプセル
}ENEMY;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(int type);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY *GetEnemy(void);
void EasyInputEnemy(void);		// 簡単入力
void MoveEnemy(void);			// 座標移動

#endif
