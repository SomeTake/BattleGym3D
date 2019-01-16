//=============================================================================
//
// ���C������ [main.cpp]
// Author : HAL���� GP11B341-17 80277 ���J���u
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
#include "eguage.h"
#include "eredguage.h"
#include "escore.h"
#include "espguage.h"
#include "evaluation.h"
#include "frame.h"
#include "framespguage.h"
#include "guage.h"
#include "knockout.h"
#include "modeselect.h"
#include "pause.h"
#include "redguage.h"
#include "result.h"
#include "resultstar.h"
#include "spguage.h"
#include "spmax.h"
#include "timer.h"
#include "title.h"
#include "tutorial.h"
#include "winner.h"
#include "sound.h"
#include "meshwall.h"
#include "thanks.h"
#include "effect.h"
#include "skybox.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"BattleGym3D"		// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �v���g�^�C�v�錾
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

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;					// Direct3D �I�u�W�F�N�g
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;			// Device�I�u�W�F�N�g(�`��ɕK�v)

int ePhase = PhaseGame;						// �Q�[���̊J�n�ʒu&�V�[���J��
int PlayerMode = 1;									// �v���C���[�l��

MATRIX MatrixState;									// �}�g���N�X

#ifdef _DEBUG
static LPD3DXFONT	g_pD3DXFont = NULL;				// �t�H���g�ւ̃|�C���^
int					g_nCountFPS;					// FPS�J�E���^
#endif

													// ��ʕ���
struct MY_VERTEX {
	float px, py, pz;
	DWORD color;
};

// �����T�C�Y�w��
// 4����
D3DVIEWPORT9 g_port4[] = { { 0, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,0.0f,1.0f },
{ SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,0.0f,1.0f },
{ 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,0.0f,1.0f },
{ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,0.0f,1.0f }
};

// ���Q����
D3DVIEWPORT9 g_port2[] = { { 0, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT,0.0f,1.0f },
{ SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT,0.0f,1.0f }
};

