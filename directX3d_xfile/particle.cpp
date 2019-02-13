//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "particle.h"
#include "camera.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		pD3DTextureParticle = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffParticle = NULL;		// 頂点バッファインターフェースへのポインタ

PARTICLE				Particle[MAX_PARTICLE];			// パーティクルワーク
static bool				g_bAlpaTest;					// アルファテストON/OFF
static int				g_nAlpha;						// アルファテストの閾値
D3DXVECTOR3 centerpos;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitParticle(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PARTICLE *particle = GetParticle(0);

	// 頂点情報の作成
	MakeVertexParticle(pDevice);

	// テクスチャの読み込み
	//D3DXCreateTextureFromFileEx(pDevice,	// デバイスへのポインタ
	//	TEXTURE_PARTICLE,					// ファイルの名前
	//	D3DX_DEFAULT,						// 横幅
	//	D3DX_DEFAULT,						// 縦幅
	//	1,									// ミップマップレベル
	//	D3DUSAGE_DEPTHSTENCIL,				// テクスチャの性質や確保するメモリの場所を指定
	//	D3DFMT_UNKNOWN,						// テクスチャのピクセルフォーマットを指定
	//	D3DPOOL_DEFAULT,					// テクスチャを格納するメモリの位置と管理に関するフラグ
	//	DWORD	Filter,
	//	DWORD	MipFilter,
	//	D3DCOLOR	ColorKey,
	//	D3DXIMAGE_INFO*	pSrcInfo,
	//	PALETTEENTRY*	pPalette,
	//	&pD3DTextureParticle,				// 読み込むメモリー
	//);

	if (type == 0)
	{
		D3DXCreateTextureFromFile(pDevice,							// デバイスへのポインタ
			TEXTURE_PARTICLE,			// ファイルの名前
			&pD3DTextureParticle);	// 読み込むメモリー
	}

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, particle++)
	{
		particle->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		particle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		particle->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		particle->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		particle->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		particle->fWidth = PARTICLE_WIDTH;
		particle->fHeight = PARTICLE_HEIGHT;
		particle->bUse = false;
		particle->time = 0;
		particle->theta = 0.0f;
	}

	g_bAlpaTest = false;
	g_nAlpha = 0x0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitParticle(void)
{
	PARTICLE *particle = GetParticle(0);

	for(int i = 0; i < MAX_PARTICLE; i++, particle++)
	{
		if (pD3DTextureParticle != NULL)
		{// テクスチャの開放
			pD3DTextureParticle->Release();
			pD3DTextureParticle = NULL;
		}

		if (pD3DVtxBuffParticle != NULL)
		{// 頂点バッファの開放
			pD3DVtxBuffParticle->Release();
			pD3DVtxBuffParticle = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateParticle(void)
{
	PARTICLE *particle = GetParticle(0);
	CHARA *playerWk = GetPlayer();


	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, particle++)
	{
		// 使用しているもののみ更新
		if (particle->bUse == true)
		{
			// パーティクルの動き
			// 運動量に合わせて動く
			particle->pos.x += particle->move.x;
			particle->pos.y += particle->move.y;
			particle->pos.z += particle->move.z;

			// 透明度を減衰値に合わせて追加
			particle->col.a -= particle->DecAlpha;
			if (particle->col.a <= 0.0f)
			{
				// 全て透明になったら使用をやめる
				particle->col.a = 0.0f;
				particle->bUse = false;
			}
			// 色の設定
			SetColorParticle(nCntParticle,
				D3DXCOLOR(Particle->col.r, particle->col.g,
					particle->col.b, particle->col.a));

			// 表示時間の更新
			particle->time--;
			if (particle->time <= 0)
			{
				particle->bUse = false;
			}

		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CAMERA *cameraWk = GetCamera(0);
	MATRIX matrix;
	PARTICLE *particle = GetParticle(0);

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 減算合成 レンダリングステートの変更→黒っぽくなる（加算合成は白っぽくなる（255に近づけていくと））
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);	// 結果 = 転送先(DEST) - 転送元(SRC)
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// Zテスト
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	// 通常ブレンド レンダリングステートをもとに戻す（戻さないと減算合成のままになる）
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 結果 = 転送元(SRC) + 転送先(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// Zテスト
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, particle++)
	{
		if (particle->bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&particle->mtxWorld);

			// ビューマトリックスを取得
			matrix.view = cameraWk->mtxView;

			// ポリゴンを正面に向ける
			particle->mtxWorld._11 = matrix.view._11;
			particle->mtxWorld._12 = matrix.view._21;
			particle->mtxWorld._13 = matrix.view._31;
			particle->mtxWorld._21 = matrix.view._12;
			particle->mtxWorld._22 = matrix.view._22;
			particle->mtxWorld._23 = matrix.view._32;
			particle->mtxWorld._31 = matrix.view._13;
			particle->mtxWorld._32 = matrix.view._23;
			particle->mtxWorld._33 = matrix.view._33;

#if 1
			// 逆行列をもとめる
			D3DXMatrixInverse(&particle->mtxWorld, NULL, &matrix.view);
			particle->mtxWorld._41 = 0.0f;
			particle->mtxWorld._42 = 0.0f;
			particle->mtxWorld._43 = 0.0f;
#else
			particle->mtxWorld._11 = mtxView._11;
			particle->mtxWorld._12 = mtxView._21;
			particle->mtxWorld._13 = mtxView._31;
			particle->mtxWorld._21 = mtxView._12;
			particle->mtxWorld._22 = mtxView._22;
			particle->mtxWorld._23 = mtxView._32;
			particle->mtxWorld._31 = mtxView._13;
			particle->mtxWorld._32 = mtxView._23;
			particle->mtxWorld._33 = mtxView._33;
#endif

			// スケールを反映
			D3DXMatrixScaling(&matrix.scale, particle->scl.x,
				particle->scl.y,
				particle->scl.z);
			D3DXMatrixMultiply(&particle->mtxWorld, &particle->mtxWorld, &matrix.scale);

			// 移動を反映
			D3DXMatrixTranslation(&matrix.translation, particle->pos.x,
				particle->pos.y,
				particle->pos.z);
			D3DXMatrixMultiply(&particle->mtxWorld, &particle->mtxWorld, &matrix.translation);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &particle->mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, pD3DVtxBuffParticle, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, pD3DTextureParticle);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntParticle * 4), NUM_POLYGON);
		}
	}

	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// αテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// 通常ブレンド レンダリングステートをもとに戻す（戻さないと減算合成のままになる）
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 結果 = 転送元(SRC) + 転送先(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Z比較あり
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice)
{
	PARTICLE *particle = GetParticle(0);

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_PARTICLE,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,						// 頂点バッファの使用法　
		FVF_VERTEX_3D,							// 使用する頂点フォーマット
		D3DPOOL_MANAGED,						// リソースのバッファを保持するメモリクラスを指定
		&pD3DVtxBuffParticle,						// 頂点バッファインターフェースへのポインタ
		NULL)))									// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		pD3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-PARTICLE_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-PARTICLE_WIDTH / 2.0f, PARTICLE_HEIGHT, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(PARTICLE_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(PARTICLE_WIDTH / 2.0f, PARTICLE_HEIGHT, 0.0f);

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
		}

		// 頂点データをアンロックする
		pD3DVtxBuffParticle->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexParticle(int nIdxParticle, float fWidth, float fHeight)
{
	PARTICLE *particle = GetParticle(nIdxParticle);

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		pD3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fWidth / 2.0f, fHeight, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fWidth / 2.0f, fHeight, 0.0f);

		// 頂点データをアンロックする
		pD3DVtxBuffParticle->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定 引数(nIdxParticle = 番号, col = 色)
