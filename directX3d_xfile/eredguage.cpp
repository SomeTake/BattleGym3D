//=============================================================================
//
// 2P用赤ゲージ画面処理 [eredguage.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "eredguage.h"
#include "enemy.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureERedGuage = NULL;		// テクスチャへのポインタ

EREDGUAGE eredguage[EREDGUAGE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitERedGuage(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = GetEnemy(0);

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_EREDGUAGE,				// ファイルの名前
			&g_pD3DTextureERedGuage);		// 読み込むメモリー

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// ゲージの初期化
	eredguage->use = true;
	eredguage->pos = D3DXVECTOR3(EREDGUAGE_POS_X, EREDGUAGE_POS_Y, 0.0f);
	eredguage->value = enemy->HPzan;

	// 頂点情報の作成
	MakeVertexERedGuage();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitERedGuage(void)
{
	if (g_pD3DTextureERedGuage != NULL)
	{// テクスチャの開放
		g_pD3DTextureERedGuage->Release();
		g_pD3DTextureERedGuage = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateERedGuage(void)
{
	if (eredguage->use == true)
	{
		//赤ゲージ
		eredguage->PatternAnim = 2;
		
		//テクスチャ座標をセット
		SetTextureERedGuage(eredguage->PatternAnim);


		SetVertexERedGuage();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawERedGuage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (eredguage->use == true)
	{

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureERedGuage);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, eredguage->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexERedGuage(void)
{

	// 頂点座標の設定
	SetVertexERedGuage();

	// テクスチャのパースペクティブコレクト用
	eredguage->vertexWk[0].rhw =
		eredguage->vertexWk[1].rhw =
		eredguage->vertexWk[2].rhw =
		eredguage->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	eredguage->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	eredguage->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	eredguage->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	eredguage->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	eredguage->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	eredguage->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	eredguage->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	eredguage->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureERedGuage(int cntPattern)
{
	ENEMY *enemy = GetEnemy(0);

	if (eredguage->value > enemy->HPzan)
	{
		eredguage->value -= 5;
	}

	//トレーニングモードなどで回復した時用
	if (enemy->HPzan > eredguage->value)
	{
		eredguage->value = enemy->HPzan;
	}

	int x = cntPattern % EREDGUAGE_PATTERN_DIVIDE_X;
	int y = cntPattern / EREDGUAGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / EREDGUAGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / EREDGUAGE_PATTERN_DIVIDE_Y;

	// テクスチャ座標の設定
	eredguage->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	eredguage->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)eredguage->value / (float)enemy->HP), (float)(y)* sizeY);
	eredguage->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	eredguage->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)eredguage->value / (float)enemy->HP), (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexERedGuage(void)
{
	ENEMY *enemy = GetEnemy(0);

	// 頂点座標の設定
	eredguage->vertexWk[0].vtx = D3DXVECTOR3(eredguage->pos.x, eredguage->pos.y, eredguage->pos.z);
	eredguage->vertexWk[1].vtx = D3DXVECTOR3(eredguage->pos.x + EREDGUAGE_SIZE_X * ((float)eredguage->value / (float)enemy->HP), eredguage->pos.y, eredguage->pos.z);
	eredguage->vertexWk[2].vtx = D3DXVECTOR3(eredguage->pos.x, eredguage->pos.y + EREDGUAGE_SIZE_Y, eredguage->pos.z);
	eredguage->vertexWk[3].vtx = D3DXVECTOR3(eredguage->pos.x + EREDGUAGE_SIZE_X * ((float)eredguage->value / (float)enemy->HP), eredguage->pos.y + EREDGUAGE_SIZE_Y, eredguage->pos.z);
}

EREDGUAGE *GetERedGuage(int eno)
{
	return &eredguage[eno];
}