// ���̂܂�
D3DVIEWPORT9 g_port1[] = { { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,0.0f,1.0f }
};

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand((unsigned)time(NULL));

	// �x��
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	DWORD dwExecLastTime;							// �ŏI���s����
	DWORD dwFPSLastTime;							// ���݃t���[��
	DWORD dwCurrentTime;							// ���ݎ���
	DWORD dwFrameCount;								// �t���[���J�E���g

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),							// �\���̂̃T�C�Y
		CS_CLASSDC,									// �E�C���h�E�X�^�C���FCS_HREDRAW / CS_VREDRAW
		WndProc,									// �E�C���h�E�v���V�[�W���̃A�h���X
		0,											// �\��������
		0,											// �E�C���h�E�I�u�W�F�N�g�쐬���Ɋm�ۂ���郁�����T�C�Y
		hInstance,									// �C���X�^���X�n���h��
		LoadIcon(hInstance, (LPCTSTR)IDI_ICON1),	// �A�v���̃V���[�g�J�b�g�ȂǂŎg�p�i�A�C�R���̐ݒ�j
		LoadCursor(NULL, IDC_ARROW),				// �E�C���h�E�̃N���C�A���g��̃}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),					// �E�C���h�E�̃N���C�A���g�̈�̔w�i�F
		NULL,										// ���j���[��
		CLASS_NAME,									// �E�C���h�E�N���X�̖��O
		LoadIcon(hInstance, (LPCTSTR)IDI_ICON2)		// �E�C���h�E�̃A�C�R��
	};
	HWND hWnd;										// �E�C���h�E�n���h��
	MSG msg;										// �E�C���h�E�v���V�[�W���ɓn�����b�Z�[�W
	
	// �E�B���h�E�N���X�̓o�^
	if (RegisterClassEx(&wcex) == 0)
	{
		return false;
	}

	// �E�B���h�E�̍쐬
	hWnd = CreateWindowEx(0,
						CLASS_NAME,																					// �N���X��
						WINDOW_NAME,																				// �E�C���h�E��
						WS_OVERLAPPEDWINDOW,																		// �E�C���h�E�X�^�C��
						CW_USEDEFAULT,																				// �E�C���h�E�̍�����W�iX�j
						CW_USEDEFAULT,																				// �E�C���h�E�̍�����W�iY�j
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,											// �E�C���h�E����
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),		// �E�C���h�E�c��
						NULL,																						// �I�[�i�[�E�C���h�E�̃n���h��
						NULL,																						// ���j���[�n���h��
						hInstance,																					// �A�v���P�[�V�����C���X�^���X�̃n���h��
						NULL);																						// �E�C���h�E�쐬�f�[�^

	// ����������(�E�B���h�E���쐬���Ă���s��)
	if(FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//�t���[���J�E���g������
	timeBeginPeriod(1);				// ����\��ݒ�
	dwExecLastTime = 
	dwFPSLastTime = timeGetTime();
	dwCurrentTime =
	dwFrameCount = 0;

	// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);


	// --------------------------------------  ���b�Z�[�W���[�v---------------------------------------------
	while(1)
	{
		// �Q�[���I������
		if (ePhase == PhaseExit)
		{
			break;
		}

        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)						//�A�v���P�[�V�����̏I���v��
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		// �Q�[������
		else
		{
			dwCurrentTime = timeGetTime();
			if((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
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

				// �X�V����
				Update();
				
				// �v���C�l�����`��
				for (int i = 0; i < PlayerMode; i++)
				{
					switch (PlayerMode)
					{
					case 1:
						//�`��̈��ύX�i�r���[�|�[�g�s��j
						g_pD3DDevice->SetViewport(&g_port1[i]);
						break;
					case 2:
						//�`��̈��ύX�i�r���[�|�[�g�s��j
						g_pD3DDevice->SetViewport(&g_port2[i]);
						break;
					case 3:
					case 4:
						//�`��̈��ύX�i�r���[�|�[�g�s��j
						g_pD3DDevice->SetViewport(&g_port4[i]);
						break;
					}

					// �`�揈��
					Draw(i);
				}

				dwFrameCount++;
			}
		}
	}
	// -----------------------------------------------------------------------------------------------------

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();

	timeEndPeriod(1);				// ����\��߂�

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// �ϐ��錾
	static HANDLE Thread_Handle;

	switch(uMsg)
	{
		// �j��
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		// ��V�X�e���L�[���b�Z�[�W
	case WM_KEYDOWN:
		switch(wParam)
		{
		// ESC�L�[�������ꂽ�ꍇ
		case VK_ESCAPE:
			DestroyWindow(hWnd);	// �E�C���h�E��j������
			break;
		}
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	// �E�C���h�E�̕\���ʒu����ʒ����ɐݒ�
	SetWindowCenter(hWnd);

	// Direct3D�I�u�W�F�N�g�̍쐬
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
    if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ���[�N���[���N���A
	d3dpp.BackBufferCount			= 1;						// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat			= d3ddm.Format;				// �o�b�N�o�b�t�@�t�H�[�}�b�g�̓f�B�X�v���C���[�h�ɍ��킹�Ďg��
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed					= bWindow;					// �E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil	= TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// �f�v�X�o�b�t�@�Ƃ���16bit���g��

	if(bWindow)
	{// �E�B���h�E���[�h
		d3dpp.FullScreen_RefreshRateInHz = 0;								// ���t���b�V�����[�g
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
	}
	else
	{// �t���X�N���[�����[�h
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	}

	// �f�o�C�X�I�u�W�F�N�g�̐���
	// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									hWnd, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING, 
									&d3dpp, &g_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs�Ȃ�
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp, &g_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs�Ȃ�
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF,
											hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp, &g_pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�p�����[�^�̐ݒ�
    g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

#ifdef _DEBUG
	// ���\���p�t�H���g��ݒ�
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pD3DXFont);

	// �f�o�b�O
	InitDebugProc();

#endif

	// ���͏����̏�����
	InitInput(hInstance, hWnd);
	InitCamera();
	InitLight();
	InitSound(hWnd);
	// �t�B�[���h�̏�����
	InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		10, 10, FIELD_SIZE, FIELD_SIZE, 0);
	
	// �ǂ̏�����
	InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, WALL_POS), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WALL_BLOCK_NUM_X, WALL_BLOCK_NUM_Y, WALL_SIZE_WIDTH, WALL_SIZE_HEIGHT);
	InitMeshWall(D3DXVECTOR3(-WALL_POS, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WALL_BLOCK_NUM_X, WALL_BLOCK_NUM_Y, WALL_SIZE_WIDTH, WALL_SIZE_HEIGHT);
	InitMeshWall(D3DXVECTOR3(WALL_POS, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WALL_BLOCK_NUM_X, WALL_BLOCK_NUM_Y, WALL_SIZE_WIDTH, WALL_SIZE_HEIGHT);
	InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, -WALL_POS), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WALL_BLOCK_NUM_X, WALL_BLOCK_NUM_Y, WALL_SIZE_WIDTH, WALL_SIZE_HEIGHT);

	// ��(�����p)
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
	InitEffect(0);
	InitScore(0);
	InitEScore(0);
	InitFrame(0);
	InitTitle(0);
	InitResult(0);
	InitGuage(0);
	InitEGuage(0);
	InitSpGuage(0);
	InitESpGuage(0);
	InitTimer(0);
	InitFrameSpguage(0);
	InitSpmax(0);
	InitRedGuage(0);
	InitERedGuage(0);
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
// �I������
//=============================================================================
void Uninit(void)
{
#ifdef _DEBUG
	if(g_pD3DXFont != NULL)
	{// ���\���p�t�H���g�̊J��
		g_pD3DXFont->Release();
		g_pD3DXFont = NULL;
	}

	// �f�o�b�O
	UninitDebugProc();

#endif
	if(g_pD3DDevice != NULL)
	{// �f�o�C�X�̊J��
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if(g_pD3D != NULL)
	{// Direct3D�I�u�W�F�N�g�̊J��
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// ���͏����̏I������
	UninitInput();
	UninitSound();

	// ���f���̏I������
	UninitPlayer();
	UninitEnemy();
	UninitMeshField();
	UninitMeshWall();
	UninitShadow();
	UninitEffect();
	UninitScore();
	UninitEScore();
	UninitFrame();
	UninitTitle();
	UninitResult();
	UninitGuage();
	UninitEGuage();
	UninitSpGuage();
	UninitESpGuage();
	UninitTimer();
	UninitFrameSpguage();
	UninitSpmax();
	UninitRedGuage();
	UninitERedGuage();
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
// �X�V����
//=============================================================================
void Update(void)
{
#ifdef _DEBUG
	UpdateDebugProc();

#endif

	PLAYER *playerWk = GetPlayer();
	ENEMY *enemyWk = GetEnemy();

	////��ʕ������̓���
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

	// ���͍X�V
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
		UpdateGuage();
		UpdateEGuage();
		UpdateFrame();
		UpdateSpGuage();
		UpdateESpGuage();
		UpdateFrameSpguage();
		UpdateSpmax();
		UpdateRedGuage();
		UpdateERedGuage();
		UpdateTutorial();
		UpdateMeshField();
		UpdateMeshWall();
		UpdatePlayer();
		UpdateEnemy();
		UpdateCamera();
		UpdateShadow();
		UpdateEffect();
		UpdateParticle();
		UpdateSkyBox();

		break;

	case PhaseTraining:
		UpdatePlayer();
		UpdateEnemy();
		UpdateGuage();
		UpdateEGuage();
		UpdateFrame();
		UpdateSpGuage();
		UpdateESpGuage();
		UpdateFrameSpguage();
		UpdateSpmax();
		UpdateRedGuage();
		UpdateERedGuage();
		UpdateMeshField();
		UpdateMeshWall();
		UpdatePlayer();
		UpdateEnemy();
		UpdateCamera();
		UpdateShadow();
		UpdateEffect();
		UpdateParticle();
		UpdateSkyBox();

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
		UpdateGuage();
		UpdateEGuage();
		UpdateFrame();
		UpdateSpGuage();
		UpdateESpGuage();
		UpdateTimer();
		UpdateFrameSpguage();
		UpdateSpmax();
		UpdateRedGuage();
		UpdateERedGuage();
		UpdateCountdown();
		UpdateMeshField();
		UpdateMeshWall();
		UpdateCamera();
		UpdateShadow();
		UpdateEffect();
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
		UpdateEffect();
		UpdateScore();
		UpdateEScore();
		UpdateGuage();
		UpdateEGuage();
		UpdateFrame();
		UpdateSpGuage();
		UpdateESpGuage();
		UpdateTimer();
		UpdateFrameSpguage();
		UpdateSpmax();
		UpdateRedGuage();
		UpdateERedGuage();
		UpdateParticle();
		UpdateSkyBox();

		break;

	case PhaseFinish:
		UpdatePlayer();
		UpdateEnemy();
		UpdateScore();
		UpdateEScore();
		UpdateGuage();
		UpdateEGuage();
		UpdateFrame();
		UpdateSpGuage();
		UpdateESpGuage();
		UpdateTimer();
		UpdateFrameSpguage();
		UpdateSpmax();
		UpdateRedGuage();
		UpdateERedGuage();
		UpdateKnockout();
		UpdateMeshField();
		UpdateMeshWall();
		UpdateCamera();
		UpdateShadow();
		UpdateEffect();
		UpdateParticle();
		UpdateSkyBox();

		break;

	case PhaseResult:
		//�h���[�Q�[���̏ꍇ
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
			//�v���C���[�����̏ꍇ
			if (playerWk->HPzan > enemyWk->HPzan)
			{
				UpdateScore();
			}
			//�G�l�~�[�����̏ꍇ
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
// �`�揈�� ����:�J�����ԍ�
//=============================================================================
void Draw(int no)
{
	PLAYER *playerWk = GetPlayer();
	ENEMY *enemyWk = GetEnemy();

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
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

			//�G�t�F�N�g
			DrawEffect();
			DrawParticle();

			// �L�����N�^�[��
			DrawEnemy();
			DrawPlayer();

			//��ʏ��UI
			DrawRedGuage();
			DrawERedGuage();
			DrawGuage();
			DrawEGuage();
			DrawFrame();

			//��ʉ���UI
			DrawSpGuage();
			DrawESpGuage();
			DrawFrameSpguage();
			DrawSpmax();

			break;

		case PhaseTutorial:
			//BG
			DrawSkyBox();
			DrawMeshField();
			DrawMeshWall();

			//�G�t�F�N�g
			DrawEffect();
			DrawParticle();

			// �L�����N�^�[��
			DrawEnemy();
			DrawPlayer();

			//��ʏ��UI
			DrawRedGuage();
			DrawERedGuage();
			DrawGuage();
			DrawEGuage();
			DrawFrame();

			//��ʉ���UI
			DrawSpGuage();
			DrawESpGuage();
			DrawFrameSpguage();
			DrawSpmax();

			DrawTutorial();
			break;

		case PhasePause:
			//BG
			DrawSkyBox();
			DrawMeshField();
			DrawMeshWall();

			//�G�t�F�N�g
			DrawEffect();
			DrawParticle();

			// �L�����N�^�[��
			DrawEnemy();
			DrawPlayer();

			//��ʏ��UI
			DrawScore();
			DrawEScore();
			DrawRedGuage();
			DrawERedGuage();
			DrawGuage();
			DrawEGuage();
			DrawFrame();
			DrawTimer();

			//��ʉ���UI
			DrawSpGuage();
			DrawESpGuage();
			DrawFrameSpguage();
			DrawSpmax();

			//�|�[�Y���
			DrawBlackscreen();
			DrawModeselect();
			DrawPause();

			break;

		case PhaseTrainingPause:
			//BG
			DrawSkyBox();
			DrawMeshField();
			DrawMeshWall();

			//�G�t�F�N�g
			DrawEffect();
			DrawParticle();

			// �L�����N�^�[��
			DrawEnemy();
			DrawPlayer();

			//��ʏ��UI
			DrawRedGuage();
			DrawERedGuage();
			DrawGuage();
			DrawEGuage();
			DrawFrame();

			//��ʉ���UI
			DrawSpGuage();
			DrawESpGuage();
			DrawFrameSpguage();
			DrawSpmax();

			//�|�[�Y���
			DrawBlackscreen();
			DrawModeselect();
			DrawPause();

			break;

		case PhaseCountdown:
			//BG
			DrawSkyBox();
			DrawMeshField();
			DrawMeshWall();

			//�G�t�F�N�g
			DrawEffect();
			DrawParticle();

			// �L�����N�^�[��
			DrawEnemy();
			DrawPlayer();

			//��ʏ��UI
			DrawScore();
			DrawEScore();
			DrawRedGuage();
			DrawERedGuage();
			DrawGuage();
			DrawEGuage();
			DrawFrame();
			DrawTimer();

			//��ʉ���UI
			DrawSpGuage();
			DrawESpGuage();
			DrawFrameSpguage();
			DrawSpmax();

			DrawCountdown();
			break;

		case PhaseGame:
			//BG
			DrawSkyBox();
			DrawMeshField();
			DrawMeshWall();

			//�G�t�F�N�g
			DrawEffect();
			DrawParticle();

			// �L�����N�^�[��
			DrawEnemy();
			DrawPlayer();

			//��ʏ��UI
			DrawScore();
			DrawEScore();
			DrawRedGuage();
			DrawERedGuage();
			DrawGuage();
			DrawEGuage();
			DrawFrame();
			DrawTimer();

			//��ʉ���UI
			DrawSpGuage();
			DrawESpGuage();
			DrawFrameSpguage();
			DrawSpmax();

			break;

		case PhaseFinish:
			//BG
			DrawSkyBox();
			DrawMeshField();
			DrawMeshWall();

			//�G�t�F�N�g
			DrawEffect();
			DrawParticle();

			// �L�����N�^�[��
			DrawEnemy();
			DrawPlayer();

			//��ʏ��UI
			DrawScore();
			DrawEScore();
			DrawRedGuage();
			DrawERedGuage();
			DrawGuage();
			DrawEGuage();
			DrawFrame();
			DrawTimer();

			//��ʉ���UI
			DrawSpGuage();
			DrawESpGuage();
			DrawFrameSpguage();
			DrawSpmax();

			DrawKnockout();
			break;

		case PhaseResult:
			//BG
			DrawSkyBox();
			DrawMeshField();
			DrawMeshWall();

			// �L�����N�^�[��
			DrawEnemy();
			DrawPlayer();

			//�h���[�Q�[���̏ꍇ
			if (playerWk->HPzan == enemyWk->HPzan)
			{
				DrawDrawgame();
			}
			else
			{
				DrawResult();
				//�v���C���[�����̏ꍇ
				if (playerWk->HPzan > enemyWk->HPzan)
				{
					DrawScore();
				}
				//�G�l�~�[�����̏ꍇ
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
		// �f�o�b�O�\��
		DrawDebugProc();

		// FPS�\��
		DrawFPS();

		PrintDebugProc("���݂̃Q�[���V�[�� %d\n", ePhase);
#endif

		// Direct3D�ɂ��`��̏I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// �f�o�C�X�̎擾
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

#ifdef _DEBUG
//=============================================================================
// FPS�\��
//=============================================================================
void DrawFPS(void)
{
	PrintDebugProc("FPS:%d\n", g_nCountFPS);
}
#endif

//=============================================================================
// �}�g���N�X�̎擾
//=============================================================================
MATRIX *GetMatrix(void)
{
	return &MatrixState;
}

//=============================================================================
// �ď���������
//=============================================================================
void ReInit(void)
{
	InitShadow(1);
	// ���f���̏�����
	InitPlayer(1);
	InitEnemy(1);
	InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10, 10, 100, 100, 1);
	InitEffect(1);
	InitScore(1);
	InitEScore(1);
	InitFrame(1);
	InitTitle(1);
	InitResult(1);
	InitGuage(1);
	InitEGuage(1);
	InitSpGuage(1);
	InitESpGuage(1);
	InitTimer(1);
	InitFrameSpguage(1);
	InitSpmax(1);
	InitERedGuage(1);
	InitRedGuage(1);
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
// �V�[���J��
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
// �V�[���̃Q�b�^�[
//=====================================================================================================
int *GetPhase(void)
{
	return &ePhase;
}

//=====================================================================================================
// �A�X�y�N�g��̃Q�b�^�[ �J�����̃v���W�F�N�V�����}�g���b�N�X�ɃZ�b�g����
//=====================================================================================================
float GetAspect(void)
{
	//2�l�v���C�̎��A�X�y�N�g���ύX����
	if (PlayerMode == 2)
	{
		return VIEW_ASPECT / 2.0f;

	}
	//����ȊO�̓f�t�H���g
	else
	{
		return VIEW_ASPECT;
	}
}

//=====================================================================================================
// �E�C���h�E�̕\���ʒu����ʒ�����
//=====================================================================================================
bool SetWindowCenter(HWND hWnd)
{
	// �ϐ��錾
	RECT WindowRegion;		//	�E�B���h�E�̈�
	RECT DesktopRegion;	//	�f�X�N�g�b�v�̈�
	int WindowPositionX;	//	�E�B���h�E�ʒu X
	int WindowPositionY;	//	�E�B���h�E�ʒu Y
	int WindowSizeX;		//	�E�B���h�E�T�C�Y X
	int WindowSizeY;		//	�E�B���h�E�T�C�Y Y

							// �e�T�C�Y�̎擾
	GetMonitorRect(&DesktopRegion);				//	�f�X�N�g�b�v�̃T�C�Y���擾
	GetWindowRect(hWnd, &WindowRegion);	//	�E�B���h�E�̃T�C�Y���擾

													// �e���W�̊���o��
	WindowSizeX = (WindowRegion.right - WindowRegion.left);														//	�E�C���h�E�̉����̊���o��
	WindowSizeY = (WindowRegion.bottom - WindowRegion.top);														//	�E�C���h�E�̏c���̊���o��
	WindowPositionX = (((DesktopRegion.right - DesktopRegion.left) - WindowSizeX) / 2 + DesktopRegion.left);	//	�������̒������W���̊���o��
	WindowPositionY = (((DesktopRegion.bottom - DesktopRegion.top) - WindowSizeY) / 2 + DesktopRegion.top);	//	�c�����̒������W���̊���o��

																													// �E�B���h�E����ʒ����Ɉړ�
	return SetWindowPos	//	SetWindowPos�֐��F�q�E�B���h�E�A�|�b�v�A�b�v�E�B���h�E�A�܂��̓g�b�v���x���E�B���h�E�̃T�C�Y�A�ʒu�A����� Z �I�[�_�[��ύX����
						//						�����̃E�B���h�E�́A���̉�ʏ�ł̕\���ɏ]���ď��������߂���A
						//						�őO�ʂɂ���E�B���h�E�͍ł����������N��^�����AZ�I�[�_�[�̐擪�ɒu�����
						(
							hWnd,											//	�E�B���h�E�n���h��
							NULL,											//	�z�u�����̃n���h���F��s����E�B���h�E�̃n���h�����w��
							WindowPositionX,								//	�E�B���h�E�������"X"���W���w��F�������̈ʒu X
							WindowPositionY,								//	�E�B���h�E�������"Y"���W���w��F�c�����̈ʒu Y
							WindowSizeX,									//	�E�B���h�E�̉������w�� X
							WindowSizeY,									//	�E�B���h�E�̏c�����w�� Y
							(SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER)	//	�E�B���h�E�ʒu�̃I�v�V�����F�E�B���h�E�̃T�C�Y��A�ʒu�̕ύX�Ɋւ���t���O���w��
						);

}

//=============================================================================
// �e�N�X�`����ǂݍ��ފ֐�
//=============================================================================
HRESULT LoadTexture(LPCSTR SrcFile, LPDIRECT3DTEXTURE9* TexturePtr, const char* ErrorSrc)
{
	char Message[64];

	D3DXCreateTextureFromFile(g_pD3DDevice, SrcFile, TexturePtr);

	if (*TexturePtr == NULL)
	{
		sprintf_s(Message, "Load %s Texture Failed�I", ErrorSrc);
		MessageBox(0, Message, "Error", 0);
		return E_FAIL;
	}

	return S_OK;
}
