//=============================================================================
//
// SPゲージMAX表示処理 [spmax.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "spmax.h"
#include "spguage.h"
#include "espguage.h"
#include "player.h"
#include "enemy.h"
#include "sound.h"
#include "battle.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureSpmax = NULL;				// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureESpmax = NULL;				// テクスチャへのポインタ

VERTEX_2D				g_vertexWkSpmax[NUM_VERTEX];			// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkESpmax[NUM_VERTEX];			// 頂点情報格納ワーク

bool					SpmaxUse;								// 使用中か
bool					ESpmaxUse;								// 使用中か

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSpmax(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_SPMAX,				// ファイルの名前
			&g_pD3DTextureSpmax);		// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_SPMAX,			// ファイルの名前
			&g_pD3DTextureESpmax);	// 読み込むメモリー
	}

	SpmaxUse = false;
	ESpmaxUse = false;

	// 頂点情報の作成
	MakeVertexSpmax();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSpmax(void)
{
	if (g_pD3DTextureSpmax != NULL)
	{// テクスチャの開放
		g_pD3DTextureSpmax->Release();
		g_pD3DTextureSpmax = NULL;
	}

	if (g_pD3DTextureESpmax != NULL)
	{// テクスチャの開放
		g_pD3DTextureESpmax->Release();
		g_pD3DTextureESpmax = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSpmax(void)
{
	CHARA *enemyWk = GetEnemy();
	CHARA *playerWk = GetPlayer();

	//音を鳴らすためのフラグ
	static bool soundflag = false;
	static bool esoundflag = false;

	int phase = *GetPhase();

	//プレイヤーのSPゲージがMAXの場合使う
	if (playerWk->SP == FULL_SPGUAGE)
	{
		SpmaxUse = true;
		//MAXになった瞬間音を鳴らす
		if (soundflag == false && phase == PhaseGame)
		{
			PlaySound(SE_EFFECT0, 0, 0);
			soundflag = true;
		}
	}
	else if (playerWk->SP < FULL_SPGUAGE)
	{
		SpmaxUse = false;
		if (phase == PhaseGame)
		{
			soundflag = false;
		}
	}

	//エネミーのSPゲージがMAXの場合使う
	if (enemyWk->SP == FULL_SPGUAGE)
	{
		ESpmaxUse = true;
		//MAXになった瞬間音を鳴らす
		if (esoundflag == false && phase == PhaseGame)
		{
			PlaySound(SE_EFFECT0, 0, 0);
			esoundflag = true;
		}
	}
	else if (enemyWk->SP < FULL_SPGUAGE)
	{
		ESpmaxUse = false;
		if (phase == PhaseGame)
		{
			esoundflag = false;
		}
	}
	SetTextureSpmax();

	SetVertexSpmax();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSpmax(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SPGUAGE *spguage = GetSpGuage(0);
	ESPGUAGE *espguage = GetESpGuage(0);

	//プレイヤーのSPゲージがMAXなら描画
	if (SpmaxUse == true)
	{
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureSpmax);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkSpmax, sizeof(VERTEX_2D));
	}

	//エネミーのSPゲージがMAXなら描画
	if (ESpmaxUse == true)
	{
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureESpmax);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkESpmax, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexSpmax(void)
{
	// プレイヤー用
	// 頂点座標の設定
	g_vertexWkSpmax[0].vtx = D3DXVECTOR3(SPMAX_POS_X, SPMAX_POS_Y, 0.0f);
	g_vertexWkSpmax[1].vtx = D3DXVECTOR3(SPMAX_POS_X + SPMAX_SIZE_X, SPMAX_POS_Y, 0.0f);
	g_vertexWkSpmax[2].vtx = D3DXVECTOR3(SPMAX_POS_X, SPMAX_POS_Y + SPMAX_SIZE_Y, 0.0f);
	g_vertexWkSpmax[3].vtx = D3DXVECTOR3(SPMAX_POS_X + SPMAX_SIZE_X, SPMAX_POS_Y + SPMAX_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkSpmax[0].rhw =
		g_vertexWkSpmax[1].rhw =
		g_vertexWkSpmax[2].rhw =
		g_vertexWkSpmax[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkSpmax[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkSpmax[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkSpmax[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkSpmax[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkSpmax[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkSpmax[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkSpmax[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkSpmax[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// エネミー用
	// 頂点座標の設定
	g_vertexWkESpmax[0].vtx = D3DXVECTOR3(ESPMAX_POS_X, ESPMAX_POS_Y, 0.0f);
	g_vertexWkESpmax[1].vtx = D3DXVECTOR3(ESPMAX_POS_X + SPMAX_SIZE_X, ESPMAX_POS_Y, 0.0f);
	g_vertexWkESpmax[2].vtx = D3DXVECTOR3(ESPMAX_POS_X, ESPMAX_POS_Y + SPMAX_SIZE_Y, 0.0f);
	g_vertexWkESpmax[3].vtx = D3DXVECTOR3(ESPMAX_POS_X + SPMAX_SIZE_X, ESPMAX_POS_Y + SPMAX_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkESpmax[0].rhw =
		g_vertexWkESpmax[1].rhw =
		g_vertexWkESpmax[2].rhw =
		g_vertexWkESpmax[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkESpmax[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkESpmax[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkESpmax[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkESpmax[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkESpmax[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkESpmax[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkESpmax[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkESpmax[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}


//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureSpmax(void)
{
	// プレイヤー用
	g_vertexWkSpmax[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkSpmax[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkSpmax[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkSpmax[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// エネミー用
	g_vertexWkESpmax[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkESpmax[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkESpmax[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkESpmax[3].tex = D3DXVECTOR2(1.0f, 1.0f);

}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexSpmax(void)
{
	// プレイヤー用
	g_vertexWkSpmax[0].vtx = D3DXVECTOR3(SPMAX_POS_X, SPMAX_POS_Y, 0.0f);
	g_vertexWkSpmax[1].vtx = D3DXVECTOR3(SPMAX_POS_X + SPMAX_SIZE_X, SPMAX_POS_Y, 0.0f);
	g_vertexWkSpmax[2].vtx = D3DXVECTOR3(SPMAX_POS_X, SPMAX_POS_Y + SPMAX_SIZE_Y, 0.0f);
	g_vertexWkSpmax[3].vtx = D3DXVECTOR3(SPMAX_POS_X + SPMAX_SIZE_X, SPMAX_POS_Y + SPMAX_SIZE_Y, 0.0f);

	// エネミー用
	g_vertexWkESpmax[0].vtx = D3DXVECTOR3(ESPMAX_POS_X, ESPMAX_POS_Y, 0.0f);
	g_vertexWkESpmax[1].vtx = D3DXVECTOR3(ESPMAX_POS_X + SPMAX_SIZE_X, ESPMAX_POS_Y, 0.0f);
	g_vertexWkESpmax[2].vtx = D3DXVECTOR3(ESPMAX_POS_X, ESPMAX_POS_Y + SPMAX_SIZE_Y, 0.0f);
	g_vertexWkESpmax[3].vtx = D3DXVECTOR3(ESPMAX_POS_X + SPMAX_SIZE_X, ESPMAX_POS_Y + SPMAX_SIZE_Y, 0.0f);
}