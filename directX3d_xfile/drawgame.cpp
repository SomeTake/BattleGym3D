//=============================================================================
//
// 引き分け画面処理 [drawgame.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "drawgame.h"
#include "input.h"
#include "sound.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureDrawgame = NULL;		// テクスチャへのポインタ

VERTEX_2D				g_vertexWkDrawgame[NUM_VERTEX];		// 頂点情報格納ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitDrawgame(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_DRAWGAME,				// ファイルの名前
			&g_pD3DTextureDrawgame);		// 読み込むメモリー

	}

	// 頂点情報の作成
	MakeVertexDrawgame();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitDrawgame(void)
{
	if (g_pD3DTextureDrawgame != NULL)
	{// テクスチャの開放
		g_pD3DTextureDrawgame->Release();
		g_pD3DTextureDrawgame = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateDrawgame(void)
{
	StopSound(BGM_BATTLE, 0);

	for (int ControllerCount = 0; ControllerCount < 2; ControllerCount++)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{// Enter押したら、ステージを切り替える
		 //ゲームループのための再初期化
			ReInit();
			SetPhase(PhaseCountdown);
		}
		// ゲームパッドで移動処理
		else if (IsButtonTriggered(ControllerCount, BUTTON_M))
		{
			//ゲームループのための再初期化
			ReInit();
			SetPhase(PhaseCountdown);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawDrawgame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureDrawgame);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkDrawgame, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexDrawgame(void)
{

	// 頂点座標の設定
	g_vertexWkDrawgame[0].vtx = D3DXVECTOR3(DRAWGAME_POS_X, DRAWGAME_POS_Y, 0.0f);
	g_vertexWkDrawgame[1].vtx = D3DXVECTOR3(DRAWGAME_POS_X + DRAWGAME_SIZE_X, DRAWGAME_POS_Y, 0.0f);
	g_vertexWkDrawgame[2].vtx = D3DXVECTOR3(DRAWGAME_POS_X, DRAWGAME_POS_Y + DRAWGAME_SIZE_Y, 0.0f);
	g_vertexWkDrawgame[3].vtx = D3DXVECTOR3(DRAWGAME_POS_X + DRAWGAME_SIZE_X, DRAWGAME_POS_Y + DRAWGAME_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkDrawgame[0].rhw =
		g_vertexWkDrawgame[1].rhw =
		g_vertexWkDrawgame[2].rhw =
		g_vertexWkDrawgame[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkDrawgame[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkDrawgame[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkDrawgame[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkDrawgame[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkDrawgame[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkDrawgame[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkDrawgame[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkDrawgame[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

