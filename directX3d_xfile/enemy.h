//=============================================================================
//
// エネミー処理 [enemy.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "D3DXAnimation.h"
#include "battle.h"
#include "hadou.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FIRST_ENEMY_POS	D3DXVECTOR3(-50.0f, 0.0f, 0.0f)	// 初期位置

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(int type);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
CHARA *GetEnemy(void);
void MoveEnemy(void);			// 座標移動

#endif
