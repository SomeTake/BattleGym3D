//=============================================================================
//
// エネミー処理 [enemy.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "main.h"
#include "enemy.h"
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
ENEMY enemyWk[1];

// オブジェクトの作成
D3DXAnimation* EnemyAnimation;
AnimationModel* EnemyModel;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(int type)
{
	ENEMY *enemyWk = GetEnemy(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ModelIndex = 0;

	//オブジェクトの初期化
	EnemyAnimation = new D3DXAnimation(pDevice);
	EnemyModel = new AnimationModel();

	// 位置・回転・スケールの初期設定
	enemyWk->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	enemyWk->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	enemyWk->rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	enemyWk->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	enemyWk->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	enemyWk->D3DXMesh = NULL;
	enemyWk->D3DXBuffMat = NULL;
	enemyWk->NumMat = 0;

	enemyWk->jump = false;
	enemyWk->speed = ENEMY_JUMP_SPEED;
	enemyWk->HP = 1000;
	enemyWk->HPzan = enemyWk->HP;

	if (type == 0)
	{
		//Xファイルの読み込み
		EnemyAnimation->Load_xFile(ENEMY_XFILE);
		EnemyModel->InitController(EnemyAnimation);

		// Xファイルの読み込み
		//if (FAILED(D3DXLoadMeshFromX(ENEMY_XFILE,			// 読み込むモデルファイル名(Xファイル)
		//	D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		//	pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		//	NULL,					// 隣接性データを含むバッファへのポインタ
		//	&enemyWk->D3DXBuffMat,	// マテリアルデータを含むバッファへのポインタ
		//	NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		//	&enemyWk->NumMat,		// D3DXMATERIAL構造体の数
		//	&enemyWk->D3DXMesh)))	// ID3DXMeshインターフェイスへのポインタのアドレス=頂点情報の結果が入る
		//{
		//	return E_FAIL;
		//}

		// メモリの動的確保
		enemyWk->D3DTexture = (LPDIRECT3DTEXTURE9 *)malloc(enemyWk->NumMat * sizeof(LPDIRECT3DTEXTURE9));
		enemyWk->MeshMaterial = (D3DMATERIAL9 *)malloc(enemyWk->NumMat * sizeof(D3DMATERIAL9));

#if 0
		D3DXMATERIAL *pD3DXMat;

		pD3DXMat = (D3DXMATERIAL*)enemyWk->D3DXBuffMat->GetBufferPointer();

		// テクスチャの読み込み
		for (int i = 0; i < ((signed)enemyWk->NumMat); i++)
		{
			enemyWk->D3DTexture[i] = NULL;

			D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
				pD3DXMat[i].pTextureFilename,					// ファイルの名前
				&enemyWk->D3DTexture[i]);						// 読み込むメモリー
		}
#endif
	}

	enemyWk->IdxShadow = CreateShadow(enemyWk->pos, SHADOW_SIZE_X, SHADOW_SIZE_Z);
	enemyWk->SizeShadow = 25.0f;
	enemyWk->ColShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	ENEMY *enemyWk = GetEnemy(0);

	// デストラクタ
	SAFE_RELEASE(*enemyWk->D3DTexture) {if(*enemyWk->D3DTexture){(*enemyWk->D3DTexture)->Release();(*enemyWk->D3DTexture)=NULL;}}
	SAFE_DELETE(enemyWk->D3DXMesh)	{if(enemyWk->D3DXMesh){delete (enemyWk->D3DXMesh); (enemyWk->D3DXMesh) = NULL;}}
	SAFE_DELETE(enemyWk->D3DXBuffMat) { if (enemyWk->D3DXBuffMat) { delete (enemyWk->D3DXBuffMat); (enemyWk->D3DXBuffMat) = NULL; } }

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	ENEMY *enemyWk = GetEnemy(0);
	CAMERA *camera = GetCamera(1);
	float fDiffRotY;

	//モデルの更新
	EnemyModel->AdvanceTime(1.0f / 60);
	
	//モデルの移動
	// D：右( → )
	if (GetKeyboardPress(DIK_L))
	{
		// D + S：右下( →・↓ )
		if (GetKeyboardPress(DIK_K))
		{
			enemyWk->move.x -= sinf(camera->rot.y + D3DX_PI * 0.75f) * VALUE_MOVE;
			enemyWk->move.z -= cosf(camera->rot.y + D3DX_PI * 0.75f) * VALUE_MOVE;

			enemyWk->rotDest.y = camera->rot.y + D3DX_PI * 0.75f;
		}
		// D + W：右上( →・↑ )
		else if (GetKeyboardPress(DIK_I))
		{
			enemyWk->move.x -= sinf(camera->rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;
			enemyWk->move.z -= cosf(camera->rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;

			enemyWk->rotDest.y = camera->rot.y + D3DX_PI * 0.25f;
		}
		// D：右( → )
		else
		{
			enemyWk->move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * VALUE_MOVE;
			enemyWk->move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * VALUE_MOVE;

			enemyWk->rotDest.y = camera->rot.y + D3DX_PI * 0.50f;
		}
	}
	// A：左( ← )
	else if (GetKeyboardPress(DIK_J))
	{
		// A + S：左下( ←・↓ )
		if (GetKeyboardPress(DIK_K))
		{
			enemyWk->move.x -= sinf(camera->rot.y - D3DX_PI * 0.75f) * VALUE_MOVE;
			enemyWk->move.z -= cosf(camera->rot.y - D3DX_PI * 0.75f) * VALUE_MOVE;

			enemyWk->rotDest.y = camera->rot.y - D3DX_PI * 0.75f;
		}
		// A + W：左上( ←・↑ )
		else if (GetKeyboardPress(DIK_I))
		{
			enemyWk->move.x -= sinf(camera->rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;
			enemyWk->move.z -= cosf(camera->rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;

			enemyWk->rotDest.y = camera->rot.y - D3DX_PI * 0.25f;
		}
		// A：左( ← )
		else
		{
			enemyWk->move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * VALUE_MOVE;
			enemyWk->move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * VALUE_MOVE;

			enemyWk->rotDest.y = camera->rot.y - D3DX_PI * 0.50f;
		}
	}
	// S：下( ↓ )
	else if (GetKeyboardPress(DIK_K))
	{
		enemyWk->move.x -= sinf(D3DX_PI + camera->rot.y) * VALUE_MOVE;
		enemyWk->move.z -= cosf(D3DX_PI + camera->rot.y) * VALUE_MOVE;

		enemyWk->rotDest.y = D3DX_PI + camera->rot.y;
	}
	// W：上( ↑ )
	else if (GetKeyboardPress(DIK_I))
	{
		enemyWk->move.x -= sinf(camera->rot.y) * VALUE_MOVE;
		enemyWk->move.z -= cosf(camera->rot.y) * VALUE_MOVE;

		enemyWk->rotDest.y = camera->rot.y;
	}

	// 目的の角度までの差分
	fDiffRotY = enemyWk->rotDest.y - enemyWk->rot.y;
	if (fDiffRotY > D3DX_PI)
	{
		fDiffRotY -= D3DX_PI * 2.0f;
	}
	if (fDiffRotY < -D3DX_PI)
	{
		fDiffRotY += D3DX_PI * 2.0f;
	}

	// 目的の角度まで慣性をかける
	enemyWk->rot.y += fDiffRotY * RATE_ROTATE_MODEL;
	if (enemyWk->rot.y > D3DX_PI)
	{
		enemyWk->rot.y -= D3DX_PI * 2.0f;
	}
	if (enemyWk->rot.y < -D3DX_PI)
	{
		enemyWk->rot.y += D3DX_PI * 2.0f;
	}

	/// 位置移動
	enemyWk->pos.x += enemyWk->move.x;
	enemyWk->pos.y += enemyWk->move.y;
	//if (enemyWk->pos.y < 5.0f)
	//{
	//	enemyWk->pos.y = 5.0f;
	//}
	//if (enemyWk->pos.y > 75.0f)
	//{
	//	enemyWk->pos.y = 75.0f;
	//}
	enemyWk->pos.z += enemyWk->move.z;

	// 移動量に慣性をかける
	enemyWk->move.x += (0.0f - enemyWk->move.x) * RATE_MOVE_MODEL;
	enemyWk->move.y += (0.0f - enemyWk->move.y) * RATE_MOVE_MODEL;
	enemyWk->move.z += (0.0f - enemyWk->move.z) * RATE_MOVE_MODEL;

	//モデルのジャンプ
	if (GetKeyboardTrigger(DIK_RETURN) && enemyWk->pos.y == 0.0f)
	{
		enemyWk->jump = true;
	}
	if (enemyWk->jump == true)
	{
		enemyWk->pos.y += enemyWk->speed;
		enemyWk->speed -= 0.98f;
		if (enemyWk->pos.y <= 0.0f)
		{
			enemyWk->speed = ENEMY_JUMP_SPEED;
			enemyWk->pos.y = 0.0f;
			enemyWk->jump = false;
		}
	}

	// 影の位置設定
	SetPositionShadow(enemyWk->IdxShadow, D3DXVECTOR3(enemyWk->pos.x, 0.1f, enemyWk->pos.z));
	SetVertexShadow(enemyWk->IdxShadow, enemyWk->SizeShadow, enemyWk->SizeShadow);
	SetColorShadow(enemyWk->IdxShadow, enemyWk->ColShadow);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DMATERIAL9 matDef;
	MATRIX matrix = *GetMatrix();

	ENEMY *enemyWk = GetEnemy(0);
	CAMERA *cameraWk = GetCamera(0);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&matrix.world);

	// スケールを反映
	D3DXMatrixScaling(&matrix.scale, enemyWk->scl.x, enemyWk->scl.y, enemyWk->scl.z);
	D3DXMatrixMultiply(&matrix.world, &matrix.world, &matrix.scale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&matrix.scale, enemyWk->rot.y, enemyWk->rot.x, enemyWk->rot.z);
	D3DXMatrixMultiply(&matrix.world, &matrix.world, &matrix.scale);

	// 移動を反映
	D3DXMatrixTranslation(&matrix.translation, enemyWk->pos.x, enemyWk->pos.y, enemyWk->pos.z);
	D3DXMatrixMultiply(&matrix.world, &matrix.world, &matrix.translation);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &matrix.world);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// エネミーのレンダリング
	EnemyModel->SetMatrix(&matrix.world);
	EnemyModel->Render();

#if 0
	D3DXMATERIAL *pD3DXMat;

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)enemyWk->D3DXBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)enemyWk->NumMat; nCntMat++)
	{

		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, enemyWk->D3DTexture[nCntMat]);

		// 描画
		enemyWk->D3DXMesh->DrawSubset(nCntMat);
	}

#endif

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
//エネミーの情報を取得する
//=============================================================================
ENEMY *GetEnemy(int pno)
{
	return &enemyWk[pno];
}