//=============================================================================
void SetColorParticle(int nIdxParticle, D3DXCOLOR col)
{
	PARTICLE *particle = GetParticle(nIdxParticle);

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		pD3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle * 4);

		// 頂点座標の設定
		pVtx[0].diffuse = col;
		pVtx[1].diffuse = col;
		pVtx[2].diffuse = col;
		pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		pD3DVtxBuffParticle->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fWidth, float fHeight, int Time)
{
	PARTICLE *particle = GetParticle(0);
	int nIdxParticle = -1;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, particle++)
	{
		if (!particle->bUse)
		{
			particle->pos = pos;
			particle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			particle->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			particle->move = move;
			particle->col = col;
			particle->fWidth = fWidth;
			particle->fHeight = fHeight;
			particle->time = Time;
			particle->DecAlpha = col.a / Time;
			particle->bUse = true;

			// 頂点座標の設定
			SetVertexParticle(nCntParticle, fWidth, fHeight);

			// 頂点カラーの設定
			SetColorParticle(nCntParticle,particle->col);

			nIdxParticle = nCntParticle;

			break;
		}
	}

	return nIdxParticle;
}

//=============================================================================
// パーティクルを取得
//=============================================================================
PARTICLE *GetParticle(int no)
{
	return &Particle[no];
}

//=============================================================================
// 使用しているパーティクルの数を数える
//=============================================================================
int NumParticle(void)
{
	PARTICLE *particle = GetParticle(0);

	int Num = 0;

	for (int i = 0; i < MAX_PARTICLE; i++, particle++)
	{
		if (particle->bUse == true)
		{
			Num++;
		}
	}

	return Num;
}

