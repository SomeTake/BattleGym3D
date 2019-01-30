//=============================================================================
//
// 1P2P表示処理 [pop.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "main.h"
#include "pop.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPop(POP *PopWk, LPDIRECT3DDEVICE9 pDevice);
void SetVertexPop(POP *PopWk, int nIdxPop, float fWidth, float fHeight);
void SetColorPop(POP *PopWk, int nIdxPop, D3DXCOLOR col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPop(int type, POP *PopWk, int ControllerNum)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexPop(PopWk, pDevice);

	if (type == 0)
	{
		// テクスチャの初期化
		switch (ControllerNum)
		{
		case 0:
			D3DXCreateTextureFromFile(pDevice,							// デバイスへのポインタ
				TEXTURE_POP1P,			// ファイルの名前
				&PopWk->D3DTexture);	// 読み込むメモリー
			break;
		case 1:
			D3DXCreateTextureFromFile(pDevice,							// デバイスへのポインタ
				TEXTURE_POP2P,			// ファイルの名前
				&PopWk->D3DTexture);	// 読み込むメモリー
			break;
		default:
			break;
		}
	}

	PopWk->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PopWk->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	PopWk->width = POP_WIDTH;
	PopWk->height = POP_HEIGHT;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPop(POP *PopWk)
{
	if (PopWk->D3DTexture != NULL)
	{// テクスチャの開放
		PopWk->D3DTexture->Release();
		PopWk->D3DTexture = NULL;
	}

	if (PopWk->D3DVtxBuff != NULL)
	{// 頂点バッファの開放
		PopWk->D3DVtxBuff->Release();
		PopWk->D3DVtxBuff = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePop(POP *PopWk, D3DXVECTOR3 pos)
{
	PopWk->pos = pos + POP_POS;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPop(POP *PopWk)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;
	CAMERA *cameraWk = GetCamera(0);

	//// 通常ブレンド レンダリングステートをもとに戻す（戻さないと減算合成のままになる）
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 結果 = 転送元(SRC) + 転送先(DEST)
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//// Zテスト
	////pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&PopWk->mtxWorld);

	// ビューマトリックスを取得
	mtxView = cameraWk->mtxView;

	// ポリゴンを正面に向ける
#if 1
	// 逆行列をもとめる
	D3DXMatrixInverse(&PopWk->mtxWorld, NULL, &mtxView);
	PopWk->mtxWorld._41 = 0.0f;
	PopWk->mtxWorld._42 = 0.0f;
	PopWk->mtxWorld._43 = 0.0f;
#else
	PopWk->mtxWorld._11 = mtxView._11;
	PopWk->mtxWorld._12 = mtxView._21;
	PopWk->mtxWorld._13 = mtxView._31;
	PopWk->mtxWorld._21 = mtxView._12;
	PopWk->mtxWorld._22 = mtxView._22;
	PopWk->mtxWorld._23 = mtxView._32;
	PopWk->mtxWorld._31 = mtxView._13;
	PopWk->mtxWorld._32 = mtxView._23;
	PopWK.mtxWorld._33 = mtxView._33;
#endif

	// スケールを反映
	D3DXMatrixScaling(&mtxScale, PopWk->scl.x,
		PopWk->scl.y,
		PopWk->scl.z);
	D3DXMatrixMultiply(&PopWk->mtxWorld, &PopWk->mtxWorld, &mtxScale);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, PopWk->pos.x,
		PopWk->pos.y,
		PopWk->pos.z);
	D3DXMatrixMultiply(&PopWk->mtxWorld, &PopWk->mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &PopWk->mtxWorld);

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, PopWk->D3DVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, PopWk->D3DTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// αテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexPop(POP *PopWk, LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,				// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,						// 頂点バッファの使用法　
												FVF_VERTEX_3D,							// 使用する頂点フォーマット
												D3DPOOL_MANAGED,						// リソースのバッファを保持するメモリクラスを指定
												&PopWk->D3DVtxBuff,						// 頂点バッファインターフェースへのポインタ
												NULL)))									// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		PopWk->D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-POP_WIDTH / 2.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-POP_WIDTH / 2.0f, POP_HEIGHT, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(POP_WIDTH / 2.0f, 0.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(POP_WIDTH / 2.0f, POP_HEIGHT, 0.0f);

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// 頂点データをアンロックする
		PopWk->D3DVtxBuff->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexPop(POP *PopWk, int nIdxPop, float fWidth, float fHeight)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		PopWk->D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fWidth / 2.0f, fHeight, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fWidth / 2.0f, fHeight, 0.0f);

		// 頂点データをアンロックする
		PopWk->D3DVtxBuff->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorPop(POP *PopWk, int nIdxPop, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		PopWk->D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].diffuse = col;
		pVtx[1].diffuse = col;
		pVtx[2].diffuse = col;
		pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		PopWk->D3DVtxBuff->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetPop(POP *PopWk, D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col)
{
	PopWk->pos = pos;
	PopWk->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	PopWk->width = fWidth;
	PopWk->height = fHeight;

	// 頂点座標の設定
	SetVertexPop(PopWk, 0, fWidth, fHeight);

	// 頂点カラーの設定
	SetColorPop(PopWk, 0, col);

	return 0;
}
