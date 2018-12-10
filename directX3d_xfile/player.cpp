//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "D3DXAnimation.h"
#include "AnimationModel.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
PLAYER playerWk[1];

// オブジェクトの作成
D3DXAnimation* PlayerAnimation;
AnimationModel* PlayerModel;
//UINT Idle;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(int type)
{
	PLAYER *playerWk = GetPlayer(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//オブジェクトの初期化
	PlayerAnimation = new D3DXAnimation(pDevice);
	PlayerModel = new AnimationModel();

	// 位置・回転・スケールの初期設定
	playerWk->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	playerWk->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	playerWk->D3DXMesh = NULL;
	playerWk->D3DXBuffMat = NULL;
	playerWk->NumMat = 0;

	playerWk->jump = false;
	playerWk->speed = PLAYER_JUMP_SPEED;

	if (type == 0)
	{
		//Xファイルの読み込み
		PlayerAnimation->Load_xFile(PLAYER_XFILE);
		PlayerModel->InitController(PlayerAnimation);

		//複数アニメーション
		//PlayerModel->ChangeAnimation(Idle);

		// メモリの動的確保
		playerWk->D3DTexture = (LPDIRECT3DTEXTURE9 *)malloc(playerWk->NumMat * sizeof(LPDIRECT3DTEXTURE9));
		playerWk->MeshMaterial = (D3DMATERIAL9 *)malloc(playerWk->NumMat * sizeof(D3DMATERIAL9));

#if 0		
		D3DXMATERIAL *pD3DXMat;

		pD3DXMat = (D3DXMATERIAL*)playerWk->D3DXBuffMat->GetBufferPointer();

		// テクスチャの読み込み
		for (int i = 0; i < ((signed)playerWk->NumMat); i++)
		{
			playerWk->D3DTexture[i] = NULL;

			D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
				pD3DXMat[i].pTextureFilename,					// ファイルの名前
				&playerWk->D3DTexture[i]);						// 読み込むメモリー
		}
#endif
	}

	playerWk->IdxShadow = CreateShadow(playerWk->pos, SHADOW_SIZE_X, SHADOW_SIZE_Z);
	playerWk->SizeShadow = 25.0f;
	playerWk->ColShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	PLAYER *playerWk = GetPlayer(0);

	SAFE_RELEASE(*playerWk->D3DTexture) { if (*playerWk->D3DTexture) { (*playerWk->D3DTexture)->Release(); (*playerWk->D3DTexture) = NULL; } }
	SAFE_DELETE(playerWk->D3DXMesh) { if (playerWk->D3DXMesh) { delete (playerWk->D3DXMesh); (playerWk->D3DXMesh) = NULL; } }
	SAFE_DELETE(playerWk->D3DXBuffMat) { if (playerWk->D3DXBuffMat) { delete (playerWk->D3DXBuffMat); (playerWk->D3DXBuffMat) = NULL; } }

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	PLAYER *playerWk = GetPlayer(0);
	CAMERA *camera = GetCamera(0);
	float fDiffRotY;

	//モデルの更新
	PlayerModel->AdvanceTime(1.0f / 60);

	//モデルの移動
	// D：右( → )
	if (GetKeyboardPress(DIK_D))
	{
		// D + S：右下( →・↓ )
		if (GetKeyboardPress(DIK_S))
		{
			playerWk->move.x -= sinf(camera->rot.y + D3DX_PI * 0.75f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y + D3DX_PI * 0.75f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y + D3DX_PI * 0.75f;
		}
		// D + W：右上( →・↑ )
		else if (GetKeyboardPress(DIK_W))
		{
			playerWk->move.x -= sinf(camera->rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y + D3DX_PI * 0.25f;
		}
		// D：右( → )
		else
		{
			playerWk->move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y + D3DX_PI * 0.50f;
		}
	}
	// A：左( ← )
	else if (GetKeyboardPress(DIK_A))
	{
		// A + S：左下( ←・↓ )
		if (GetKeyboardPress(DIK_S))
		{
			playerWk->move.x -= sinf(camera->rot.y - D3DX_PI * 0.75f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y - D3DX_PI * 0.75f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y - D3DX_PI * 0.75f;
		}
		// A + W：左上( ←・↑ )
		else if (GetKeyboardPress(DIK_W))
		{
			playerWk->move.x -= sinf(camera->rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y - D3DX_PI * 0.25f;
		}
		// A：左( ← )
		else
		{
			playerWk->move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y - D3DX_PI * 0.50f;
		}
	}
	// S：下( ↓ )
	else if (GetKeyboardPress(DIK_S))
	{
		playerWk->move.x -= sinf(D3DX_PI + camera->rot.y) * VALUE_MOVE;
		playerWk->move.z -= cosf(D3DX_PI + camera->rot.y) * VALUE_MOVE;

		playerWk->rotDest.y = D3DX_PI + camera->rot.y;
	}
	// W：上( ↑ )
	else if (GetKeyboardPress(DIK_W))
	{
		playerWk->move.x -= sinf(camera->rot.y) * VALUE_MOVE;
		playerWk->move.z -= cosf(camera->rot.y) * VALUE_MOVE;

		playerWk->rotDest.y = camera->rot.y;
	}


	// 目的の角度までの差分
	fDiffRotY = playerWk->rotDest.y - playerWk->rot.y;
	if (fDiffRotY > D3DX_PI)
	{
		fDiffRotY -= D3DX_PI * 2.0f;
	}
	if (fDiffRotY < -D3DX_PI)
	{
		fDiffRotY += D3DX_PI * 2.0f;
	}

	// 目的の角度まで慣性をかける
	playerWk->rot.y += fDiffRotY * RATE_ROTATE_MODEL;
	if (playerWk->rot.y > D3DX_PI)
	{
		playerWk->rot.y -= D3DX_PI * 2.0f;
	}
	if (playerWk->rot.y < -D3DX_PI)
	{
		playerWk->rot.y += D3DX_PI * 2.0f;
	}

	/// 位置移動
	playerWk->pos.x += playerWk->move.x;
	playerWk->pos.y += playerWk->move.y;
	playerWk->pos.z += playerWk->move.z;

	// 移動量に慣性をかける
	playerWk->move.x += (0.0f - playerWk->move.x) * RATE_MOVE_MODEL;
	playerWk->move.y += (0.0f - playerWk->move.y) * RATE_MOVE_MODEL;
	playerWk->move.z += (0.0f - playerWk->move.z) * RATE_MOVE_MODEL;

	//モデルの拡大縮小
	if (GetKeyboardPress(DIK_C))
	{
		playerWk->scl.x += VALUE_ENLARGEMENT;
		playerWk->scl.y += VALUE_ENLARGEMENT;
		playerWk->scl.z += VALUE_ENLARGEMENT;
	}
	else if (GetKeyboardPress(DIK_Z))
	{
		playerWk->scl.x -= VALUE_ENLARGEMENT;
		playerWk->scl.y -= VALUE_ENLARGEMENT;
		playerWk->scl.z -= VALUE_ENLARGEMENT;
	}

	//モデルのジャンプ
	if (GetKeyboardTrigger(DIK_SPACE) && playerWk->pos.y == 0.0f)
	{
		playerWk->jump = true;
	}
	if (playerWk->jump == true)
	{
		playerWk->pos.y += playerWk->speed;
		playerWk->speed -= 0.98f;
		if (playerWk->pos.y <= 0.0f)
		{
			playerWk->speed = PLAYER_JUMP_SPEED;
			playerWk->pos.y = 0.0f;
			playerWk->jump = false;
		}
	}

	// 影の位置設定
	SetPositionShadow(playerWk->IdxShadow, D3DXVECTOR3(playerWk->pos.x, 0.1f, playerWk->pos.z));
	SetVertexShadow(playerWk->IdxShadow, playerWk->SizeShadow, playerWk->SizeShadow);
	SetColorShadow(playerWk->IdxShadow, playerWk->ColShadow);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DMATERIAL9 matDef;
	MATRIX matrix = *GetMatrix();

	PLAYER *playerWk = GetPlayer(0);
	CAMERA *cameraWk = GetCamera(0);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&matrix.world);

	// スケールを反映
	D3DXMatrixScaling(&matrix.scale, playerWk->scl.x, playerWk->scl.y, playerWk->scl.z);
	D3DXMatrixMultiply(&matrix.world, &matrix.world, &matrix.scale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&matrix.scale, playerWk->rot.y, playerWk->rot.x, playerWk->rot.z);
	D3DXMatrixMultiply(&matrix.world, &matrix.world, &matrix.scale);

	// 移動を反映
	D3DXMatrixTranslation(&matrix.translation, playerWk->pos.x, playerWk->pos.y, playerWk->pos.z);
	D3DXMatrixMultiply(&matrix.world, &matrix.world, &matrix.translation);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &matrix.world);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// エネミーのレンダリング
	PlayerModel->SetMatrix(&matrix.world);
	PlayerModel->Render();

#if 0
	D3DXMATERIAL *pD3DXMat;

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)playerWk->D3DXBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)playerWk->NumMat; nCntMat++)
	{

		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, playerWk->D3DTexture[nCntMat]);

		// 描画
		playerWk->D3DXMesh->DrawSubset(nCntMat);
	}

#endif

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
//プレイヤーの情報を取得する
//=============================================================================
PLAYER *GetPlayer(int pno)
{
	return &playerWk[pno];
}