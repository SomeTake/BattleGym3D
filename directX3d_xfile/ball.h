//=============================================================================
//
// 当たり判定用ボール処理 [ball.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _BALL_H_
#define _BALL_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	BALL_XFILE		"data/MODEL/Ball.x"					// 読み込むモデル名
#define	HIT_CHECK_NUM	(13)								// 当たり判定の数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 現在の位置
	D3DXVECTOR3 rot;		// 回転
	D3DXVECTOR3 scl;		// 大きさ
	LPDIRECT3DTEXTURE9	D3DTexture;		// テクスチャ読み込み場所
	LPD3DXMESH			Mesh;			// ID3DXMeshインターフェイスへのポインタ
	LPD3DXBUFFER		D3DXMatBuff;	// メッシュのマテリアル情報を格納
	DWORD				NumMat;			// 属性情報の総数
	D3DXMATRIX			mtxWorld;		// ワールドマトリックス

} BALL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBall(int type, BALL *Ball, D3DXMATRIX Matrix, float Radius);	// 初期化、BALL構造体、行列、半径
void UninitBall(BALL *Ball);
void UpdateBall(BALL *Ball, D3DXMATRIX Matrix);
void DrawBall(BALL *Ball);

#endif
