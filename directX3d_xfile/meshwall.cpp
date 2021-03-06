//=============================================================================
//
// メッシュ壁の処理 [meshwall.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "meshwall.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTexture = NULL;		// テクスチャ読み込み場所

MESH_WALL g_aMeshWall[MAX_MESHWALL];			// メッシュ壁ワーク
MESH_WALL *pMesh = NULL;						// メッシュ壁のポインタ
int g_nNumMeshField = 0;						// メッシュ壁の数

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
						int nNumBlockX, int nNumBlockY, float fBlockSizeX, float fBlockSizeZ)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//MESH_WALL *pMesh;

	if(g_nNumMeshField >= MAX_MESHWALL)
	{
		return E_FAIL;
	}

	// テクスチャの読み込み
	if(!g_pD3DTexture)
	{
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
									TEXTURE_MESHWALL,		// ファイルの名前
									&g_pD3DTexture);		// 読み込むメモリー
	}

	pMesh = &g_aMeshWall[g_nNumMeshField];
	g_nNumMeshField++;

	// ポリゴン表示位置の中心座標を設定
	pMesh->pos = pos;

	pMesh->rot = rot;

	// ブロック数の設定
	pMesh->nNumBlockX = nNumBlockX;
	pMesh->nNumBlockY = nNumBlockY;

	// 頂点数の設定
	pMesh->nNumVertex = (nNumBlockX + 1) * (nNumBlockY + 1);

	// インデックス数の設定
	pMesh->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockY + (nNumBlockY - 1) * 2;

	// ポリゴン数の設定
	pMesh->nNumPolygon = nNumBlockX * nNumBlockY * 2 + (nNumBlockY - 1) * 4;

	// ブロックサイズの設定
	pMesh->fBlockSizeX = fBlockSizeX;
	pMesh->fBlockSizeY = fBlockSizeZ;

	// オブジェクトの頂点バッファを生成
    if(FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * pMesh->nNumVertex,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
												FVF_VERTEX_3D,						// 使用する頂点フォーマット
												D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
												&pMesh->pD3DVtxBuff,				// 頂点バッファインターフェースへのポインタ
												NULL)))								// NULLに設定
	{
        return E_FAIL;
	}

	// オブジェクトのインデックスバッファを生成
    if(FAILED( pDevice->CreateIndexBuffer(sizeof(WORD) * pMesh->nNumVertexIndex,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
												D3DFMT_INDEX16,						// 使用するインデックスフォーマット
												D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
												&pMesh->pD3DIdxBuff,				// インデックスバッファインターフェースへのポインタ
												NULL)))								// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockY;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		pMesh->pD3DVtxBuff->Lock( 0, 0, (void**)&pVtx, 0 );

		for(int nCntVtxY = 0; nCntVtxY < (pMesh->nNumBlockY + 1); nCntVtxY++)
		{
			for(int nCntVtxX = 0; nCntVtxX < (pMesh->nNumBlockX + 1); nCntVtxX++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].vtx.x = -(pMesh->nNumBlockX / 2.0f) * pMesh->fBlockSizeX + nCntVtxX * pMesh->fBlockSizeX;
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].vtx.y = pMesh->nNumBlockY * pMesh->fBlockSizeY - nCntVtxY * pMesh->fBlockSizeY;
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].vtx.z = 0.0f;

				// 法線の設定
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				// 反射光の設定
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].diffuse = col;

				// テクスチャ座標の設定
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxY;
			}
		}

		// 頂点データをアンロックする
		pMesh->pD3DVtxBuff->Unlock();
	}

	{//インデックスバッファの中身を埋める
		WORD *pIdx;

		// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
		pMesh->pD3DIdxBuff->Lock( 0, 0, (void**)&pIdx, 0 );

		int nCntIdx = 0;
		for(int nCntVtxY = 0; nCntVtxY < pMesh->nNumBlockY; nCntVtxY++)
		{
			if(nCntVtxY > 0)
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = (nCntVtxY + 1) * (pMesh->nNumBlockX + 1);
				nCntIdx++;
			}

			for(int nCntVtxX = 0; nCntVtxX < (pMesh->nNumBlockX + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxY + 1) * (pMesh->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
			}

			if(nCntVtxY < (pMesh->nNumBlockY - 1))
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = nCntVtxY * (pMesh->nNumBlockX + 1) + pMesh->nNumBlockX;
				nCntIdx++;
			}
		}

		// インデックスデータをアンロックする
		pMesh->pD3DIdxBuff->Unlock();
	}



	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshWall(void)
{
	//MESH_WALL *pMesh;
	int nCntMeshField;

	for(nCntMeshField = 0; nCntMeshField < g_nNumMeshField; nCntMeshField++)
	{
		pMesh = &g_aMeshWall[nCntMeshField];

		if(pMesh->pD3DVtxBuff)
		{// 頂点バッファの開放
			pMesh->pD3DVtxBuff->Release();
			pMesh->pD3DVtxBuff = NULL;
		}

		if(pMesh->pD3DIdxBuff)
		{// インデックスバッファの開放
			pMesh->pD3DIdxBuff->Release();
			pMesh->pD3DIdxBuff = NULL;
		}
	}

	if(g_pD3DTexture)
	{// テクスチャの開放
		g_pD3DTexture->Release();
		g_pD3DTexture = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshWall(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTranslate;

	//MESH_WALL *pMesh;
	int nCntMeshField;

	for(nCntMeshField = 0; nCntMeshField < g_nNumMeshField; nCntMeshField++)
	{
		pMesh = &g_aMeshWall[nCntMeshField];

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pMesh->mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh->rot.y, pMesh->rot.x, pMesh->rot.z);
		D3DXMatrixMultiply(&pMesh->mtxWorld, &pMesh->mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, pMesh->pos.x, pMesh->pos.y, pMesh->pos.z);
		D3DXMatrixMultiply(&pMesh->mtxWorld, &pMesh->mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pMesh->mtxWorld);

		// 頂点バッファをレンダリングパイプラインに設定
		pDevice->SetStreamSource(0, pMesh->pD3DVtxBuff, 0, sizeof(VERTEX_3D));

		// インデックスバッファをレンダリングパイプラインに設定
		pDevice->SetIndices(pMesh->pD3DIdxBuff);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTexture);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, pMesh->nNumVertex, 0, pMesh->nNumPolygon);
	}
}

//=============================================================================
// 壁との当たり判定
// pos0:始点（移動前）
// pos1:終点（移動後）
//=============================================================================
int hitCheckMeshwall(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	int				ans = 0;	// 一先ず当たっていないをセットしておく

	D3DXVECTOR3		pos[4];
	D3DXVECTOR3		nor;		// ポリゴンの法線

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	VERTEX_3D		*pVtx;
	pMesh->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i<1; i++, pVtx += 4)
	{	// まずは、ポリゴンの頂点を求める

		// 頂点座標の設定
		pos[0] = pVtx[0].vtx;
		pos[1] = pVtx[1].vtx;
		pos[2] = pVtx[2].vtx;
		pos[3] = pVtx[3].vtx;

		//// 左下側ポリゴンと線分の当たり判定
		//ans = hitCheck(pos[0], pos[2], pos[3], pos0, pos1);
		//if (ans != 0) break;

		//// 右上側ポリゴンと線分の当たり判定
		//ans = hitCheck(pos[0], pos[3], pos[1], pos0, pos1);
		//if (ans != 0) break;

	}

	// 頂点データをアンロックする
	pMesh->pD3DVtxBuff->Unlock();

	return(ans);

}
