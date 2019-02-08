//=============================================================================
//
// モードセレクト処理 [modeselect.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _MODESELECT_H_
#define _MODESELECT_H_

// マクロ定義
#define TEXTURE_GAME_MODESELECT		_T("data/TEXTURE/select000.png")	// 画像
#define TEXTURE_MODESELECT_SIZE_X	(380)			// テクスチャサイズ
#define TEXTURE_MODESELECT_SIZE_Y	(56)			// 同上
#define TEXTURE_PATTERN_DIVIDE_X_MODESELECT	(1)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_MODESELECT	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_MODESELECT			(TEXTURE_PATTERN_DIVIDE_X_MODESELECT*TEXTURE_PATTERN_DIVIDE_Y_MODESELECT)	// アニメーションパターン数
#define MODESELECT_MAX						(1)		// テクスチャの数
#define TITLESELECT_MAX						(4)		// タイトル画面から選べる選択肢の数
#define PAUSESELECT_MAX						(3)		// ポーズ画面から選べる選択肢の数
#define BUTTON_TIMER						(15)	// ボタンのリピートを有効にするための待ち時間

//*****************************************************************************
// 構造体宣言
//*****************************************************************************

typedef struct	// モードセレクト構造体
{
	D3DXVECTOR3		pos;						// ポリゴンの移動量
	int				PatternAnim;				// アニメーションパターンナンバー
	int				CountAnim;					// アニメーションカウント
	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

} MODESELECT;

// タイトル画面のセレクト番号
enum TitleNum
{
	NextTutorial,
	NextTraining,
	NextGame,
	NextThankyou
};

// ポーズ画面のセレクト番号
enum PauseNum
{
	NextPauseGame,
	NextPauseCountdown,
	NextPauseTitle,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitModeselect(int type);
void UninitModeselect(void);
void UpdateModeselect(void);
void DrawModeselect(void);
MODESELECT *GetModeselect(int no);
HRESULT MakeVertexModeselect(int no);
void SetTextureModeselect(int cntPattern);
void SetVertexModeselect(void);
void SetReflectModeselect(float per);		// 透明度の設定
int GetNextPhase(void);						// 次のフェーズを選択する番号
bool GetTitleSelect(void);					// タイトル画面のセレクトが終了しているかどうか取得

#endif
