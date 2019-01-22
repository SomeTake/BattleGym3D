//=============================================================================
//
// SPゲージ画面処理 [spguage.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "spguage.h"
#include "player.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureSpGuage = NULL;		// テクスチャへのポインタ

SPGUAGE spguage[SPGUAGE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSpGuage(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_SPGUAGE,				// ファイルの名前
			&g_pD3DTextureSpGuage);		// 読み込むメモリー

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// ゲージの初期化

	SPGUAGE *spguage = GetSpGuage(0);
	spguage->use = true;
	spguage->pos = D3DXVECTOR3(SPGUAGE_POS_X, SPGUAGE_POS_Y, 0.0f);
	spguage->sp = 0;
	spguage->PatternAnim = 0;
	spguage->CountAnim = 0;

	// 頂点情報の作成
	MakeVertexSpGuage();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSpGuage(void)
{
	if (g_pD3DTextureSpGuage != NULL)
	{// テクスチャの開放
		g_pD3DTextureSpGuage->Release();
		g_pD3DTextureSpGuage = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSpGuage(void)
{
	CHARA *player = GetPlayer();
	SPGUAGE *spguage = GetSpGuage(0);

	if (spguage->use == true)
	{
		spguage->PatternAnim = 1;

		//テクスチャ座標をセット
		SetTextureSpGuage(spguage->PatternAnim);

		SetVertexSpGuage();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSpGuage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SPGUAGE *spguage = GetSpGuage(0);

	if (spguage->use == true)
	{

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureSpGuage);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, spguage->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexSpGuage(void)
{

	// 頂点座標の設定
	SetVertexSpGuage();

	// テクスチャのパースペクティブコレクト用
	spguage->vertexWk[0].rhw =
		spguage->vertexWk[1].rhw =
		spguage->vertexWk[2].rhw =
		spguage->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	spguage->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	spguage->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	spguage->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	spguage->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	spguage->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	spguage->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	spguage->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	spguage->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureSpGuage(int cntPattern)
{

	int x = cntPattern % SPGUAGE_PATTERN_DIVIDE_X;
	int y = cntPattern / SPGUAGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / SPGUAGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / SPGUAGE_PATTERN_DIVIDE_Y;

	// テクスチャ座標の設定
	spguage->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	spguage->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	spguage->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	spguage->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexSpGuage(void)
{

	// 頂点座標の設定
	spguage->vertexWk[0].vtx = D3DXVECTOR3(spguage->pos.x, spguage->pos.y, spguage->pos.z);
	spguage->vertexWk[1].vtx = D3DXVECTOR3(spguage->pos.x + SPGUAGE_SIZE_X * ((float)spguage->sp / (float)FULL_SPGUAGE), spguage->pos.y, spguage->pos.z);
	spguage->vertexWk[2].vtx = D3DXVECTOR3(spguage->pos.x, spguage->pos.y + SPGUAGE_SIZE_Y, spguage->pos.z);
	spguage->vertexWk[3].vtx = D3DXVECTOR3(spguage->pos.x + SPGUAGE_SIZE_X * ((float)spguage->sp / (float)FULL_SPGUAGE), spguage->pos.y + SPGUAGE_SIZE_Y, spguage->pos.z);
}

//=============================================================================
// プレイヤーSPゲージのゲッター
//=============================================================================
SPGUAGE *GetSpGuage(int gno)
{
	return &spguage[gno];
}

//=============================================================================
// SPゲージの増加
// 引数:add :追加する点数。マイナスも可能、初期化などに。
//=============================================================================
void AddSpGuage(int add)
{
	SPGUAGE *spguage = GetSpGuage(0);
	spguage->sp += add;

	//カンスト処理
	if (spguage->sp > FULL_SPGUAGE)
	{
		spguage->sp = FULL_SPGUAGE;
	}
	else if (spguage->sp < 0)
	{
		spguage->sp = 0;
	}

}
