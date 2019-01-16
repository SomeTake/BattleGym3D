//=============================================================================
//
// メッシュ壁の処理 [meshwall.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_MESHWALL		(10)							// 壁の総数
#define	TEXTURE_MESHWALL	"data/TEXTURE/wall000.jpg"		// 読み込むテクスチャファイル名
#define	VALUE_MOVE_MESHWALL		(5.0f)							// 移動速度
#define	VALUE_ROTATE_MESHWALL	(D3DX_PI * 0.001f)				// 回転速度

//*****************************************************************************
// グローバル変数
//*****************************************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;		// 頂点バッファインターフェースへのポインタ
	LPDIRECT3DINDEXBUFFER9 pD3DIdxBuff;			// インデックスバッファインターフェースへのポインタ

	D3DXMATRIX mtxWorld;						// ワールドマトリックス
	D3DXVECTOR3 pos;							// ポリゴン表示位置の中心座標
	D3DXVECTOR3 rot;							// ポリゴンの回転角
	int nNumBlockX, nNumBlockY;					// ブロック数
	int nNumVertex;								// 総頂点数	
	int nNumVertexIndex;						// 総インデックス数
	int nNumPolygon;							// 総ポリゴン数
	float fBlockSizeX, fBlockSizeY;				// ブロックサイズ
} MESH_WALL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
						int nNumBlockX, int nNumBlockY, float fSizeBlockX, float fSizeBlockY);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);
int hitCheckMeshwall(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);

#endif
