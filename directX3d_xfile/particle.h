//=============================================================================
//
// パーティクル処理 [particle.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_PARTICLE			(8192)	// パーティクル最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 scl;		// スケール
	D3DXCOLOR col;			// 色
	float fWidth;			// 幅
	float fHeight;			// 高さ
	int nIdxShadow;			// 影ID
	bool bUse;				// 使用しているかどうか
	int time;				// 表示する時間
	float theta;			// 角度θ
} PARTICLE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitParticle(int type);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

int SetParticle(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col);

PARTICLE *GetParticle(int no);

#endif