//=============================================================================
// 攻撃ヒット時に使用するパーティクルエフェクト
//=============================================================================
void SetHitParticle(D3DXVECTOR3 AttackPos)
{
	D3DXVECTOR3 move;

	for (int i = 0; i < ATTACK_PERTICLE_NUM; i++)
	{
		move.x = (float)(rand() % 10 - 5);
		move.y = (float)(rand() % 10 - 5);
		move.z = (float)(rand() % 10 - 5);

		SetParticle(AttackPos, D3DXVECTOR3(move.x, move.y, move.z),
			ORANGE(0.5f), ATTACK_SIZE_A.x, ATTACK_SIZE_A.y, ATTACK_TIME_C);
	}
}

//=============================================================================
// 移動時に使用するパーティクルエフェクト
//=============================================================================
void SetWalkParticle(D3DXVECTOR3 pos)
{
	SetParticle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		WHITE(0.50f), PARTICLE_SIZE_A.x, PARTICLE_SIZE_A.y, PARTICLE_TIME_A);
	SetParticle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		WHITE(0.30f), PARTICLE_SIZE_B.x, PARTICLE_SIZE_B.y, PARTICLE_TIME_B);
	SetParticle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		WHITE(0.15f), PARTICLE_SIZE_C.x, PARTICLE_SIZE_C.y, PARTICLE_TIME_C);
}

//=============================================================================
// SP攻撃時に使用するパーティクルエフェクト
//=============================================================================
void SetSPattackParticle(D3DXVECTOR3 pos)
{
	for (int i = 0; i < SP_PERTICLE_NUM; i++)
	{
		pos.x += (float)(rand() % 20 - 10) * 0.1f;
		pos.y += (float)(rand() % 20 - 10) * 0.1f;
		pos.z += (float)(rand() % 20 - 10) * 0.1f;

		SetParticle(pos, D3DXVECTOR3(0.0f, 1.0f, 0.0f),
			RED(0.50f), ATTACK_SIZE_A.x, ATTACK_SIZE_A.y, ATTACK_TIME_A);
		SetParticle(pos, D3DXVECTOR3(0.0f, 1.0f, 0.0f),
			VERMILION(0.30f), ATTACK_SIZE_B.x, ATTACK_SIZE_B.y, ATTACK_TIME_B);
		SetParticle(pos, D3DXVECTOR3(0.0f, 1.0f, 0.0f),
			ORANGE(0.15f), ATTACK_SIZE_C.x, ATTACK_SIZE_C.y, ATTACK_TIME_C);
	}
}

//=============================================================================
// 波動拳に使用するパーティクルエフェクト
//=============================================================================
void SetHadouParticle(D3DXVECTOR3 pos, float radius)
{
	D3DXVECTOR3 EffectPos;

	for (int angle1 = 0; angle1 < 360; angle1 += 45)
	{
		for (int angle2 = 0; angle2 < 360; angle2 += 45)
		{
			EffectPos = pos + GetSpherePosition((float)angle1, (float)angle2, radius);
			SetParticle(EffectPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				VERMILION(0.50f), PARTICLE_SIZE_A.x, PARTICLE_SIZE_A.y, ATTACK_TIME_A);
			SetParticle(EffectPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				VERMILION(0.30f), PARTICLE_SIZE_B.x, PARTICLE_SIZE_B.y, ATTACK_TIME_B);
			SetParticle(EffectPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				VERMILION(0.15f), PARTICLE_SIZE_C.x, PARTICLE_SIZE_C.y, ATTACK_TIME_C);
		}
	}
}

//=============================================================================
// 球体表面上の任意座標を取得する
//=============================================================================
D3DXVECTOR3 GetSpherePosition(float Angle1, float Angle2, float Radius)
{
	D3DXVECTOR3 Pos;
	
	Pos.x = Radius * sinf(D3DXToRadian(Angle1)) * cosf(D3DXToRadian(Angle2));
	Pos.y = Radius * sinf(D3DXToRadian(Angle1)) * sinf(D3DXToRadian(Angle2));
	Pos.z = Radius * cosf(D3DXToRadian(Angle1));
	
	return Pos;
}

//=============================================================================
// ガード時に使用するパーティクルエフェクト
//=============================================================================
void SetGuardParticle(D3DXVECTOR3 DefendPos)
{
	D3DXVECTOR3 move;

	for (int i = 0; i < ATTACK_PERTICLE_NUM; i++)
	{
		move.x = (float)(rand() % 5 - 2);
		move.y = (float)(rand() % 5 - 2);
		move.z = (float)(rand() % 5 - 2);

		SetParticle(DefendPos, D3DXVECTOR3(move.x, move.y, move.z),
			WHITE(0.5f), ATTACK_SIZE_A.x, ATTACK_SIZE_A.y, ATTACK_TIME_C);
	}
}
