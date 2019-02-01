//=============================================================================
//
// メイン処理 [main.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "meshfield.h"
#include "camera.h"
#include "light.h"
#include "shadow.h"
#include "enemy.h"
#include "debugproc.h"
#include "particle.h"
#include "score.h"
#include "Icon.h"
#include "blackscreen.h"
#include "company.h"
#include "countdown.h"
#include "drawgame.h"
#include "egauge.h"
#include "eredgauge.h"
#include "escore.h"
#include "espgauge.h"
#include "evaluation.h"
#include "frame.h"
#include "framespgauge.h"
#include "gauge.h"
#include "knockout.h"
#include "modeselect.h"
#include "pause.h"
#include "redgauge.h"
#include "result.h"
#include "resultstar.h"
#include "spgauge.h"
#include "spmax.h"
#include "timer.h"
#include "title.h"
#include "tutorial.h"
#include "winner.h"
#include "sound.h"
#include "meshwall.h"
#include "thanks.h"
#include "skybox.h"
#include "ball.h"
#include "battle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// ウインドウのクラス名
#define WINDOW_NAME		"BattleGym3D"		// ウインドウのキャプション名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(int no);
#ifdef _DEBUG
void DrawFPS(void);
#endif
bool SetWindowCenter(HWND hWnd);
bool RenderWireframe = false;						// ワイヤーフレームで描画

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;					// Direct3D オブジェクト
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;			// Deviceオブジェクト(描画に必要)

int ePhase = PhaseCompanyLogo;						// ゲームの開始位置&シーン遷移
int PlayerMode = 1;									// プレイヤー人数

MATRIX MatrixState;									// マトリクス

#ifdef _DEBUG
static LPD3DXFONT	g_pD3DXFont = NULL;				// フォントへのポインタ
int					g_nCountFPS;					// FPSカウンタ
#endif

													// 画面分割
struct MY_VERTEX {
	float px, py, pz;
	DWORD color;
};

// 分割サイズ指定
// 4分割
D3DVIEWPORT9 g_port4[] = { { 0, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,0.0f,1.0f },
{ SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,0.0f,1.0f },
{ 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,0.0f,1.0f },
{ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,0.0f,1.0f }
};

// 横２分割
D3DVIEWPORT9 g_port2[] = { { 0, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT,0.0f,1.0f },
{ SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT,0.0f,1.0f }
};

