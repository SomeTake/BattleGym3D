//=============================================================================
//
// モードセレクト処理 [modeselect.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "modeselect.h"
#include "input.h"
#include "sound.h"
#include "cursor.h"
#include "player.h"
#include "enemy.h"
#include "replay.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTextureModeselect = NULL;		// テクスチャへのポリゴン

MODESELECT modeselectWk[MODESELECT_MAX];				// 構造体

int titleselect = 0;									// タイトル画面でモードセレクトを行うための変数
int pauseselect = 0;									// ポーズ画面でモードセレクトを行うための変数
int buttoncount = 0;									// 擬似的にボタンリピートを作るためのカウント
int nextphase;											// カウントダウンフェーズが終了後をトレーニングモードにするかVSモードにするか
bool selectok;											// タイトル画面のモードセレクトが終了しているかどうか

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitModeselect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MODESELECT *modeselect = modeselectWk;

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
			TEXTURE_GAME_MODESELECT,				// ファイルの名前
			&g_pD3DTextureModeselect);			// 読み込むメモリのポインタ
	}

	titleselect = 0;
	pauseselect = 0;
	selectok = false;

	// 初期化処理
	for (int i = 0; i < MODESELECT_MAX; i++, modeselect++)
	{
		modeselectWk->pos = D3DXVECTOR3(MODESELECT_POS_X, MODESELECT_POS_Y, 0.0f);	// 座標データを初期化
		modeselectWk->PatternAnim = 0;									// アニメパターン番号をランダムで初期化
		modeselectWk->CountAnim = 0;									// アニメカウントを初期化

		modeselectWk->Texture = g_pD3DTextureModeselect;				// テクスチャ情報
		MakeVertexModeselect(i);										// 頂点情報の作成
	}


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitModeselect(void)
{
	if (g_pD3DTextureModeselect != NULL)
	{	// テクスチャの開放
		g_pD3DTextureModeselect->Release();
		g_pD3DTextureModeselect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateModeselect(void)
{
	int phase = *GetPhase();
	CURSOR *CursorWk = GetCursor(0);
	CHARA *playerWk = GetPlayer();
	CHARA *enemyWk = GetEnemy();

	static int TitleCount = 0;

	switch (phase)
	{
	case PhaseTitle:
		if (selectok == false)
		{
			TitleCount++;

			// リプレイ画面へ
			if (TitleCount == TITLE_TIME)
			{
				TitleCount = 0;
				SetPhase(PhaseReplay);
				StopSound(BGM_TITLE);

				// 位置を合わせる
				playerWk->pos = playerWk->ReplayPos;
				enemyWk->pos = enemyWk->ReplayPos;
			}

			//カーソル上下移動
			if (GetKeyboardRepeat(DIK_UP) || IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, STICK_UP)
				|| IsButtonTriggered(1, BUTTON_UP) || IsButtonTriggered(1, STICK_UP))
			{
				PlaySound(SE_SELECT0);
				titleselect = (titleselect == 0) ? TITLESELECT_MAX - 1 : titleselect - 1;
				modeselectWk->pos.y = (float)MODESELECT_POS_Y + (titleselect * TEXTURE_MODESELECT_SIZE_Y);
			}
			else if (GetKeyboardRepeat(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN) || IsButtonTriggered(0, STICK_DOWN)
				|| IsButtonTriggered(1, BUTTON_DOWN) || IsButtonTriggered(1, STICK_DOWN))
			{
				PlaySound(SE_SELECT0);
				titleselect = (titleselect == TITLESELECT_MAX - 1) ? 0 : titleselect + 1;
				modeselectWk->pos.y = (float)MODESELECT_POS_Y + (titleselect * TEXTURE_MODESELECT_SIZE_Y);
			}

			//ボタンのリピート操作を擬似的に作成
			if (IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, STICK_UP)
				|| IsButtonPressed(1, BUTTON_UP) || IsButtonPressed(1, STICK_UP))
			{
				if (buttoncount < BUTTON_TIMER)
				{
					buttoncount++;
				}
				if (buttoncount >= BUTTON_TIMER)
				{
					PlaySound(SE_SELECT0);
					titleselect = (titleselect == 0) ? TITLESELECT_MAX - 1 : titleselect - 1;
					modeselectWk->pos.y = (float)MODESELECT_POS_Y + (titleselect * TEXTURE_MODESELECT_SIZE_Y);
				}
			}
			else if (IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, STICK_DOWN)
				|| IsButtonPressed(1, BUTTON_DOWN) || IsButtonPressed(1, STICK_DOWN))
			{
				if (buttoncount < BUTTON_TIMER)
				{
					buttoncount++;
				}
				if (buttoncount >= BUTTON_TIMER)
				{
					PlaySound(SE_SELECT0);
					titleselect = (titleselect == TITLESELECT_MAX - 1) ? 0 : titleselect + 1;
					modeselectWk->pos.y = (float)MODESELECT_POS_Y + (titleselect * TEXTURE_MODESELECT_SIZE_Y);
				}
			}
			if (!IsButtonPressed(0, BUTTON_UP) && !IsButtonPressed(0, STICK_UP) && !IsButtonPressed(0, BUTTON_DOWN) && !IsButtonPressed(0, STICK_DOWN)
				&& !IsButtonPressed(1, BUTTON_UP) && !IsButtonPressed(1, STICK_UP) && !IsButtonPressed(1, BUTTON_DOWN) && !IsButtonPressed(1, STICK_DOWN))
			{
				buttoncount = 0;
			}

			//モードセレクト
			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_C) || IsButtonTriggered(1, BUTTON_C))
			{
				TitleCount = 0;
				PlaySound(SE_SELECT1);

				switch (titleselect)
				{
				case NextTutorial:
					nextphase = NextTutorial;
					selectok = true;
					break;
				case NextTraining:
					nextphase = NextTraining;
					selectok = true;
					break;
				case NextGame:
					nextphase = NextGame;
					selectok = true;
					break;
				case NextThankyou:
					SetPhase(PhaseThankyou);
					break;
				default:
					break;
				}
			}
		}
		else if (selectok == true && (CursorWk[0].modeinput == false && CursorWk[1].modeinput == false))
		{
			// 前画面に戻す
			if (GetKeyboardTrigger(DIK_BACK))
			{
				PlaySound(SE_SELECT1);
				selectok = false;
			}
		}
		else if (selectok == true && CursorWk[0].modeinput == false)
		{
			if (IsButtonTriggered(0, BUTTON_B))
			{
				PlaySound(SE_SELECT1);
				selectok = false;
			}
		}
		else if (selectok == true && CursorWk[1].modeinput == false)
		{
			if (IsButtonTriggered(1, BUTTON_B))
			{
				PlaySound(SE_SELECT1);
				selectok = false;
			}
		}
		break;
	case PhaseTrainingPause:
		//カーソル上下移動
		if (GetKeyboardRepeat(DIK_UP) || IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, STICK_UP)
			|| IsButtonTriggered(1, BUTTON_UP) || IsButtonTriggered(1, STICK_UP))
		{
			PlaySound(SE_SELECT0);
			pauseselect = (pauseselect == 0) ? PAUSESELECT_MAX - 1 : pauseselect - 1;
			modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
		}
		else if (GetKeyboardRepeat(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN) || IsButtonTriggered(0, STICK_DOWN)
			|| IsButtonTriggered(1, BUTTON_DOWN) || IsButtonTriggered(1, STICK_DOWN))
		{
			PlaySound(SE_SELECT0);
			pauseselect = (pauseselect == PAUSESELECT_MAX - 1) ? 0 : pauseselect + 1;
			modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
		}

		//ボタンのリピート操作を擬似的に作成
		if (IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, STICK_UP)
			|| IsButtonPressed(1, BUTTON_UP) || IsButtonPressed(1, STICK_UP))
		{
			buttoncount++;
			if (buttoncount >= BUTTON_TIMER)
			{
				PlaySound(SE_SELECT0);
				pauseselect = (pauseselect == 0) ? PAUSESELECT_MAX - 1 : pauseselect - 1;
				modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
			}
		}
		else if (IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, STICK_DOWN)
			|| IsButtonPressed(1, BUTTON_DOWN) || IsButtonPressed(1, STICK_DOWN))
		{
			buttoncount++;
			if (buttoncount >= BUTTON_TIMER)
			{
				PlaySound(SE_SELECT0);
				pauseselect = (pauseselect == PAUSESELECT_MAX - 1) ? 0 : pauseselect + 1;
				modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
			}
		}
		if (!IsButtonPressed(0, BUTTON_UP) && !IsButtonPressed(0, STICK_UP) && !IsButtonPressed(0, BUTTON_DOWN) && !IsButtonPressed(0, STICK_DOWN)
			&& !IsButtonPressed(1, BUTTON_UP) && !IsButtonPressed(1, STICK_UP) && !IsButtonPressed(1, BUTTON_DOWN) && !IsButtonPressed(1, STICK_DOWN))
		{
			buttoncount = 0;
		}

		//モードセレクト
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_C) || IsButtonTriggered(1, BUTTON_C))
		{
			switch (pauseselect)
			{
			case NextPauseGame:
				PlaySound(SE_SELECT1);
				SetPhase(PhaseTraining);
				PlaySound(BGM_TRAINING);
				break;
			case NextPauseCountdown:
				PlaySound(SE_SELECT1);
				ReInit();
				SetPhase(PhaseCountdown);
				PlaySound(BGM_TRAINING);
				nextphase = NextTraining;
				break;
			case NextPauseTitle:
				PlaySound(SE_SELECT1);
				ReInit();
				SetPhase(PhaseTitle);
				PlaySound(BGM_TITLE);
				break;
			default:
				break;
			}
		}

		// ゲーム画面に戻る
		if (IsButtonTriggered(0, BUTTON_M) || IsButtonTriggered(1, BUTTON_M))
		{
			PlaySound(SE_SELECT1);
			SetPhase(PhaseTraining);
			PlaySound(BGM_TRAINING);
		}
		break;
	case PhasePause:
		//カーソル上下移動
		if (GetKeyboardRepeat(DIK_UP) || IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, STICK_UP)
			|| IsButtonTriggered(1, BUTTON_UP) || IsButtonTriggered(1, STICK_UP))
		{
			PlaySound(SE_SELECT0);
			pauseselect = (pauseselect == 0) ? PAUSESELECT_MAX - 1 : pauseselect - 1;
			modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
		}
		else if (GetKeyboardRepeat(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN) || IsButtonTriggered(0, STICK_DOWN)
			|| IsButtonTriggered(1, BUTTON_DOWN) || IsButtonTriggered(1, STICK_DOWN))
		{
			PlaySound(SE_SELECT0);
			pauseselect = (pauseselect == PAUSESELECT_MAX - 1) ? 0 : pauseselect + 1;
			modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
		}

		//ボタンのリピート操作を擬似的に作成
		if (IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, STICK_UP)
			|| IsButtonPressed(1, BUTTON_UP) || IsButtonPressed(1, STICK_UP))
		{
			buttoncount++;
			if (buttoncount >= BUTTON_TIMER)
			{
				PlaySound(SE_SELECT0);
				pauseselect = (pauseselect == 0) ? PAUSESELECT_MAX - 1 : pauseselect - 1;
				modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
			}
		}
		else if (IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, STICK_DOWN)
			|| IsButtonPressed(1, BUTTON_DOWN) || IsButtonPressed(1, STICK_DOWN))
		{
			buttoncount++;
			if (buttoncount >= BUTTON_TIMER)
			{
				PlaySound(SE_SELECT0);
				pauseselect = (pauseselect == PAUSESELECT_MAX - 1) ? 0 : pauseselect + 1;
				modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
			}
		}
		if (!IsButtonPressed(0, BUTTON_UP) && !IsButtonPressed(0, STICK_UP) && !IsButtonPressed(0, BUTTON_DOWN) && !IsButtonPressed(0, STICK_DOWN)
			&& !IsButtonPressed(1, BUTTON_UP) && !IsButtonPressed(1, STICK_UP) && !IsButtonPressed(1, BUTTON_DOWN) && !IsButtonPressed(1, STICK_DOWN))
		{
			buttoncount = 0;
		}

		//モードセレクト
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_C) || IsButtonTriggered(0, BUTTON_C))
		{
			switch (pauseselect)
			{
			case NextPauseGame:
				PlaySound(SE_SELECT1);
				SetPhase(PhaseGame);
				PlaySound(BGM_BATTLE);
				break;
			case NextPauseCountdown:
				PlaySound(SE_SELECT1);
				ReInit();
				SetPhase(PhaseCountdown);
				nextphase = NextGame;
				break;
			case NextPauseTitle:
				PlaySound(SE_SELECT1);
				ReInit();
				SetPhase(PhaseTitle);
				PlaySound(BGM_TITLE);
				break;
			default:
				break;
			}
		}

		// ゲーム画面に戻る
		if (IsButtonTriggered(0, BUTTON_M) || IsButtonTriggered(1, BUTTON_M))
		{
			PlaySound(SE_SELECT1);
			SetPhase(PhaseGame);
			PlaySound(BGM_BATTLE);
		}
		break;
	default:
		break;
	}

	//透明度の設定
	if (phase == PhasePause || phase == PhaseTrainingPause)
	{
		SetReflectModeselect(TRANSPARENCY);
	}
	if (phase == PhaseTitle)
	{
		static int i = 0, reflect = 0;
		i++;
		if (i == FLASHING_TIME)
		{
			reflect == 0 ? reflect = 1 : reflect = 0;
			i = 0;
		}
		SetReflectModeselect((float)reflect);
	}

	SetTextureModeselect(0);

	SetVertexModeselect();	// 移動後の座標で頂点を設定

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawModeselect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MODESELECT *modeselect = modeselectWk;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MODESELECT_MAX; i++, modeselect++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, modeselectWk->Texture);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, modeselectWk->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexModeselect(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertexModeselect();

	// rhwの設定
	modeselectWk->vertexWk[0].rhw =
		modeselectWk->vertexWk[1].rhw =
		modeselectWk->vertexWk[2].rhw =
		modeselectWk->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	modeselectWk->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	modeselectWk->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	modeselectWk->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	modeselectWk->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);

	// テクスチャ座標の設定
	modeselectWk->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	modeselectWk->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_MODESELECT, 0.0f);
	modeselectWk->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MODESELECT);
	modeselectWk->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_MODESELECT, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MODESELECT);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureModeselect(int cntPattern)
{
	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_MODESELECT;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_MODESELECT;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_MODESELECT;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MODESELECT;

	modeselectWk->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	modeselectWk->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	modeselectWk->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	modeselectWk->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexModeselect(void)
{
	// 頂点座標の設定
	modeselectWk->vertexWk[0].vtx = D3DXVECTOR3(modeselectWk->pos.x, modeselectWk->pos.y, modeselectWk->pos.z);
	modeselectWk->vertexWk[1].vtx = D3DXVECTOR3(modeselectWk->pos.x + TEXTURE_MODESELECT_SIZE_X, modeselectWk->pos.y, modeselectWk->pos.z);
	modeselectWk->vertexWk[2].vtx = D3DXVECTOR3(modeselectWk->pos.x, modeselectWk->pos.y + TEXTURE_MODESELECT_SIZE_Y, modeselectWk->pos.z);
	modeselectWk->vertexWk[3].vtx = D3DXVECTOR3(modeselectWk->pos.x + TEXTURE_MODESELECT_SIZE_X, modeselectWk->pos.y + TEXTURE_MODESELECT_SIZE_Y, modeselectWk->pos.z);
}

//=============================================================================
// 反射光の設定 引数:int per = 透明度の％
//=============================================================================
void SetReflectModeselect(float per)
{
	int clear = (int)(255 * per);

	modeselectWk->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	modeselectWk->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	modeselectWk->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	modeselectWk->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
}

//=============================================================================
// モードセレクト取得関数
//=============================================================================
MODESELECT *GetModeselect(int no)
{
	return(&modeselectWk[no]);
}

//=============================================================================
// 次のフェーズを取得するフラグ
//=============================================================================
int GetNextPhase(void)
{
	return nextphase;
}

//=============================================================================
// タイトル画面のセレクトが終了しているかどうかを取得するフラグ
//=============================================================================
bool GetTitleSelect(void)
{
	return selectok;
}