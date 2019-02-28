//=============================================================================
//
// 会社ロゴ画面表示処理 [blackscreen.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "company.h"
#include "sound.h"
#include "input.h"
#include "player.h"
#include "enemy.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureCompany = NULL;	// テクスチャへのポインタ

static VERTEX_2D				g_vertexWkCompany[NUM_VERTEX];		// 頂点情報格納ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitCompany(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_COMPANY,			// ファイルの名前
			&g_pD3DTextureCompany);	// 読み込むメモリー
	}

	// 頂点情報の作成
	MakeVertexCompany();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCompany(void)
{
	if (g_pD3DTextureCompany != NULL)
	{// テクスチャの開放
		g_pD3DTextureCompany->Release();
		g_pD3DTextureCompany = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCompany(void)
{
	CHARA *playerWk = GetPlayer();
	CHARA *enemyWk = GetEnemy();

	playerWk->ReplayPos = FIRST_PLAYER_POS;
	enemyWk->ReplayPos = FIRST_ENEMY_POS;

	static float per = 0.0f;
	static int frame = 0;

	frame++;

	//透明度の設定
	//徐々に表示
	if (frame < CLEAR_DOWN)
	{
		per += 0.01f;
	}
	//100%表示されたら100フレーム間表示する
	else if (frame >= CLEAR_DOWN && frame < CLEAR_NONE)
	{
		per = 1.00f;
	}
	//徐々に消していく
	else if (frame >= CLEAR_NONE && frame < CLEAR_UP)
	{
		per -= 0.01f;
	}

	if (frame >= CLEAR_UP)
	{
		per = 0.00f;
	}

	//完全に消えたらタイトル画面へ
	if (frame == CLEAR_UP)
	{
		SetPhase(PhaseTitle);
		PlaySound(BGM_TITLE);
	}

	//ボタンでタイトル画面へ
	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_C) || IsButtonTriggered(0, BUTTON_M)
		|| IsButtonTriggered(1, BUTTON_C) || IsButtonTriggered(1, BUTTON_M))
	{
		SetPhase(PhaseTitle);
		PlaySound(BGM_TITLE);
	}

	SetReflectCompany(per);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCompany(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureCompany);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkCompany, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexCompany(void)
{
	// 頂点座標の設定
	g_vertexWkCompany[0].vtx = D3DXVECTOR3(COMPANY_POS_X, COMPANY_POS_Y, 0.0f);
	g_vertexWkCompany[1].vtx = D3DXVECTOR3(COMPANY_POS_X + COMPANY_SIZE_X, COMPANY_POS_Y, 0.0f);
	g_vertexWkCompany[2].vtx = D3DXVECTOR3(COMPANY_POS_X, COMPANY_POS_Y + COMPANY_SIZE_Y, 0.0f);
	g_vertexWkCompany[3].vtx = D3DXVECTOR3(COMPANY_POS_X + COMPANY_SIZE_X, COMPANY_POS_Y + COMPANY_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkCompany[0].rhw =
		g_vertexWkCompany[1].rhw =
		g_vertexWkCompany[2].rhw =
		g_vertexWkCompany[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkCompany[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkCompany[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkCompany[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkCompany[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkCompany[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkCompany[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkCompany[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkCompany[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 反射光の設定 引数:float per = 透明度の％
//=============================================================================
void SetReflectCompany(float per)
{
	int clear = (int)(255 * per);

	g_vertexWkCompany[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkCompany[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkCompany[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkCompany[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
}