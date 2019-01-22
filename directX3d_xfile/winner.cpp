//=============================================================================
//
// ウィナー表示処理 [winner.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "winner.h"
#include "player.h"
#include "enemy.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureWinner = NULL;		// テクスチャへのポインタ

static VERTEX_2D				g_vertexWkWinner[NUM_VERTEX];			// 頂点情報格納ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitWinner(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_WINNER,				// ファイルの名前
			&g_pD3DTextureWinner);		// 読み込むメモリー

	}

	// 頂点情報の作成
	MakeVertexWinner();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitWinner(void)
{
	if (g_pD3DTextureWinner != NULL)
	{// テクスチャの開放
		g_pD3DTextureWinner->Release();
		g_pD3DTextureWinner = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateWinner(void)
{
	CHARA *player = GetPlayer();
	CHARA *enemy = GetEnemy();

	if (player->HPzan > enemy->HPzan)
	{
		SetTextureWinner(0);
	}
	else if (player->HPzan < enemy->HPzan)
	{
		SetTextureWinner(1);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawWinner(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureWinner);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkWinner, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexWinner(void)
{

	// 頂点座標の設定
	SetVertexWinner();

	// テクスチャのパースペクティブコレクト用
	g_vertexWkWinner[0].rhw =
		g_vertexWkWinner[1].rhw =
		g_vertexWkWinner[2].rhw =
		g_vertexWkWinner[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkWinner[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkWinner[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkWinner[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkWinner[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkWinner[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkWinner[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkWinner[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkWinner[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureWinner(int cntPattern)
{
	int x = cntPattern % TEXTURE_PATTERN_WINNER_X;
	int y = cntPattern / TEXTURE_PATTERN_WINNER_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_WINNER_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_WINNER_Y;

	// テクスチャ座標の設定
	g_vertexWkWinner[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	g_vertexWkWinner[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	g_vertexWkWinner[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	g_vertexWkWinner[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexWinner(void)
{
	g_vertexWkWinner[0].vtx = D3DXVECTOR3(WINNER_POS_X, WINNER_POS_Y, 0.0f);
	g_vertexWkWinner[1].vtx = D3DXVECTOR3(WINNER_POS_X + WINNER_SIZE_X, WINNER_POS_Y, 0.0f);
	g_vertexWkWinner[2].vtx = D3DXVECTOR3(WINNER_POS_X, WINNER_POS_Y + WINNER_SIZE_Y, 0.0f);
	g_vertexWkWinner[3].vtx = D3DXVECTOR3(WINNER_POS_X + WINNER_SIZE_X, WINNER_POS_Y + WINNER_SIZE_Y, 0.0f);
}