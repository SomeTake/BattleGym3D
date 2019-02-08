//=============================================================================
//
// 入力処理 [input.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************

// プログラム分けするときに使う
#define	USE_KEYBOARD										// 宣言するとキーボードで操作可能になる
#define	USE_MOUSE											// 宣言するとマウスで操作可能になる
#define	USE_PAD												// 宣言するとパッドで操作可能になる

/* game pad情報 */
#define STICK_UP		0x00000001l	// 方向キー上(.IY<0)
#define STICK_DOWN		0x00000002l	// 方向キー下(.IY>0)
#define STICK_LEFT		0x00000004l	// 方向キー左(.IX<0)
#define STICK_RIGHT		0x00000008l	// 方向キー右(.IX>0)

#define BUTTON_A		0x00000010l	// Ａボタン(.rgbButtons[0]&0x80) PS4コン□ボタン
#define BUTTON_B		0x00000020l	// Ｂボタン(.rgbButtons[1]&0x80) PS4コン×ボタン
#define BUTTON_C		0x00000040l	// Ｃボタン(.rgbButtons[2]&0x80) PS4コン△ボタン
#define BUTTON_X		0x00000080l	// Ｘボタン(.rgbButtons[3]&0x80) PS4コン○ボタン
#define BUTTON_Y		0x00000100l	// Ｙボタン(.rgbButtons[4]&0x80) PS4コンL1ボタン
#define BUTTON_Z		0x00000200l	// Ｚボタン(.rgbButtons[5]&0x80) PS4コンR1ボタン
#define BUTTON_L		0x00000400l	// Ｌボタン(.rgbButtons[6]&0x80) PS4コンL2ボタン
#define BUTTON_R		0x00000800l	// Ｒボタン(.rgbButtons[7]&0x80) PS4コンR2ボタン
#define BUTTON_START	0x00001000l	// ＳＴＡＲＴボタン(.rgbButtons[8]&0x80) PS4コンSHAREボタン
#define BUTTON_M		0x00002000l	// Ｍボタン(.rgbButtons[9]&0x80) PS4コンOPTIONSボタン
#define GAMEPADMAX		2			// 同時に接続するジョイパッドの最大数をセット

//十字キー
#define BUTTON_UP		0x00004000l
#define BUTTON_DOWN		(BUTTON_UP<<1)
#define BUTTON_RIGHT	(BUTTON_DOWN<<1)
#define BUTTON_LEFT		(BUTTON_RIGHT<<1)
#define BUTTON_RIGHTUP	(BUTTON_LEFT<<1)
#define BUTTON_RIGHTDOWN	(BUTTON_RIGHTUP<<1)
#define BUTTON_LEFTDOWN	(BUTTON_RIGHTDOWN<<1)
#define BUTTON_LEFTUP	(BUTTON_LEFTDOWN<<1)

#define BUTTON_DEFAULT	(BUTTON_LEFTUP)		// 十字キーデフォルト状態

#define INPUT_END		(BUTTON_DEFAULT<<1)	// 入力終了

#define REPEAT_FRAME	(16)		// ボタンのリピートを有効にするまでのフレーム数
#define REPEAT_TIME		(6)			// リピートが有効になってからのフレーム数

#define INPUT_MAX		(60)		// 入力用配列の最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInst, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

//---------------------------- keyboard
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);

//---------------------------- mouse
BOOL IsMouseLeftPressed(void);      // 左クリックした状態
BOOL IsMouseLeftTriggered(void);    // 左クリックした瞬間
BOOL IsMouseRightPressed(void);     // 右クリックした状態
BOOL IsMouseRightTriggered(void);   // 右クリックした瞬間
BOOL IsMouseCenterPressed(void);    // 中クリックした状態
BOOL IsMouseCenterTriggered(void);  // 中クリックした瞬間
long GetMouseX(void);               // マウスがX方向に動いた相対値
long GetMouseY(void);               // マウスがY方向に動いた相対値
long GetMouseZ(void);               // マウスホイールが動いた相対値

//---------------------------- game pad
BOOL IsButtonPressed(int padNo, DWORD button);
BOOL IsButtonTriggered(int padNo, DWORD button);
BOOL IsButtonReleased(int padNo, DWORD button);
BOOL IsButtonRepeated(int padNo, DWORD button);

int GetPov(int num);

#endif