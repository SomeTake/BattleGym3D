//=============================================================================
//
// 当たり判定用カプセル処理 [CapsuleMesh.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _CAPSULEMESH_H_
#define _CAPSULEMESH_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SPHERE_SLICE	(8)		// カプセル半球部のスライス数(円筒部の分割数にもなる)
#define SPHERE_STACK	(4)		// カプセル半球部のスタック数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 VtxBuffer;			// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	IdxBuffer;			// インデックスバッファへのポインタ
	D3DXMATRIX				WorldMatrix;		// ワールドマトリックス
	D3DXVECTOR3				P1, P2;				// 左と右側二つの半球体の中心座標
	D3DXVECTOR3				pos;				// カプセルの座標
	D3DXVECTOR3				vec;				// 描画するときの方向ベクトル
	D3DXVECTOR3				CreateVec;			// カプセルを作成するとき設置するベクトル
												// 例えば、Create = (0.0f,1.0f,0.0f)のとき
												// カプセルを回転させない場合、カプセルは真上に向く
	int						NumOfVtxBuffer;
	int						NumOfTriangle;
	float					Length;				// カプセルの長さ
	float					Radius;				// カプセルの半径
} CAPSULE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
//HRESULT InitCapsule(void);
HRESULT CreateCapsule(CAPSULE *Capsule, D3DXVECTOR3 Pos, D3DXVECTOR3 CreateVec, float Length, float Radius, bool DrawInCenter);
void UninitCapsule(CAPSULE *Capsule);
void UpdateCapsule(CAPSULE *Capsule);
void DrawCapsule(CAPSULE *Capsule,const D3DXMATRIX *WorldMatrix);

#endif
