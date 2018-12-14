//=============================================================================
//
// リザルト画面のスター表示処理 [resultstar.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "resultstar.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureResultstar = NULL;		// テクスチャへのポリゴン

RESULTSTAR resultstar[STAR_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResultstar(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			STAR00,				// ファイルの名前
			&g_pD3DTextureResultstar);				// 読み込むメモリのポインタ

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// リザルトスターの初期化
	resultstar->use = true;
	resultstar->pos = D3DXVECTOR3(STAR_SIZE_X / 2, STAR_SIZE_Y / 2, 0.0f);
	resultstar->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	resultstar->angle = atan2f(resultstar->pos.y - 0.0f, resultstar->pos.x - 0.0f);
	D3DXVECTOR2 temp = D3DXVECTOR2(STAR_SIZE_X / 2, STAR_SIZE_Y / 2);
	resultstar->radius = D3DXVec2Length(&temp);
	resultstar->CountAnim = 0;
	resultstar->PatternAnim = 0;

	// 頂点情報の作成
	MakeVertexResultstar();

	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResultstar(void)
{
	if (g_pD3DTextureResultstar != NULL)
	{	// テクスチャの開放
		g_pD3DTextureResultstar->Release();
		g_pD3DTextureResultstar = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResultstar(void)
{
	if (resultstar->use == true)
	{
		// アニメーション
		resultstar->CountAnim++;

		//アニメーションwaitチェック
		if ((resultstar->CountAnim % TIME_ANIMATION_STAR) == 0)
		{
			//パターンの切り替え
			resultstar->PatternAnim = 1;

			//テクスチャ座標をセット
			SetTextureResultstar(resultstar->PatternAnim);
		}

		//回転処理
		resultstar->rot.z += 0.01f;

	}
	SetVertexResultstar();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResultstar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (resultstar->use == true)
	{
		// テクスチャの設定(ポリゴンの描画前に読み込んだテクスチャのセットを行う)
		// テクスチャのセットをしないと前にセットされたテクスチャが貼られる→何もはらないことを指定するpDevide->SetTexture(0, NULL);
		pDevice->SetTexture(0, g_pD3DTextureResultstar);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, resultstar->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexResultstar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	resultstar->vertexWk[0].vtx = D3DXVECTOR3(resultstar->pos.x, resultstar->pos.y, resultstar->pos.z);
	resultstar->vertexWk[1].vtx = D3DXVECTOR3(resultstar->pos.x + STAR_SIZE_X, resultstar->pos.y, resultstar->pos.z);
	resultstar->vertexWk[2].vtx = D3DXVECTOR3(resultstar->pos.x, resultstar->pos.y + STAR_SIZE_Y, resultstar->pos.z);
	resultstar->vertexWk[3].vtx = D3DXVECTOR3(resultstar->pos.x + STAR_SIZE_X, resultstar->pos.y + STAR_SIZE_Y, resultstar->pos.z);

	// rhwの設定
	resultstar->vertexWk[0].rhw =
		resultstar->vertexWk[1].rhw =
		resultstar->vertexWk[2].rhw =
		resultstar->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	resultstar->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	resultstar->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	resultstar->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	resultstar->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);

	// テクスチャ座標の設定
	resultstar->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	resultstar->vertexWk[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	resultstar->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	resultstar->vertexWk[3].tex = D3DXVECTOR2(0.125f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureResultstar(int cntPattern)
{
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_STAR_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_STAR_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_STAR_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_STAR_Y;

	// テクスチャ座標の設定
	resultstar->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	resultstar->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	resultstar->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	resultstar->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexResultstar(void)
{
	// 頂点座標の設定
	resultstar->vertexWk[0].vtx.x = resultstar->pos.x - cosf(resultstar->angle + resultstar->rot.z) * resultstar->radius + STAR_POS_X;
	resultstar->vertexWk[0].vtx.y = resultstar->pos.y - sinf(resultstar->angle + resultstar->rot.z) * resultstar->radius + STAR_POS_Y;

	resultstar->vertexWk[1].vtx.x = resultstar->pos.x + cosf(resultstar->angle - resultstar->rot.z) * resultstar->radius + STAR_POS_X;
	resultstar->vertexWk[1].vtx.y = resultstar->pos.y - sinf(resultstar->angle - resultstar->rot.z) * resultstar->radius + STAR_POS_Y;

	resultstar->vertexWk[2].vtx.x = resultstar->pos.x - cosf(resultstar->angle - resultstar->rot.z) * resultstar->radius + STAR_POS_X;
	resultstar->vertexWk[2].vtx.y = resultstar->pos.y + sinf(resultstar->angle - resultstar->rot.z) * resultstar->radius + STAR_POS_Y;

	resultstar->vertexWk[3].vtx.x = resultstar->pos.x + cosf(resultstar->angle + resultstar->rot.z) * resultstar->radius + STAR_POS_X;
	resultstar->vertexWk[3].vtx.y = resultstar->pos.y + sinf(resultstar->angle + resultstar->rot.z) * resultstar->radius + STAR_POS_Y;

}