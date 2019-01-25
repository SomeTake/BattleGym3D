//=============================================================================
//
// 波動拳用バレット処理 [hadou.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _HADOU_H_
#define _HADOU_H_

#include "main.h"
#include "battle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	HADOU_XFILE		"data/MODEL/Ball.x"					// 読み込むモデル名
#define HADOU_SIZE		(8.0f)								// 波動拳のサイズ
#define HADOU_SPEED_A	(2.0f)								// 移動スピード
#define HADOU_SPEED_B	(3.0f)								// 移動スピード

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3			pos;			// 現在の位置
	D3DXVECTOR3			rot;			// 回転
	D3DXVECTOR3			scl;			// 大きさ
	LPDIRECT3DTEXTURE9	D3DTexture;		// テクスチャ読み込み場所
	LPD3DXMESH			Mesh;			// ID3DXMeshインターフェイスへのポインタ
	LPD3DXBUFFER		D3DXMatBuff;	// メッシュのマテリアル情報を格納
	DWORD				NumMat;			// 属性情報の総数
	D3DXMATRIX			mtxWorld;		// ワールドマトリックス
	bool				use;			// 使用しているかどうか
	D3DXVECTOR3			move;			// 移動量

} HADOU;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitHadou(int type, HADOU *Hadou);	// 初期化、BALL構造体、行列、半径
void UninitHadou(HADOU *Hadou);
void UpdateHadou(HADOU *Hadou);
void DrawHadou(HADOU *Hadou);
void SetHadou(HADOU *Hadou, BALL *HitBall, D3DXVECTOR3 CharaRot);

#endif