// そのまま
D3DVIEWPORT9 g_port1[] = { { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,0.0f,1.0f }
};

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand((unsigned)time(NULL));

	// 警告
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	DWORD dwExecLastTime;							// 最終実行時間
	DWORD dwFPSLastTime;							// 現在フレーム
	DWORD dwCurrentTime;							// 現在時刻
	DWORD dwFrameCount;								// フレームカウント

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),							// 構造体のサイズ
		CS_CLASSDC,									// ウインドウスタイル：CS_HREDRAW / CS_VREDRAW
		WndProc,									// ウインドウプロシージャのアドレス
		0,											// 予備メモリ
		0,											// ウインドウオブジェクト作成時に確保されるメモリサイズ
		hInstance,									// インスタンスハンドル
		LoadIcon(hInstance, (LPCTSTR)IDI_ICON1),	// アプリのショートカットなどで使用（アイコンの設定）
		LoadCursor(NULL, IDC_ARROW),				// ウインドウのクライアント上のマウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),					// ウインドウのクライアント領域の背景色
		NULL,										// メニュー名
		CLASS_NAME,									// ウインドウクラスの名前
		LoadIcon(hInstance, (LPCTSTR)IDI_ICON2)		// ウインドウのアイコン
	};
	HWND hWnd;										// ウインドウハンドル
	MSG msg;										// ウインドウプロシージャに渡すメッセージ
	
	// ウィンドウクラスの登録
	if (RegisterClassEx(&wcex) == 0)
	{
		return false;
	}

	// ウィンドウの作成
	hWnd = CreateWindowEx(0,
						CLASS_NAME,																					// クラス名
						WINDOW_NAME,																				// ウインドウ名
						WS_OVERLAPPEDWINDOW,																		// ウインドウスタイル
						CW_USEDEFAULT,																				// ウインドウの左上座標（X）
						CW_USEDEFAULT,																				// ウインドウの左上座標（Y）
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,											// ウインドウ横幅
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),		// ウインドウ縦幅
						NULL,																						// オーナーウインドウのハンドル
						NULL,																						// メニューハンドル
						hInstance,																					// アプリケーションインスタンスのハンドル
						NULL);																						// ウインドウ作成データ

	// 初期化処理(ウィンドウを作成してから行う)
	if(FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//フレームカウント初期化
	timeBeginPeriod(1);				// 分解能を設定
	dwExecLastTime = 
	dwFPSLastTime = timeGetTime();
	dwCurrentTime =
	dwFrameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);


	// --------------------------------------  メッセージループ---------------------------------------------
	while(1)
	{
		// ゲーム終了処理
		if (ePhase == PhaseExit)
		{
			break;
		}

        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)						//アプリケーションの終了要求
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		// ゲーム処理
		else
		{
			dwCurrentTime = timeGetTime();
			if((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
			{
#ifdef _DEBUG
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				Update();
				
				// プレイ人数分描画
				for (int i = 0; i < PlayerMode; i++)
				{
					switch (PlayerMode)
					{
					case 1:
						//描画領域を変更（ビューポート行列）
						g_pD3DDevice->SetViewport(&g_port1[i]);
						break;
					case 2:
						//描画領域を変更（ビューポート行列）
						g_pD3DDevice->SetViewport(&g_port2[i]);
						break;
					case 3:
					case 4:
						//描画領域を変更（ビューポート行列）
						g_pD3DDevice->SetViewport(&g_port4[i]);
						break;
					}

					// 描画処理
					Draw(i);
				}

				dwFrameCount++;
			}
		}
	}
	// -----------------------------------------------------------------------------------------------------

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	timeEndPeriod(1);				// 分解能を戻す

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 変数宣言
	static HANDLE Thread_Handle;

	switch(uMsg)
	{
		// 破棄
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		// 非システムキーメッセージ
	case WM_KEYDOWN:
		switch(wParam)
		{
		// ESCキーが押された場合
		case VK_ESCAPE:
			DestroyWindow(hWnd);	// ウインドウを破棄する
			break;
		}
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	// ウインドウの表示位置を画面中央に設定
	SetWindowCenter(hWnd);

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
    if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferCount			= 1;						// バックバッファの数
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			= d3ddm.Format;				// バックバッファフォーマットはディスプレイモードに合わせて使う
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed					= bWindow;					// ウィンドウモード
	d3dpp.EnableAutoDepthStencil	= TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// デプスバッファとして16bitを使う

	if(bWindow)
	{// ウィンドウモード
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{// フルスクリーンモード
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスオブジェクトの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									hWnd, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING, 
									&d3dpp, &g_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp, &g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF,
											hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp, &g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートパラメータの設定
    g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

#ifdef _DEBUG
	// 情報表示用フォントを設定
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pD3DXFont);

	// デバッグ
	InitDebugProc();

#endif

	// 入力処理の初期化
	InitInput(hInstance, hWnd);
	InitCamera();
	InitLight();
	InitSound(hWnd);
	// フィールドの初期化
	InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		10, 10, FIELD_SIZE, FIELD_SIZE, 0);
	
	// 壁の初期化
	InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, WALL_POS), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WALL_BLOCK_NUM_X, WALL_BLOCK_NUM_Y, WALL_SIZE_WIDTH, WALL_SIZE_HEIGHT);
	InitMeshWall(D3DXVECTOR3(-WALL_POS, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WALL_BLOCK_NUM_X, WALL_BLOCK_NUM_Y, WALL_SIZE_WIDTH, WALL_SIZE_HEIGHT);
	InitMeshWall(D3DXVECTOR3(WALL_POS, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WALL_BLOCK_NUM_X, WALL_BLOCK_NUM_Y, WALL_SIZE_WIDTH, WALL_SIZE_HEIGHT);
	InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, -WALL_POS), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WALL_BLOCK_NUM_X, WALL_BLOCK_NUM_Y, WALL_SIZE_WIDTH, WALL_SIZE_HEIGHT);

	// 壁(裏側用)
	InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, WALL_POS), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), WALL_BLOCK_NUM_X, WALL_BLOCK_NUM_Y, WALL_SIZE_WIDTH, WALL_SIZE_HEIGHT);
	InitMeshWall(D3DXVECTOR3(-WALL_POS, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), WALL_BLOCK_NUM_X, WALL_BLOCK_NUM_Y, WALL_SIZE_WIDTH, WALL_SIZE_HEIGHT);
	InitMeshWall(D3DXVECTOR3(WALL_POS, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), WALL_BLOCK_NUM_X, WALL_BLOCK_NUM_Y, WALL_SIZE_WIDTH, WALL_SIZE_HEIGHT);
	InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, -WALL_POS), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), WALL_BLOCK_NUM_X, WALL_BLOCK_NUM_Y, WALL_SIZE_WIDTH, WALL_SIZE_HEIGHT);
	
	InitShadow(0);
	InitPlayer(0);
	InitEnemy(0);
	InitScore(0);
	InitEScore(0);
	InitFrame(0);
	InitTitle(0);
	InitResult(0);
	InitGauge(0);
	InitEGauge(0);
	InitSpGauge(0);
	InitESpGauge(0);
	InitTimer(0);
	InitFrameSpgauge(0);
	InitSpmax(0);
	InitRedGauge(0);
	InitERedGauge(0);
	InitCountdown(0);
	InitKnockout(0);
	InitResultstar(0);
	InitWinner(0);
	InitEvaluation(0);
	InitTutorial(0);
	InitModeselect(0);
	InitBlackscreen(0);
	InitPause(0);
	InitDrawgame(0);
	InitCompany(0);
	InitThanks(0);
	InitParticle(0);
	InitSkyBox(0);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
