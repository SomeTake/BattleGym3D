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

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexModeselect(int no);
void SetTextureModeselect(int cntPattern);
void SetVertexModeselect(void);
void SetReflectModeselect(float per);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTextureModeselect = NULL;		// テクスチャへのポリゴン

MODESELECT modeselectWk[MODESELECT_MAX];				// 構造体

int titleselect = 0;									// タイトル画面でモードセレクトを行うための変数
int pauseselect = 0;									// ポーズ画面でモードセレクトを行うための変数
int buttoncount = 0;									// 擬似的にボタンリピートを作るためのカウント

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

	//タイトル画面
	if (phase == PhaseTitle)
	{
		//カーソル上下移動
		if (GetKeyboardRepeat(DIK_UP) || IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, BUTTON_UE))
		{
			Play_Sound(SE_SELECT0, 0, 0);
			titleselect = (titleselect == 0) ? TITLESELECT_MAX - 1 : titleselect - 1;
			modeselectWk->pos.y = (float)MODESELECT_POS_Y + (titleselect * TEXTURE_MODESELECT_SIZE_Y);
		}
		else if (GetKeyboardRepeat(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN) || IsButtonTriggered(0, BUTTON_SHITA))
		{
			Play_Sound(SE_SELECT0, 0, 0);
			titleselect = (titleselect == TITLESELECT_MAX - 1) ? 0 : titleselect + 1;
			modeselectWk->pos.y = (float)MODESELECT_POS_Y + (titleselect * TEXTURE_MODESELECT_SIZE_Y);
		}

		//ボタンのリピート操作を擬似的に作成
		if (IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, BUTTON_UE))
		{
			if (buttoncount < BUTTON_TIMER)
			{
				buttoncount++;
			}
			if (buttoncount >= BUTTON_TIMER)
			{
				Play_Sound(SE_SELECT0, 0, 0);
				titleselect = (titleselect == 0) ? TITLESELECT_MAX - 1 : titleselect - 1;
				modeselectWk->pos.y = (float)MODESELECT_POS_Y + (titleselect * TEXTURE_MODESELECT_SIZE_Y);
			}
		}
		else if (IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, BUTTON_SHITA))
		{
			if (buttoncount < BUTTON_TIMER)
			{
				buttoncount++;
			}
			if (buttoncount >= BUTTON_TIMER)
			{
				Play_Sound(SE_SELECT0, 0, 0);
				titleselect = (titleselect == TITLESELECT_MAX - 1) ? 0 : titleselect + 1;
				modeselectWk->pos.y = (float)MODESELECT_POS_Y + (titleselect * TEXTURE_MODESELECT_SIZE_Y);
			}
		}
		if (!IsButtonPressed(0, BUTTON_UP) && !IsButtonPressed(0, BUTTON_UE) && !IsButtonPressed(0, BUTTON_DOWN) && !IsButtonPressed(0, BUTTON_SHITA))
		{
			buttoncount = 0;
		}

		//モードセレクト
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_C))
		{
			if (titleselect == 0)
			{
				Play_Sound(SE_SELECT1, 0, 0);
				ReInit();
				SetPhase(PhaseTutorial);
				Stop_Sound(BGM_TITLE, 0);
				Play_Sound(BGM_TUTORIAL, 1, 1);
			}
			else if (titleselect == 1)
			{
				Play_Sound(SE_SELECT1, 0, 0);
				Stop_Sound(BGM_TITLE, 0);
				Play_Sound(BGM_TRAINING, 1, 1);
				ReInit();
				SetPhase(PhaseTraining);
			}
			else if (titleselect == 2)
			{
				Play_Sound(SE_SELECT1, 0, 0);
				Stop_Sound(BGM_TITLE, 0);
				ReInit();
				SetPhase(PhaseCountdown);
			}
			else if (titleselect == 3)
			{
				Play_Sound(SE_SELECT1, 0, 0);
				SetPhase(PhaseExit);
			}
		}
	}

	//VSモードのポーズ画面
	else if (phase == PhasePause)
	{
		//カーソル上下移動
		if (GetKeyboardRepeat(DIK_UP) || IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, BUTTON_UE))
		{
			Play_Sound(SE_SELECT0, 0, 0);
			pauseselect = (pauseselect == 0) ? PAUSESELECT_MAX - 1 : pauseselect - 1;
			modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
		}
		else if (GetKeyboardRepeat(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN) || IsButtonTriggered(0, BUTTON_SHITA))
		{
			Play_Sound(SE_SELECT0, 0, 0);
			pauseselect = (pauseselect == PAUSESELECT_MAX - 1) ? 0 : pauseselect + 1;
			modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
		}

		//ボタンのリピート操作を擬似的に作成
		if (IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, BUTTON_UE))
		{
			buttoncount++;
			if (buttoncount >= BUTTON_TIMER)
			{
				Play_Sound(SE_SELECT0, 0, 0);
				pauseselect = (pauseselect == 0) ? PAUSESELECT_MAX - 1 : pauseselect - 1;
				modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
			}
		}
		else if (IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, BUTTON_SHITA))
		{
			buttoncount++;
			if (buttoncount >= BUTTON_TIMER)
			{
				Play_Sound(SE_SELECT0, 0, 0);
				pauseselect = (pauseselect == PAUSESELECT_MAX - 1) ? 0 : pauseselect + 1;
				modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
			}
		}
		if (!IsButtonPressed(0, BUTTON_UP) && !IsButtonPressed(0, BUTTON_UE) && !IsButtonPressed(0, BUTTON_DOWN) && !IsButtonPressed(0, BUTTON_SHITA))
		{
			buttoncount = 0;
		}

		//モードセレクト
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_C))
		{
			if (pauseselect == 0)
			{
				Play_Sound(SE_SELECT1, 0, 0);
				SetPhase(PhaseGame);
				Play_Sound(BGM_BATTLE, 1, 1);
			}
			else if (pauseselect == 1)
			{
				Play_Sound(SE_SELECT1, 0, 0);
				ReInit();
				SetPhase(PhaseCountdown);
			}
			else if (pauseselect == 2)
			{
				Play_Sound(SE_SELECT1, 0, 0);
				ReInit();
				SetPhase(PhaseTitle);
				Play_Sound(BGM_TITLE, 1, 1);
			}
		}
		if (IsButtonTriggered(0, BUTTON_M))
		{
			Play_Sound(SE_SELECT1, 0, 0);
			SetPhase(PhaseGame);
			Play_Sound(BGM_BATTLE, 1, 1);
		}
	}

	//トレーニングモードのポーズ画面
	else if (phase == PhaseTrainingPause)
	{
		//カーソル上下移動
		if (GetKeyboardRepeat(DIK_UP) || IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, BUTTON_UE))
		{
			Play_Sound(SE_SELECT0, 0, 0);
			pauseselect = (pauseselect == 0) ? PAUSESELECT_MAX - 1 : pauseselect - 1;
			modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
		}
		else if (GetKeyboardRepeat(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN) || IsButtonTriggered(0, BUTTON_SHITA))
		{
			Play_Sound(SE_SELECT0, 0, 0);
			pauseselect = (pauseselect == PAUSESELECT_MAX - 1) ? 0 : pauseselect + 1;
			modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
		}

		//ボタンのリピート操作を擬似的に作成
		if (IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, BUTTON_UE))
		{
			buttoncount++;
			if (buttoncount >= BUTTON_TIMER)
			{
				Play_Sound(SE_SELECT0, 0, 0);
				pauseselect = (pauseselect == 0) ? PAUSESELECT_MAX - 1 : pauseselect - 1;
				modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
			}
		}
		else if (IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, BUTTON_SHITA))
		{
			buttoncount++;
			if (buttoncount >= BUTTON_TIMER)
			{
				Play_Sound(SE_SELECT0, 0, 0);
				pauseselect = (pauseselect == PAUSESELECT_MAX - 1) ? 0 : pauseselect + 1;
				modeselectWk->pos.y = (float)PAUSESELECT_POS_Y + (pauseselect * TEXTURE_MODESELECT_SIZE_Y);
			}
		}
		if (!IsButtonPressed(0, BUTTON_UP) && !IsButtonPressed(0, BUTTON_UE) && !IsButtonPressed(0, BUTTON_DOWN) && !IsButtonPressed(0, BUTTON_SHITA))
		{
			buttoncount = 0;
		}

		//モードセレクト
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_C))
		{
			if (pauseselect == 0)
			{
				Play_Sound(SE_SELECT1, 0, 0);
				SetPhase(PhaseTraining);
				Play_Sound(BGM_TRAINING, 1, 1);
			}
			else if (pauseselect == 1)
			{
				Play_Sound(SE_SELECT1, 0, 0);
				ReInit();
				SetPhase(PhaseTraining);
				Play_Sound(BGM_TRAINING, 0, 1);
			}
			else if (pauseselect == 2)
			{
				Play_Sound(SE_SELECT1, 0, 0);
				ReInit();
				SetPhase(PhaseTitle);
				Play_Sound(BGM_TITLE, 0, 1);
			}
		}
		if (IsButtonTriggered(0, BUTTON_M))
		{
			Play_Sound(SE_SELECT1, 0, 0);
			SetPhase(PhaseTraining);
			Play_Sound(BGM_TRAINING, 1, 1);
		}
	}

	//透明度の設定
	if (phase == PhasePause || phase == PhaseTrainingPause)
	{
		SetReflectModeselect(TRANSPARENCY);
	}
	if (phase == PhaseTitle)
	{
		SetReflectModeselect(1);
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