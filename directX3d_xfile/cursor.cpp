//=============================================================================
//
// 入力モードセレクト画面処理 [cursor.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "cursor.h"
#include "input.h"
#include "modeselect.h"
#include "player.h"
#include "enemy.h"
#include "sound.h"
#include "debugproc.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
CURSOR	CursorWk[CURSOR_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitCursor(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_CURSOR1P,			// ファイルの名前
			&CursorWk[0].D3DTexture);	// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_CURSOR2P,			// ファイルの名前
			&CursorWk[1].D3DTexture);	// 読み込むメモリー

	}

	for (int i = 0; i < CURSOR_MAX; i++)
	{
		CursorWk[i].pos = D3DXVECTOR3(CURSOR_POS_X_EASY, CURSOR_POS_Y, 0.0f);
		CursorWk[i].modeinput = false;
		// 頂点情報の作成
		MakeVertexCursor(i);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCursor(void)
{
	for (int i = 0; i < CURSOR_MAX; i++)
	{
		if (CursorWk[i].D3DTexture != NULL)
		{// テクスチャの開放
			CursorWk[i].D3DTexture->Release();
			CursorWk[i].D3DTexture = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCursor(void)
{
	CHARA *playerWk = GetPlayer();
	CHARA *enemyWk = GetEnemy();
	int NextPhase = GetNextPhase();

#ifdef _DEBUG
	// デバッグ用
	PrintDebugProc("1Pのモード選択%s\n", CursorWk[0].modeinput ? "完了" : "未完了");
	PrintDebugProc("2Pのモード選択%s\n", CursorWk[1].modeinput ? "完了" : "未完了");
	PrintDebugProc("1Pカーソル位置 X:%f Y:%f\n", CursorWk[0].pos.x, CursorWk[1].pos.y);

#endif

	// トレーニングモードでは2P側自動選択
	if (NextPhase == NextTraining)
	{
		CursorWk[1].modeinput = true;
	}

	int PadCount = GetPadCount();
	// 1P側しかつながっていなかった場合、2P側は自動選択
	if (PadCount <= 1)
	{
		CursorWk[1].modeinput = true;
	}

	// ○を押したら選択終了
	// 1P
	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_C))
	{
		if (CursorWk[0].modeinput == false)
		{
			PlaySound(SE_SELECT1);
			CursorWk[0].modeinput = true;
		}
	}
	// 2P
	if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(1, BUTTON_C))
	{
		if (CursorWk[1].modeinput == false)
		{
			PlaySound(SE_SELECT1);
			CursorWk[1].modeinput = true;
		}
	}

	// バツを押したら選択解除
	if (CursorWk[0].modeinput == true || CursorWk[1].modeinput == true)
	{
		if (GetKeyboardTrigger(DIK_BACK))
		{
			PlaySound(SE_SELECT1);
			CursorWk[0].modeinput = false;
			CursorWk[1].modeinput = false;
		}
	}
	for (int i = 0; i < CURSOR_MAX; i++)
	{
		if (CursorWk[i].modeinput == true)
		{
			if (IsButtonTriggered(i, BUTTON_B))
			{
				CursorWk[i].modeinput = false;
			}
		}
	}

	// 両方入力が終わったらシーン遷移
	if (CursorWk[0].modeinput == true && CursorWk[1].modeinput == true)
	{
		StopSound(BGM_TITLE);
		// カーソル位置によって入力モードを決定
		// 1P
		if (CursorWk[0].pos.x == CURSOR_POS_X_COMMAND)
		{
			playerWk->CommandInput = true;
		}
		else
		{
			playerWk->CommandInput = false;
		}
		// 2P
		if (CursorWk[1].pos.x == CURSOR_POS_X_COMMAND)
		{
			enemyWk->CommandInput = true;
		}
		else
		{
			enemyWk->CommandInput = false;
		}
		ReInit();

		switch (NextPhase)
		{
		case NextTutorial:
			SetPhase(PhaseTutorial);
			PlaySound(BGM_TUTORIAL);
			break;
		case NextTraining:
			SetPhase(PhaseCountdown);
			break;
		case NextGame:
			SetPhase(PhaseCountdown);
			break;
		default:
			break;
		}
	}

	// 左右（キーボード1P:左右キー 2P:ADキー）入力で入力モードの選択&カーソル移動
	// 1P側
	if (CursorWk[0].modeinput == false)
	{
		if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_RIGHT))
		{
			PlaySound(SE_SELECT0);
			CursorWk[0].pos = D3DXVECTOR3(CURSOR_POS_X_COMMAND, CURSOR_POS_Y, 0.0f);
		}
		else if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_LEFT))
		{
			PlaySound(SE_SELECT0);
			CursorWk[0].pos = D3DXVECTOR3(CURSOR_POS_X_EASY, CURSOR_POS_Y, 0.0f);
		}
	}

	// 2P側
	if (CursorWk[1].modeinput == false)
	{
		if (GetKeyboardTrigger(DIK_D) || IsButtonTriggered(1, BUTTON_RIGHT))
		{
			PlaySound(SE_SELECT0);
			CursorWk[1].pos = D3DXVECTOR3(CURSOR_POS_X_COMMAND, CURSOR_POS_Y, 0.0f);
		}
		else if (GetKeyboardTrigger(DIK_A) || IsButtonTriggered(1, BUTTON_LEFT))
		{
			PlaySound(SE_SELECT0);
			CursorWk[1].pos = D3DXVECTOR3(CURSOR_POS_X_EASY, CURSOR_POS_Y, 0.0f);
		}
	}

	// 頂点座標の更新
	for (int i = 0; i < CURSOR_MAX; i++)
	{
		SetVertexCursor(i);
	}

	float per;
	// 透明度の設定
	for (int i = 0; i < CURSOR_MAX; i++)
	{
		if (CursorWk[i].modeinput == true)
		{
			per = 1.0f;
		}
		else
		{
			per = 0.5f;
		}
			SetReflectCursor(i, per);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCursor(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int NextPhase = GetNextPhase();
	int PadCount = GetPadCount();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// トレーニングモードとコントローラ接続していないチュートリアルモードでは2P側は選択しない
	if (NextPhase != NextTraining && PadCount == GAMEPADMAX)
	{
		// 2P側
		// テクスチャの設定
		pDevice->SetTexture(0, CursorWk[1].D3DTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, CursorWk[1].vertexWk, sizeof(VERTEX_2D));
	}

	// 1P側
	// テクスチャの設定
	pDevice->SetTexture(0, CursorWk[0].D3DTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, CursorWk[0].vertexWk, sizeof(VERTEX_2D));

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexCursor(int num)
{
	// 頂点座標の設定
	SetVertexCursor(num);

	// テクスチャのパースペクティブコレクト用
	CursorWk[num].vertexWk[0].rhw =
		CursorWk[num].vertexWk[1].rhw =
		CursorWk[num].vertexWk[2].rhw =
		CursorWk[num].vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	CursorWk[num].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	CursorWk[num].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	CursorWk[num].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	CursorWk[num].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	CursorWk[num].vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	CursorWk[num].vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	CursorWk[num].vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	CursorWk[num].vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexCursor(int num)
{
	CursorWk[num].vertexWk[0].vtx = D3DXVECTOR3(CursorWk[num].pos.x, CursorWk[num].pos.y, CursorWk[num].pos.z);
	CursorWk[num].vertexWk[1].vtx = D3DXVECTOR3(CursorWk[num].pos.x + CURSOR_SIZE_X, CursorWk[num].pos.y, CursorWk[num].pos.z);
	CursorWk[num].vertexWk[2].vtx = D3DXVECTOR3(CursorWk[num].pos.x, CursorWk[num].pos.y + CURSOR_SIZE_Y, CursorWk[num].pos.z);
	CursorWk[num].vertexWk[3].vtx = D3DXVECTOR3(CursorWk[num].pos.x + CURSOR_SIZE_X, CursorWk[num].pos.y + CURSOR_SIZE_Y, CursorWk[num].pos.z);
}

//=============================================================================
// 反射光の設定 引数:int per = 透明度の％
//=============================================================================
void SetReflectCursor(int num, float per)
{
	int clear = (int)(255 * per);

	CursorWk[num].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	CursorWk[num].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	CursorWk[num].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	CursorWk[num].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
}

//=============================================================================
// カーソル構造体のゲッター
//=============================================================================
CURSOR *GetCursor(int num)
{
	return &CursorWk[num];
}
