//=============================================================================
//
// 2P用SPゲージ画面処理 [espguage.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "espguage.h"
#include "enemy.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureESpGuage = NULL;		// テクスチャへのポインタ

ESPGUAGE espguage[ESPGUAGE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitESpGuage(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_ESPGUAGE,				// ファイルの名前
			&g_pD3DTextureESpGuage);		// 読み込むメモリー

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// ゲージの初期化
	ESPGUAGE *espguage = GetESpGuage(0);
	espguage->use = true;
	espguage->pos = D3DXVECTOR3(ESPGUAGE_POS_X, ESPGUAGE_POS_Y, 0.0f);
	espguage->PatternAnim = 0;
	espguage->CountAnim = 0;

	// 頂点情報の作成
	MakeVertexESpGuage();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitESpGuage(void)
{
	if (g_pD3DTextureESpGuage != NULL)
	{// テクスチャの開放
		g_pD3DTextureESpGuage->Release();
		g_pD3DTextureESpGuage = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateESpGuage(void)
{
	ESPGUAGE *espguage = GetESpGuage(0);

	if (espguage->use == true)
	{
		espguage->PatternAnim = 1;

		//テクスチャ座標をセット
		SetTextureESpGuage(espguage->PatternAnim);

		SetVertexESpGuage();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawESpGuage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ESPGUAGE *espguage = GetESpGuage(0);

	if (espguage->use == true)
	{

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureESpGuage);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, espguage->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexESpGuage(void)
{
	// 頂点座標の設定
	SetVertexESpGuage();

	// テクスチャのパースペクティブコレクト用
	espguage->vertexWk[0].rhw =
		espguage->vertexWk[1].rhw =
		espguage->vertexWk[2].rhw =
		espguage->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	espguage->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	espguage->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	espguage->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	espguage->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	espguage->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	espguage->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	espguage->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	espguage->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureESpGuage(int cntPattern)
{
	int x = cntPattern % ESPGUAGE_PATTERN_DIVIDE_X;
	int y = cntPattern / ESPGUAGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / ESPGUAGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / ESPGUAGE_PATTERN_DIVIDE_Y;

	// テクスチャ座標の設定
	espguage->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	espguage->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	espguage->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	espguage->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexESpGuage(void)
{
	CHARA *enemyWk = GetEnemy();

	// 頂点座標の設定
	espguage->vertexWk[0].vtx = D3DXVECTOR3(espguage->pos.x + ESPGUAGE_SIZE_X - ESPGUAGE_SIZE_X * ((float)enemyWk->SP / (float)FULL_SPGUAGE), espguage->pos.y, espguage->pos.z);
	espguage->vertexWk[1].vtx = D3DXVECTOR3(espguage->pos.x + ESPGUAGE_SIZE_X, espguage->pos.y, espguage->pos.z);
	espguage->vertexWk[2].vtx = D3DXVECTOR3(espguage->pos.x + ESPGUAGE_SIZE_X - ESPGUAGE_SIZE_X * ((float)enemyWk->SP / (float)FULL_SPGUAGE), espguage->pos.y + ESPGUAGE_SIZE_Y, espguage->pos.z);
	espguage->vertexWk[3].vtx = D3DXVECTOR3(espguage->pos.x + ESPGUAGE_SIZE_X, espguage->pos.y + ESPGUAGE_SIZE_Y, espguage->pos.z);
}

//=============================================================================
// エネミーSPゲージのゲッター
//=============================================================================
ESPGUAGE *GetESpGuage(int gno)
{
	return &espguage[gno];
}
