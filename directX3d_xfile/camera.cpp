//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "main.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "debugproc.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
#define VIEW_DIST			(4)		// 最低限のカメラの距離
#define VIEW_DIST_RATE		(0.3f)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
CAMERA cameraWk[MAX_SEPARATE];

D3DXVECTOR3 CenterPos;				// プレイヤーとエネミーの間の位置
float PEdistance;					// プレイヤーとエネミーの距離

//=============================================================================
// カメラの初期化処理
//=============================================================================
void InitCamera(void)
{
	PLAYER *playerWk = GetPlayer();
	ENEMY *enemyWk = GetEnemy();
	CAMERA *cameraWk = GetCamera(0);

	CenterPos = (playerWk->pos + enemyWk->pos) * 0.5f;
	cameraWk->pos = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
	cameraWk->at = CenterPos + D3DXVECTOR3(AT_X_CAM, AT_Y_CAM, AT_Z_CAM);

	cameraWk->up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	cameraWk->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 注視点からカメラの位置までの距離計測
	float vx, vz;
	vx = cameraWk->pos.x - cameraWk->at.x;
	vz = cameraWk->pos.z - cameraWk->at.z;
	cameraWk->distance = sqrtf(vx * vx + vz * vz);

}

//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
	PLAYER *playerWk = GetPlayer();
	ENEMY *enemyWk = GetEnemy();
	CAMERA *cameraWk = GetCamera(0);

	// PとEの中心位置
	CenterPos = (playerWk->pos + enemyWk->pos) * 0.5f;

	// PとEの距離計測
	D3DXVECTOR3 unit = playerWk->pos - enemyWk->pos;	// PE間のベクトル
	PEdistance = D3DXVec3Length(&unit);					// PE間の距離
	D3DXVec3Normalize(&unit, &unit);					// 正規化する

	// カメラの注視点と位置の距離指定
	if (PEdistance <= POS_Z_CAM)
	{
		cameraWk->distance = POS_Z_CAM;
	}
	else
	{
		cameraWk->distance = PEdistance;
	}

	// PとEの座標の中点から、PとEの座標をそれぞれ結んだ線分に対して垂直に移動した位置をカメラ位置とする
	D3DXVECTOR3 ViewFrom = D3DXVECTOR3(CenterPos.x - unit.z * cameraWk->distance, AT_Y_CAM + POS_Y_CAM, CenterPos.z + unit.x * cameraWk->distance);
	cameraWk->pos = ViewFrom;

	cameraWk->rot.y = atan2f(cameraWk->at.x - cameraWk->pos.x, cameraWk->at.z - cameraWk->pos.z) * D3DX_PI;	// カメラの回転（常に注視点を向き続ける）
	cameraWk->at = CenterPos + D3DXVECTOR3(AT_X_CAM, AT_Y_CAM, AT_Z_CAM);									// カメラの注視点＝モデルの中心点
	cameraWk->up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);															// 3D空間の上方向はどちら？＝Yが＋方向が上

#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("カメラ座標 X:%f Y:%f Z:%f\n", cameraWk->pos.x, cameraWk->pos.y, cameraWk->pos.z);
	PrintDebugProc("カメラ角度 X:%f Y:%f Z:%f\n", cameraWk->rot.x, cameraWk->rot.y, cameraWk->rot.z);
	PrintDebugProc("プレイヤー間の距離 %f\n", PEdistance);
#endif
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetCamera(int no)
{
	CAMERA *cameraWk = GetCamera(no);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	float asp = GetAspect();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&cameraWk->mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&cameraWk->mtxView,
		&cameraWk->pos,		// カメラの視点
		&cameraWk->at,		// カメラの注視点
		&cameraWk->up);		// カメラの上方向ベクトル

							// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &cameraWk->mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&cameraWk->mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&cameraWk->mtxProjection,
		VIEW_ANGLE,			// ビュー平面の視野角
		asp,				// ビュー平面のアスペクト比
		VIEW_NEAR_Z,		// ビュー平面のNearZ値
		VIEW_FAR_Z);		// ビュー平面のFarZ値

							// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &cameraWk->mtxProjection);

}

//=============================================================================
//カメラの情報を取得する
//=============================================================================
CAMERA *GetCamera(int cno)
{
	return &cameraWk[cno];
}

//=============================================================================
//PとEのの中心位置を取得する
//=============================================================================
D3DXVECTOR3 GetCenterPos(void)
{
	return CenterPos;
}

//=============================================================================
//PとEの距離を取得する
//=============================================================================
float GetPEdistance(void)
{
	return PEdistance;
}