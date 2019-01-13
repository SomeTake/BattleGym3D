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
#define	TEXTURE_PARTICLE		"data/TEXTURE/effect000.jpg"		// 読み込むテクスチャファイル名
#define	PARTICLE_WIDTH			(10.0f)								// 木の幅
#define	PARTICLE_HEIGHT			(10.0f)								// 木の高さ
#define	VALUE_MOVE_PARTICLE		(3.00f)								// 移動速度
#define RAND_RANGE				(500)								// ランダムの範囲
#define CIRCLE_RADIUS			(100.0f)							// 円の半径
#define	MAX_PARTICLE			(8192)								// パーティクル最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 回転
	D3DXVECTOR3 scl;		// スケール
	D3DXCOLOR col;			// 色
	D3DXVECTOR3 move;		// 移動量
	float fWidth;			// 幅
	float fHeight;			// 高さ
	int nIdxShadow;			// 影ID
	bool bUse;				// 使用しているかどうか
	float DecAlpha;			// 減衰値
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
HRESULT MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice);
void SetVertexParticle(int nIdxParticle, float fWidth, float fHeight);
void SetColorParticle(int nIdxParticle, D3DXCOLOR col);

int SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fWidth, float fHeight, int Time);	// パーティクル発生用(場所, 移動量, 色, 幅, 高さ, 時間)
PARTICLE *GetParticle(int no);																				// パーティクル構造体のゲッター

#endif
