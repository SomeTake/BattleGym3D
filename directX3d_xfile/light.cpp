//=============================================================================
//
// ライト処理 [light.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "main.h"
#include "light.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
D3DLIGHT9			Light[NUM_LIGHT];	// ライト情報

//=============================================================================
// ライトの初期化処理
//=============================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;

	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&Light[0], sizeof(D3DLIGHT9));

	// ライト0のタイプの設定
	Light[0].Type = D3DLIGHT_DIRECTIONAL;

	// ライト0の拡散光の設定
	Light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライト0の環境光の設定
	Light[0].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ライト0の方向の設定
	vecDir = D3DXVECTOR3(0.20f, -0.60f, 0.80f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light[0].Direction, &vecDir);

	// ライト0をレンダリングパイプラインに設定
	pDevice->SetLight(0, &Light[0]);

	// ライト0を使用使用状態に
	pDevice->LightEnable(0, TRUE);


	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&Light[1], sizeof(D3DLIGHT9));

	// ライト1のタイプの設定
	Light[1].Type = D3DLIGHT_DIRECTIONAL;

	// ライト1の拡散光の設定
	Light[1].Diffuse = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

	// ライト1の環境光の設定
	Light[1].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ライト1の方向の設定
	vecDir = D3DXVECTOR3(-0.20f, 1.00f, -0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light[1].Direction, &vecDir);

	// ライト1をレンダリングパイプラインに設定
	pDevice->SetLight(1, &Light[1]);

	// ライト1の設定
	pDevice->LightEnable(1, TRUE);

	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&Light[2], sizeof(D3DLIGHT9));

	// ライトのタイプの設定
	Light[2].Type = D3DLIGHT_DIRECTIONAL;

	// 拡散光
	Light[2].Diffuse = D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f);

	// 環境光
	Light[2].Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(0.80f, 0.10f, 0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light[2].Direction, &vecDir);

	// ライトをレンダリングパイプラインに設定
	pDevice->SetLight(2, &Light[2]);

	// ライトの設定
	pDevice->LightEnable(2, TRUE);


	// ライティングモードをON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//=============================================================================
// ライトのゲッター
//=============================================================================
D3DLIGHT9 *GetLight(int no)
{
	return &Light[no];
}