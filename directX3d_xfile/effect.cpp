//=============================================================================
//
// 1P2P表示処理 [effect.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "main.h"
#include "effect.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEffect(EFFECT *EffectWk, LPDIRECT3DDEVICE9 pDevice);
void SetVertexEffect(EFFECT *EffectWk, int nIdxEffect, float fWidth, float fHeight);
void SetColorEffect(EFFECT *EffectWk, int nIdxEffect, D3DXCOLOR col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEffect(int type, EFFECT *EffectWk)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexEffect(EffectWk, pDevice);

	if (type == 0)
	{
		// テクスチャの初期化
		D3DXCreateTextureFromFile(pDevice,							// デバイスへのポインタ
			TEXTURE_HIT00,			// ファイルの名前
			&EffectWk->D3DTexture[hit00]);	// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,							// デバイスへのポインタ
			TEXTURE_HIT01,			// ファイルの名前
			&EffectWk->D3DTexture[hit01]);	// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,							// デバイスへのポインタ
			TEXTURE_GUARD00,			// ファイルの名前
			&EffectWk->D3DTexture[guard00]);	// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,							// デバイスへのポインタ
			TEXTURE_CHARGE00,			// ファイルの名前
			&EffectWk->D3DTexture[charge00]);	// 読み込むメモリー

	}

	EffectWk->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	EffectWk->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	EffectWk->width = EFFECT_WIDTH;
	EffectWk->height = EFFECT_HEIGHT;
	EffectWk->use = false;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(EFFECT *EffectWk)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (EffectWk->D3DTexture[i] != NULL)
		{// テクスチャの開放
			EffectWk->D3DTexture[i]->Release();
			EffectWk->D3DTexture[i] = NULL;
		}
	}

	if (EffectWk->D3DVtxBuff != NULL)
	{// 頂点バッファの開放
		EffectWk->D3DVtxBuff->Release();
		EffectWk->D3DVtxBuff = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEffect(EFFECT *EffectWk)
{
	
	EffectWk->cntPattern++;
	switch (EffectWk->num)
	{
	case hit00:
		if (EffectWk->cntPattern == ANIM_PATTERN_NUM_HIT00)
		{
			EffectWk->use = false;
		}
		break;
	case hit01:
		if (EffectWk->cntPattern == ANIM_PATTERN_NUM_HIT01)
		{
			EffectWk->use = false;
		}
		break;
	case guard00:
		if (EffectWk->cntPattern == ANIM_PATTERN_NUM_GUARD)
		{
			EffectWk->use = false;
		}
		break;
	case charge00:
		if (EffectWk->cntPattern == ANIM_PATTERN_NUM_CHARGE)
		{
			EffectWk->use = false;
		}
		break;
	default:
		break;
	}

	SetTextureEffect(EffectWk, EffectWk->cntPattern);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(EFFECT *EffectWk)
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
	D3DXMatrixIdentity(&EffectWk->mtxWorld);

	// ビューマトリックスを取得
	mtxView = cameraWk->mtxView;

	// ポリゴンを正面に向ける
#if 1
	// 逆行列をもとめる
	D3DXMatrixInverse(&EffectWk->mtxWorld, NULL, &mtxView);
	EffectWk->mtxWorld._41 = 0.0f;
	EffectWk->mtxWorld._42 = 0.0f;
	EffectWk->mtxWorld._43 = 0.0f;
#else
	EffectWk->mtxWorld._11 = mtxView._11;
	EffectWk->mtxWorld._12 = mtxView._21;
	EffectWk->mtxWorld._13 = mtxView._31;
	EffectWk->mtxWorld._21 = mtxView._12;
	EffectWk->mtxWorld._22 = mtxView._22;
	EffectWk->mtxWorld._23 = mtxView._32;
	EffectWk->mtxWorld._31 = mtxView._13;
	EffectWk->mtxWorld._32 = mtxView._23;
	EffectWK.mtxWorld._33 = mtxView._33;
#endif

	// スケールを反映
	D3DXMatrixScaling(&mtxScale, EffectWk->scl.x,
		EffectWk->scl.y,
		EffectWk->scl.z);
	D3DXMatrixMultiply(&EffectWk->mtxWorld, &EffectWk->mtxWorld, &mtxScale);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, EffectWk->pos.x,
		EffectWk->pos.y,
		EffectWk->pos.z);
	D3DXMatrixMultiply(&EffectWk->mtxWorld, &EffectWk->mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &EffectWk->mtxWorld);

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, EffectWk->D3DVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	switch (EffectWk->num)
	{
	case hit00:
		// テクスチャの設定
		pDevice->SetTexture(0, EffectWk->D3DTexture[hit00]);
		break;
	case hit01:
		// テクスチャの設定
		pDevice->SetTexture(0, EffectWk->D3DTexture[hit01]);
		break;
	case guard00:
		// テクスチャの設定
		pDevice->SetTexture(0, EffectWk->D3DTexture[guard00]);
		break;
	case charge00:
		// テクスチャの設定
		pDevice->SetTexture(0, EffectWk->D3DTexture[charge00]);
		break;
	default:
		break;
	}

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
HRESULT MakeVertexEffect(EFFECT *EffectWk, LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,				// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,						// 頂点バッファの使用法　
		FVF_VERTEX_3D,							// 使用する頂点フォーマット
		D3DPOOL_MANAGED,						// リソースのバッファを保持するメモリクラスを指定
		&EffectWk->D3DVtxBuff,						// 頂点バッファインターフェースへのポインタ
		NULL)))									// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		EffectWk->D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-EFFECT_WIDTH / 2.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-EFFECT_WIDTH / 2.0f, EFFECT_HEIGHT, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(EFFECT_WIDTH / 2.0f, 0.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(EFFECT_WIDTH / 2.0f, EFFECT_HEIGHT, 0.0f);

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
		EffectWk->D3DVtxBuff->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexEffect(EFFECT *EffectWk, int nIdxEffect, float fWidth, float fHeight)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		EffectWk->D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fWidth / 2.0f, fHeight, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fWidth / 2.0f, fHeight, 0.0f);

		// 頂点データをアンロックする
		EffectWk->D3DVtxBuff->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorEffect(EFFECT *EffectWk, int nIdxEffect, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		EffectWk->D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].diffuse = col;
		pVtx[1].diffuse = col;
		pVtx[2].diffuse = col;
		pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		EffectWk->D3DVtxBuff->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetEffect(EFFECT *EffectWk, D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col, int num)
{
	EffectWk->pos = pos;
	EffectWk->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	EffectWk->width = fWidth;
	EffectWk->height = fHeight;

	// 頂点座標の設定
	SetVertexEffect(EffectWk, 0, fWidth, fHeight);

	// 頂点カラーの設定
	SetColorEffect(EffectWk, 0, col);

	// 使用するエフェクトの種類
	EffectWk->cntPattern = 0;
	EffectWk->num = num;
	EffectWk->use = true;

	return 0;
}