#ifdef _DEBUG
	if(g_pD3DXFont != NULL)
	{// 情報表示用フォントの開放
		g_pD3DXFont->Release();
		g_pD3DXFont = NULL;
	}

	// デバッグ
	UninitDebugProc();

#endif
	if(g_pD3DDevice != NULL)
	{// デバイスの開放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if(g_pD3D != NULL)
	{// Direct3Dオブジェクトの開放
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// 入力処理の終了処理
	UninitInput();
	UninitSound();

	// モデルの終了処理
	UninitPlayer();
	UninitEnemy();
	UninitMeshField();
	UninitMeshWall();
	UninitShadow();
	UninitScore();
	UninitEScore();
	UninitFrame();
	UninitTitle();
	UninitResult();
	UninitGauge();
	UninitEGauge();
	UninitSpGauge();
	UninitESpGauge();
	UninitTimer();
	UninitFrameSpgauge();
	UninitSpmax();
	UninitRedGauge();
	UninitERedGauge();
	UninitCountdown();
	UninitKnockout();
	UninitResultstar();
	UninitWinner();
	UninitEvaluation();
	UninitTutorial();
	UninitModeselect();
	UninitBlackscreen();
	UninitPause();
	UninitDrawgame();
	UninitCompany();
	UninitThanks();
	UninitParticle();
	UninitSkyBox();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
#ifdef _DEBUG
	UpdateDebugProc();

#endif

	CHARA *playerWk = GetPlayer();
	CHARA *enemyWk = GetEnemy();

	////画面分割数の入力
	//if (GetKeyboardTrigger(DIK_1))
	//{
	//	PlayerMode = 1;
	//}
	//else if (GetKeyboardTrigger(DIK_2))
	//{
	//	PlayerMode = 2;
	//}
	//else if (GetKeyboardTrigger(DIK_3))
	//{
	//	PlayerMode = 3;
	//}
	//else if (GetKeyboardTrigger(DIK_4))
	//{
	//	PlayerMode = 4;
	//}
	
	// 描画モードの切り替え
if (GetKeyboardTrigger(DIK_0))
	{
		RenderWireframe = RenderWireframe ? false : true;
	}


	// 入力更新
	UpdateInput();

	switch (ePhase)
	{
	case PhaseCompanyLogo:
		UpdateBlackscreen();
		UpdateCompany();
		break;

	case PhaseTitle:
		UpdateBlackscreen();
		UpdateModeselect();
		UpdateTitle();

		break;

	case PhaseTutorial:
		UpdatePlayer();
		UpdateEnemy();
		UpdateGauge();
		UpdateEGauge();
		UpdateFrame();
		UpdateSpGauge();
		UpdateESpGauge();
		UpdateFrameSpgauge();
		UpdateSpmax();
		UpdateRedGauge();
		UpdateERedGauge();
		UpdateTutorial();
		UpdateMeshField();
		UpdateMeshWall();
		UpdateEnemy();
		UpdateCamera();
		UpdateShadow();
		UpdateParticle();
		UpdateSkyBox();

		break;

	case PhaseTraining:
		UpdatePlayer();
		UpdateEnemy();
		UpdateGauge();
		UpdateEGauge();
		UpdateFrame();
		UpdateSpGauge();
		UpdateESpGauge();
		UpdateFrameSpgauge();
		UpdateSpmax();
		UpdateRedGauge();
		UpdateERedGauge();
		UpdateMeshField();
		UpdateMeshWall();
		UpdateEnemy();
		UpdateCamera();
		UpdateShadow();
		UpdateParticle();
		UpdateSkyBox();
		UpdateTimer();

		break;

	case PhasePause:
		UpdateBlackscreen();
		UpdateModeselect();
		UpdatePause();
		break;

	case PhaseTrainingPause:
		UpdateBlackscreen();
		UpdateModeselect();
		UpdatePause();
		break;

	case PhaseCountdown:
		UpdatePlayer();
		UpdateEnemy();
		UpdateScore();
		UpdateEScore();
		UpdateGauge();
		UpdateEGauge();
		UpdateFrame();
		UpdateSpGauge();
		UpdateESpGauge();
		UpdateTimer();
		UpdateFrameSpgauge();
		UpdateSpmax();
		UpdateRedGauge();
		UpdateERedGauge();
		UpdateCountdown();
		UpdateMeshField();
		UpdateMeshWall();
		UpdateCamera();
		UpdateShadow();
		UpdateParticle();
		UpdateSkyBox();

		break;

	case PhaseGame:
		UpdateMeshField();
		UpdateMeshWall();
		UpdatePlayer();
		UpdateEnemy();
		UpdateCamera();
		UpdateShadow();
		UpdateScore();
		UpdateEScore();
		UpdateGauge();
		UpdateEGauge();
		UpdateFrame();
		UpdateSpGauge();
		UpdateESpGauge();
		UpdateTimer();
		UpdateFrameSpgauge();
		UpdateSpmax();
		UpdateRedGauge();
		UpdateERedGauge();
		UpdateParticle();
		UpdateSkyBox();

		break;

	case PhaseFinish:
		UpdatePlayer();
		UpdateEnemy();
		UpdateScore();
		UpdateEScore();
		UpdateGauge();
		UpdateEGauge();
		UpdateFrame();
		UpdateSpGauge();
		UpdateESpGauge();
		UpdateTimer();
		UpdateFrameSpgauge();
		UpdateSpmax();
		UpdateRedGauge();
		UpdateERedGauge();
		UpdateKnockout();
		UpdateMeshField();
		UpdateMeshWall();
		UpdateCamera();
		UpdateShadow();
		UpdateParticle();
		UpdateSkyBox();

		break;

	case PhaseResult:
		//ドローゲームの場合
		if (playerWk->HPzan == enemyWk->HPzan)
		{
			UpdateDrawgame();
		}
		else
		{
			UpdateResult();
			UpdateResultstar();
			UpdateWinner();
			UpdateEvaluation();
			//プレイヤー勝利の場合
			if (playerWk->HPzan > enemyWk->HPzan)
			{
				UpdateScore();
			}
			//エネミー勝利の場合
			else if (enemyWk->HPzan > playerWk->HPzan)
			{
				UpdateEScore();
			}
		}
		break;
	case PhaseThankyou:
		UpdateThanks();
		break;
	}
}

//=============================================================================
// 描画処理 引数:カメラ番号
//=============================================================================
void Draw(int no)
{
	CHARA *playerWk = GetPlayer();
	CHARA *enemyWk = GetEnemy();

	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		SetCamera(no);

		switch (ePhase)
		{
		case PhaseCompanyLogo:
			DrawBlackscreen();
			DrawCompany();
			break;

		case PhaseTitle:
			DrawBlackscreen();
			DrawModeselect();
			DrawTitle();
			break;

		case PhaseTraining:
			//BG
			DrawSkyBox();
			DrawMeshField();
			DrawMeshWall();

			//エフェクト
			DrawParticle();

			// キャラクター等
			DrawEnemy();
			DrawPlayer();

			//画面上のUI
			DrawRedGauge();
			DrawERedGauge();
			DrawGauge();
			DrawEGauge();
			DrawFrame();
			DrawTimer();

			//画面下のUI
			DrawSpGauge();
			DrawESpGauge();
			DrawFrameSpgauge();
			DrawSpmax();

			break;

		case PhaseTutorial:
			//BG
			DrawSkyBox();
			DrawMeshField();
			DrawMeshWall();

			//エフェクト
			DrawParticle();

			// キャラクター等
			DrawEnemy();
			DrawPlayer();

			//画面上のUI
			DrawRedGauge();
			DrawERedGauge();
			DrawGauge();
			DrawEGauge();
			DrawFrame();

			//画面下のUI
			DrawSpGauge();
			DrawESpGauge();
			DrawFrameSpgauge();
			DrawSpmax();

			DrawTutorial();
			break;

		case PhasePause:
			//BG
			DrawSkyBox();
			DrawMeshField();
			DrawMeshWall();

			//エフェクト
			DrawParticle();

			// キャラクター等
			DrawEnemy();
			DrawPlayer();

			//画面上のUI
			DrawScore();
			DrawEScore();
			DrawRedGauge();
			DrawERedGauge();
			DrawGauge();
			DrawEGauge();
			DrawFrame();
			DrawTimer();

			//画面下のUI
			DrawSpGauge();
			DrawESpGauge();
			DrawFrameSpgauge();
			DrawSpmax();

			//ポーズ画面
			DrawBlackscreen();
			DrawModeselect();
			DrawPause();

			break;

		case PhaseTrainingPause:
			//BG
			DrawSkyBox();
			DrawMeshField();
			DrawMeshWall();

			//エフェクト
			DrawParticle();

			// キャラクター等
			DrawEnemy();
			DrawPlayer();

			//画面上のUI
			DrawRedGauge();
			DrawERedGauge();
			DrawGauge();
			DrawEGauge();
			DrawFrame();
			DrawTimer();

			//画面下のUI
			DrawSpGauge();
			DrawESpGauge();
			DrawFrameSpgauge();
			DrawSpmax();

			//ポーズ画面
			DrawBlackscreen();
			DrawModeselect();
			DrawPause();

			break;

		case PhaseCountdown:
			//BG
			DrawSkyBox();
			DrawMeshField();
			DrawMeshWall();

			//エフェクト
			DrawParticle();

			// キャラクター等
			DrawEnemy();
			DrawPlayer();

			//画面上のUI
			DrawScore();
			DrawEScore();
			DrawRedGauge();
			DrawERedGauge();
			DrawGauge();
			DrawEGauge();
			DrawFrame();
			DrawTimer();

			//画面下のUI
			DrawSpGauge();
			DrawESpGauge();
			DrawFrameSpgauge();
			DrawSpmax();

			DrawCountdown();
			break;

		case PhaseGame:
			//BG
			DrawSkyBox();
			DrawMeshField();
			DrawMeshWall();

			// キャラクター等
			DrawEnemy();
			DrawPlayer();

			//エフェクト
			DrawParticle();

			//画面上のUI
			DrawScore();
			DrawEScore();
			DrawRedGauge();
			DrawERedGauge();
			DrawGauge();
			DrawEGauge();
			DrawFrame();
			DrawTimer();

			//画面下のUI
			DrawSpGauge();
			DrawESpGauge();
			DrawFrameSpgauge();
			DrawSpmax();

			break;

		case PhaseFinish:
			//BG
			DrawSkyBox();
			DrawMeshField();
			DrawMeshWall();

			//エフェクト
			DrawParticle();

			// キャラクター等
			DrawEnemy();
			DrawPlayer();

			//画面上のUI
			DrawScore();
			DrawEScore();
			DrawRedGauge();
			DrawERedGauge();
			DrawGauge();
			DrawEGauge();
			DrawFrame();
			DrawTimer();

			//画面下のUI
			DrawSpGauge();
			DrawESpGauge();
			DrawFrameSpgauge();
			DrawSpmax();

			DrawKnockout();
			break;

		case PhaseResult:
			//BG
			DrawSkyBox();
			DrawMeshField();
			DrawMeshWall();

			// キャラクター等
			DrawEnemy();
			DrawPlayer();

			//ドローゲームの場合
			if (playerWk->HPzan == enemyWk->HPzan)
			{
				DrawDrawgame();
			}
			else
			{
				DrawResult();
				//プレイヤー勝利の場合
				if (playerWk->HPzan > enemyWk->HPzan)
				{
					DrawScore();
				}
				//エネミー勝利の場合
				else if (enemyWk->HPzan > playerWk->HPzan)
				{
					DrawEScore();
				}
				DrawResultstar();
				DrawWinner();
				DrawEvaluation();
			}
			break;
		case PhaseThankyou:
			DrawThanks();
			break;
		}

#ifdef _DEBUG
		bool debug = GetRenderState();
		if (debug == true)
		{
			// デバッグ表示
			DrawDebugProc();
		}

		// FPS表示
		DrawFPS();

		PrintDebugProc("現在のゲームシーン %d\n", ePhase);
#endif

		// Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

#ifdef _DEBUG
//=============================================================================
// FPS表示
//=============================================================================
void DrawFPS(void)
{
	PrintDebugProc("FPS:%d\n", g_nCountFPS);
}
#endif

//=============================================================================
// マトリクスの取得
//=============================================================================
MATRIX *GetMatrix(void)
{
	return &MatrixState;
}

//=============================================================================
// 再初期化処理
//=============================================================================
void ReInit(void)
{
	InitShadow(1);
	// モデルの初期化
	InitPlayer(1);
	InitEnemy(1);
	InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10, 10, 100, 100, 1);
	InitScore(1);
	InitEScore(1);
	InitFrame(1);
	InitTitle(1);
	InitResult(1);
	InitGauge(1);
	InitEGauge(1);
	InitSpGauge(1);
	InitESpGauge(1);
	InitTimer(1);
	InitFrameSpgauge(1);
	InitSpmax(1);
	InitERedGauge(1);
	InitRedGauge(1);
	InitCountdown(1);
	InitKnockout(1);
	InitResultstar(1);
	InitWinner(1);
	InitEvaluation(1);
	InitTutorial(1);
	InitModeselect(1);
	InitBlackscreen(1);
	InitPause(1);
	InitDrawgame(1);
	InitCompany(1);
	InitThanks(1);
	InitSkyBox(1);
}

//=====================================================================================================
// シーン遷移
//=====================================================================================================
void SetPhase(int phase)
{
	MODESELECT *modeselect = GetModeselect(0);

	if (phase == PhasePause || phase == PhaseTrainingPause)
	{
		modeselect->pos.y = PAUSESELECT_POS_Y;
	}

	ePhase = phase;
}

//=====================================================================================================
// シーンのゲッター
//=====================================================================================================
int *GetPhase(void)
{
	return &ePhase;
}

//=====================================================================================================
// アスペクト比のゲッター カメラのプロジェクションマトリックスにセットする
//=====================================================================================================
float GetAspect(void)
{
	//2人プレイの時アスペクト比を変更する
	if (PlayerMode == 2)
	{
		return VIEW_ASPECT / 2.0f;

	}
	//それ以外はデフォルト
	else
	{
		return VIEW_ASPECT;
	}
}

//=====================================================================================================
// ウインドウの表示位置を画面中央に
//=====================================================================================================
bool SetWindowCenter(HWND hWnd)
{
	// 変数宣言
	RECT WindowRegion;		//	ウィンドウ領域
	RECT DesktopRegion;	//	デスクトップ領域
	int WindowPositionX;	//	ウィンドウ位置 X
	int WindowPositionY;	//	ウィンドウ位置 Y
	int WindowSizeX;		//	ウィンドウサイズ X
	int WindowSizeY;		//	ウィンドウサイズ Y

							// 各サイズの取得
	GetMonitorRect(&DesktopRegion);				//	デスクトップのサイズを取得
	GetWindowRect(hWnd, &WindowRegion);	//	ウィンドウのサイズを取得

													// 各座標の割り出し
	WindowSizeX = (WindowRegion.right - WindowRegion.left);														//	ウインドウの横幅の割り出し
	WindowSizeY = (WindowRegion.bottom - WindowRegion.top);														//	ウインドウの縦幅の割り出し
	WindowPositionX = (((DesktopRegion.right - DesktopRegion.left) - WindowSizeX) / 2 + DesktopRegion.left);	//	横方向の中央座標軸の割り出し
	WindowPositionY = (((DesktopRegion.bottom - DesktopRegion.top) - WindowSizeY) / 2 + DesktopRegion.top);	//	縦方向の中央座標軸の割り出し

																													// ウィンドウを画面中央に移動
	return SetWindowPos	//	SetWindowPos関数：子ウィンドウ、ポップアップウィンドウ、またはトップレベルウィンドウのサイズ、位置、および Z オーダーを変更する
						//						これらのウィンドウは、その画面上での表示に従って順序が決められる、
						//						最前面にあるウィンドウは最も高いランクを与えられ、Zオーダーの先頭に置かれる
						(
							hWnd,											//	ウィンドウハンドル
							NULL,											//	配置順序のハンドル：先行するウィンドウのハンドルを指定
							WindowPositionX,								//	ウィンドウ左上隅の"X"座標を指定：横方向の位置 X
							WindowPositionY,								//	ウィンドウ左上隅の"Y"座標を指定：縦方向の位置 Y
							WindowSizeX,									//	ウィンドウの横幅を指定 X
							WindowSizeY,									//	ウィンドウの縦幅を指定 Y
							(SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER)	//	ウィンドウ位置のオプション：ウィンドウのサイズや、位置の変更に関するフラグを指定
						);

}

//=============================================================================
// テクスチャを読み込む関数
//=============================================================================
HRESULT LoadTexture(LPCSTR SrcFile, LPDIRECT3DTEXTURE9* TexturePtr, const char* ErrorSrc)
{
	char Message[64];

	D3DXCreateTextureFromFile(g_pD3DDevice, SrcFile, TexturePtr);

	if (*TexturePtr == NULL)
	{
		sprintf_s(Message, "Load %s Texture Failed！", ErrorSrc);
		MessageBox(0, Message, "Error", 0);
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// 描画情報（TRUE:ワイヤーフレーム描画 FALSE:ソリッド描画）
//=============================================================================
bool GetRenderState(void)
{
	return RenderWireframe;
}