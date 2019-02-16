//=============================================================================
//
// リザルト画面処理 [result.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "result.h"
#include "input.h"
#include "sound.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureResult = NULL;		// テクスチャへのポインタ

VERTEX_2D				g_vertexWkResult[NUM_VERTEX];			// 頂点情報格納ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_RESULT,				// ファイルの名前
			&g_pD3DTextureResult);		// 読み込むメモリー

	}

	// 頂点情報の作成
	MakeVertexResult();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	if (g_pD3DTextureResult != NULL)
	{// テクスチャの開放
		g_pD3DTextureResult->Release();
		g_pD3DTextureResult = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	if (GetKeyboardTrigger(DIK_RETURN))
	{// Enter押したら、ステージを切り替える
	 //ゲームループのための再初期化
		ReInit();
		SetPhase(PhaseTitle);
		PlaySound(BGM_TITLE);
	}
	// ゲームパッドで移動処理
	else if (IsButtonTriggered(0, BUTTON_M) || IsButtonTriggered(0, BUTTON_C) || IsButtonTriggered(1, BUTTON_M) || IsButtonTriggered(1, BUTTON_C))
	{
		//ゲームループのための再初期化
		ReInit();
		SetPhase(PhaseTitle);
		PlaySound(BGM_TITLE);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureResult);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkResult, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexResult(void)
{

	// 頂点座標の設定
	g_vertexWkResult[0].vtx = D3DXVECTOR3(RESULT_POS_X, RESULT_POS_Y, 0.0f);
	g_vertexWkResult[1].vtx = D3DXVECTOR3(RESULT_POS_X + RESULT_SIZE_X, RESULT_POS_Y, 0.0f);
	g_vertexWkResult[2].vtx = D3DXVECTOR3(RESULT_POS_X, RESULT_POS_Y + RESULT_SIZE_Y, 0.0f);
	g_vertexWkResult[3].vtx = D3DXVECTOR3(RESULT_POS_X + RESULT_SIZE_X, RESULT_POS_Y + RESULT_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkResult[0].rhw =
		g_vertexWkResult[1].rhw =
		g_vertexWkResult[2].rhw =
		g_vertexWkResult[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkResult[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkResult[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResult[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResult[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResult[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