//=============================================================================
// テクスチャ座標の作成
//=============================================================================
void SetTextureEffect(EFFECT *EffectWk, int cntPattern)
{
	int x,y;
	float sizeX, sizeY;

	switch (EffectWk->num)
	{
	case hit00:
		x = cntPattern % HIT00_PATTERN_DIVIDE_X;
		y = cntPattern / HIT00_PATTERN_DIVIDE_X;
		sizeX = 1.0f / HIT00_PATTERN_DIVIDE_X;
		sizeY = 1.0f / HIT00_PATTERN_DIVIDE_Y;
		break;
	case hit01:
		x = cntPattern % HIT01_PATTERN_DIVIDE_X;
		y = cntPattern / HIT01_PATTERN_DIVIDE_X;
		sizeX = 1.0f / HIT01_PATTERN_DIVIDE_X;
		sizeY = 1.0f / HIT01_PATTERN_DIVIDE_Y;
		break;
	case guard00:
		x = cntPattern % GUARD_PATTERN_DIVIDE_X;
		y = cntPattern / GUARD_PATTERN_DIVIDE_X;
		sizeX = 1.0f / GUARD_PATTERN_DIVIDE_X;
		sizeY = 1.0f / GUARD_PATTERN_DIVIDE_Y;
		break;
	case charge00:
		x = cntPattern % CHARGE_PATTERN_DIVIDE_X;
		y = cntPattern / CHARGE_PATTERN_DIVIDE_X;
		sizeX = 1.0f / CHARGE_PATTERN_DIVIDE_X;
		sizeY = 1.0f / CHARGE_PATTERN_DIVIDE_Y;
		break;
	default:
		break;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		EffectWk->D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);

		// 頂点データをアンロックする
		EffectWk->D3DVtxBuff->Unlock();
	}
}