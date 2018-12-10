//=============================================================================
//
// 会社ロゴ画面表示処理 [company.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _COMPANY_H_
#define _COMPANY_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_COMPANY	_T("data/TEXTURE/company000.png")			// 読み込むテクスチャファイル名
#define	COMPANY_SIZE_X	(500)										// 会社ロゴの幅
#define	COMPANY_SIZE_Y	(500)										// 会社ロゴの高さ
#define	COMPANY_POS_X	(SCREEN_WIDTH / 2 - COMPANY_SIZE_X / 2)		// 会社ロゴの表示位置
#define	COMPANY_POS_Y	(SCREEN_HEIGHT / 2 - COMPANY_SIZE_Y / 2)	// 会社ロゴの表示位置

#define CLEAR_DOWN		(100)										// 透明度を下げる時間
#define CLEAR_NONE		(200)										// 100%表示されている時間
#define CLEAR_UP		(300)										// 透明度を上げる時間

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitCompany(int type);
void UninitCompany(void);
void UpdateCompany(void);
void DrawCompany(void);

#endif
