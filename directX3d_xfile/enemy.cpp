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
#include "debugproc.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
ENEMY enemyWk;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ModelIndex = 0;

	// 位置・回転・スケールの初期設定
	enemyWk.pos = FIRST_ENEMY_POS;
	enemyWk.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	enemyWk.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	enemyWk.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	enemyWk.HP = FULL_HP;
	enemyWk.HPzan = enemyWk.HP;

	if (type == 0)
	{
		//オブジェクトの初期化
		enemyWk.Animation = CreateAnimationObject();

		// xFileの読み込み
		if (FAILED(Load_xFile(enemyWk.Animation, ENEMY_XFILE, "enemy")))
		{
			return E_FAIL;
		}

		// AnimationCallbackをセットする

		// AnimationSetを初期化する

		// 影の生成
		enemyWk.IdxShadow = CreateShadow(enemyWk.pos, SHADOW_SIZE_X, SHADOW_SIZE_Z);
		enemyWk.SizeShadow = 25.0f;
		enemyWk.ColShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	// アニメーションをリリース
	enemyWk.Animation->UninitAnimation(enemyWk.Animation);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	CAMERA *camera = GetCamera(1);
	D3DXVECTOR3 centerpos = GetCenterPos();

	// アニメーションを更新
	enemyWk.Animation->UpdateAnimation(enemyWk.Animation, TIME_PER_FRAME);

	//モデルの移動
	// D：右( → )
	if (GetKeyboardPress(DIK_L))
	{
		// D + S：右下( →・↓ )
		if (GetKeyboardPress(DIK_K))
		{
			enemyWk.move.x -= sinf(camera->rot.y + D3DX_PI * 0.75f) * VALUE_MOVE;
			enemyWk.move.z -= cosf(camera->rot.y + D3DX_PI * 0.75f) * VALUE_MOVE;
		}
		// D + W：右上( →・↑ )
		else if (GetKeyboardPress(DIK_I))
		{
			enemyWk.move.x -= sinf(camera->rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;
			enemyWk.move.z -= cosf(camera->rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;
		}
		// D：右( → )
		else
		{
			enemyWk.move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * VALUE_MOVE;
			enemyWk.move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * VALUE_MOVE;
		}
	}
	// A：左( ← )
	else if (GetKeyboardPress(DIK_J))
	{
		// A + S：左下( ←・↓ )
		if (GetKeyboardPress(DIK_K))
		{
			enemyWk.move.x -= sinf(camera->rot.y - D3DX_PI * 0.75f) * VALUE_MOVE;
			enemyWk.move.z -= cosf(camera->rot.y - D3DX_PI * 0.75f) * VALUE_MOVE;
		}
		// A + W：左上( ←・↑ )
		else if (GetKeyboardPress(DIK_I))
		{
			enemyWk.move.x -= sinf(camera->rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;
			enemyWk.move.z -= cosf(camera->rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;
		}
		// A：左( ← )
		else
		{
			enemyWk.move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * VALUE_MOVE;
			enemyWk.move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * VALUE_MOVE;
		}
	}
	// S：下( ↓ )
	else if (GetKeyboardPress(DIK_K))
	{
		enemyWk.move.x -= sinf(D3DX_PI + camera->rot.y) * VALUE_MOVE;
		enemyWk.move.z -= cosf(D3DX_PI + camera->rot.y) * VALUE_MOVE;
	}
	// W：上( ↑ )
	else if (GetKeyboardPress(DIK_I))
	{
		enemyWk.move.x -= sinf(camera->rot.y) * VALUE_MOVE;
		enemyWk.move.z -= cosf(camera->rot.y) * VALUE_MOVE;
	}

	// 敗北時
	if (enemyWk.HPzan <= 0)
	{
		enemyWk.HPzan = 0;
		SetPhase(PhaseFinish);
	}

	// 攻撃時以外常に中心を向く
	//if(enemyWk.Animation->CurrentAnimID == Punchi_E || enemyWk.Animation->CurrentAnimID == Kick_E || 
	//	enemyWk.Animation->CurrentAnimID == Hadou_E || enemyWk.Animation->CurrentAnimID == Shoryu_E)
	enemyWk.rot.y = atan2f(centerpos.x - enemyWk.pos.x, centerpos.z - enemyWk.pos.z) + D3DX_PI;

	// 位置移動
	enemyWk.pos.x += enemyWk.move.x;
	enemyWk.pos.y += enemyWk.move.y;
	enemyWk.pos.z += enemyWk.move.z;

	// 移動量に慣性をかける
	enemyWk.move.x += (0.0f - enemyWk.move.x) * RATE_MOVE_MODEL;
	enemyWk.move.y += (0.0f - enemyWk.move.y) * RATE_MOVE_MODEL;
	enemyWk.move.z += (0.0f - enemyWk.move.z) * RATE_MOVE_MODEL;

#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("エネミー座標 X:%f Y:%f Z:%f\n", enemyWk.pos.x, enemyWk.pos.y, enemyWk.pos.z);
	PrintDebugProc("エネミー角度 X:%f Y:%f Z:%f\n", enemyWk.rot.x, enemyWk.rot.y, enemyWk.rot.z);
#endif

	// 影の位置設定
	SetPositionShadow(enemyWk.IdxShadow, D3DXVECTOR3(enemyWk.pos.x, 0.1f, enemyWk.pos.z));
	SetVertexShadow(enemyWk.IdxShadow, enemyWk.SizeShadow, enemyWk.SizeShadow);
	SetColorShadow(enemyWk.IdxShadow, enemyWk.ColShadow);
}

D3DXMATRIX g_scale;		// スケールマトリクス
D3DXMATRIX g_rotation;	// ローテーションマトリクス
D3DXMATRIX g_translation;	// トランスレーションマトリクス
D3DXMATRIX g_view;		// ビューマトリクス
D3DXMATRIX g_projection;	// プロジェクションマトリクス
D3DXMATRIX g_world;		// ワールドマトリクス

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DMATERIAL9 matDef;
	//MATRIX matrix = *GetMatrix();

	CAMERA *cameraWk = GetCamera(0);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_world);

	// スケールを反映
	D3DXMatrixScaling(&g_scale, enemyWk.scl.x, enemyWk.scl.y, enemyWk.scl.z);
	D3DXMatrixMultiply(&g_world, &g_world, &g_scale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&g_scale, enemyWk.rot.y, enemyWk.rot.x, enemyWk.rot.z);
	D3DXMatrixMultiply(&g_world, &g_world, &g_scale);

	// 移動を反映
	D3DXMatrixTranslation(&g_translation, enemyWk.pos.x, enemyWk.pos.y, enemyWk.pos.z);
	D3DXMatrixMultiply(&g_world, &g_world, &g_translation);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_world);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// レンダリング
	enemyWk.Animation->DrawAnimation(enemyWk.Animation, &g_world);

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
//エネミーの情報を取得する
//=============================================================================
ENEMY *GetEnemy(void)
{
	return &enemyWk;
}