//=============================================================================
//
// HPゲージ画面処理 [guage.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "guage.h"
#include "player.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureGuage = NULL;		// テクスチャへのポインタ

GUAGE guage[GUAGE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGuage(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_GUAGE,				// ファイルの名前
			&g_pD3DTextureGuage);		// 読み込むメモリー

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// ゲージの初期化
	guage->use = true;
	guage->pos = D3DXVECTOR3(GUAGE_POS_X, GUAGE_POS_Y, 0.0f);

	// 頂点情報の作成
	MakeVertexGuage();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGuage(void)
{
	if (g_pD3DTextureGuage != NULL)
	{// テクスチャの開放
		g_pD3DTextureGuage->Release();
		g_pD3DTextureGuage = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGuage(void)
{
	PLAYER *player = GetPlayer(0);

	if (guage->use == true)
	{
		//HP満タンの場合
		if (player->HPzan == player->HP)
		{
			guage->PatternAnim = 0;
		}
		//100%未満の場合
		else if (player->HPzan < player->HP)
		{
			guage->PatternAnim = 1;
		}

		//テクスチャ座標をセット
		SetTextureGuage(guage->PatternAnim);

		SetVertexGuage();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGuage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (guage->use == true)
	{

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureGuage);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, guage->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexGuage(void)
{

	// 頂点座標の設定
	SetVertexGuage();

	// テクスチャのパースペクティブコレクト用
	guage->vertexWk[0].rhw =
		guage->vertexWk[1].rhw =
		guage->vertexWk[2].rhw =
		guage->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	guage->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	guage->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	guage->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	guage->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	guage->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	guage->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	guage->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	guage->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureGuage(int cntPattern)
{
	PLAYER *player = GetPlayer(0);

	int x = cntPattern % GUAGE_PATTERN_DIVIDE_X;
	int y = cntPattern / GUAGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / GUAGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / GUAGE_PATTERN_DIVIDE_Y;

	// テクスチャ座標の設定
	guage->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)(player->HP - player->HPzan) / (float)player->HP), (float)(y)* sizeY);
	guage->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	guage->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)(player->HP - player->HPzan) / (float)player->HP), (float)(y)* sizeY + sizeY);
	guage->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexGuage(void)
{
	PLAYER *player = GetPlayer(0);

	// 頂点座標の設定
	guage->vertexWk[0].vtx = D3DXVECTOR3(guage->pos.x + GUAGE_SIZE_X * ((float)(player->HP - player->HPzan) / player->HP), guage->pos.y, guage->pos.z);
	guage->vertexWk[1].vtx = D3DXVECTOR3(guage->pos.x + GUAGE_SIZE_X, guage->pos.y, guage->pos.z);
	guage->vertexWk[2].vtx = D3DXVECTOR3(guage->pos.x + GUAGE_SIZE_X * ((float)(player->HP - player->HPzan) / player->HP), guage->pos.y + GUAGE_SIZE_Y, guage->pos.z);
	guage->vertexWk[3].vtx = D3DXVECTOR3(guage->pos.x + GUAGE_SIZE_X, guage->pos.y + GUAGE_SIZE_Y, guage->pos.z);
}
