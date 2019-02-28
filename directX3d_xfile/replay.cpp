//=============================================================================
//
// リプレイ画面処理 [replay.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "replay.h"
#include "input.h"
#include "sound.h"
#include "player.h"
#include "enemy.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureReplay = NULL;		// テクスチャへのポインタ

VERTEX_2D				g_vertexWkReplay[NUM_VERTEX];		// 頂点情報格納ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitReplay(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_REPLAY,				// ファイルの名前
			&g_pD3DTextureReplay);		// 読み込むメモリー

	}

	// 頂点情報の作成
	MakeVertexReplay();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitReplay(void)
{
	if (g_pD3DTextureReplay != NULL)
	{// テクスチャの開放
		g_pD3DTextureReplay->Release();
		g_pD3DTextureReplay = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateReplay(void)
{
	int Phase = *GetPhase();
	static int ReplayCount = 0;
	CHARA *playerWk = GetPlayer();
	CHARA *enemyWk = GetEnemy();

	// リプレイ画面からタイトル画面へ戻る処理
	if (Phase == PhaseReplay)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			SetPhase(PhaseTitle);
			PlaySound(BGM_TITLE);
			ReplayCount = 0;
		}

		for (int i = 0; i < GAMEPADMAX; i++)
		{
			// PS4コン○ボタンが押されたらタイトル画面へ戻る
			if (IsButtonLockTriggered(i, BUTTON_C))
			{
				SetPhase(PhaseTitle);
				PlaySound(BGM_TITLE);
				ReplayCount = 0;
			}
		}

		ReplayCount++;

		if (ReplayCount == REC_TIME)
		{
			SetPhase(PhaseTitle);
			PlaySound(BGM_TITLE);
			ReplayCount = 0;
		}

		// 透明度の設定
		static int i = 0, reflect = 0;
		i++;
		if (i == FLASHING_TIME)
		{
			reflect == 0 ? reflect = 1 : reflect = 0;
			i = 0;
		}
		SetReflectReplay((float)reflect);

	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawReplay(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureReplay);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkReplay, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexReplay(void)
{

	// 頂点座標の設定
	g_vertexWkReplay[0].vtx = D3DXVECTOR3(REPLAY_POS_X, REPLAY_POS_Y, 0.0f);
	g_vertexWkReplay[1].vtx = D3DXVECTOR3(REPLAY_POS_X + REPLAY_SIZE_X, REPLAY_POS_Y, 0.0f);
	g_vertexWkReplay[2].vtx = D3DXVECTOR3(REPLAY_POS_X, REPLAY_POS_Y + REPLAY_SIZE_Y, 0.0f);
	g_vertexWkReplay[3].vtx = D3DXVECTOR3(REPLAY_POS_X + REPLAY_SIZE_X, REPLAY_POS_Y + REPLAY_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkReplay[0].rhw =
		g_vertexWkReplay[1].rhw =
		g_vertexWkReplay[2].rhw =
		g_vertexWkReplay[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkReplay[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkReplay[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkReplay[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkReplay[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkReplay[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkReplay[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkReplay[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkReplay[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 反射光の設定 引数:int per = 透明度の％
//=============================================================================
void SetReflectReplay(float per)
{
	int clear = (int)(255 * per);

	g_vertexWkReplay[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkReplay[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkReplay[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkReplay[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
}
