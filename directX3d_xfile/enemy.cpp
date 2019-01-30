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
#include "player.h"
#include "meshwall.h"
#include "particle.h"
#include "knockout.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
CHARA enemyWk;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 位置・回転・スケールの初期設定
	enemyWk.pos = FIRST_ENEMY_POS;
	enemyWk.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	enemyWk.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	enemyWk.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ステータス等の初期設定
	enemyWk.HP = FULL_HP;
	enemyWk.HPzan = enemyWk.HP;
	enemyWk.SP = 0;
	enemyWk.HitFrag = false;
	enemyWk.score = 0;
	enemyWk.graceflag = false;
	enemyWk.graceframe = 0;
	enemyWk.gracetype = Idle;

	if (type == 0)
	{
		//オブジェクトの初期化
		enemyWk.Animation = CreateAnimationObject();

		// xFileの読み込み
		if (FAILED(Load_xFile(enemyWk.Animation, CHARA_XFILE, "enemy")))
		{
			return E_FAIL;
		}

		// 横移動
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Rightstep])))
		{
			return E_FAIL;
		}
		// 横移動
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Leftstep])))
		{
			return E_FAIL;
		}
		// ダメージ
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Damage])))
		{
			return E_FAIL;
		}
		// ダウン
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Down])))
		{
			return E_FAIL;
		}
		// ダウン状態
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Downpose])))
		{
			return E_FAIL;
		}
		// 起き上がり
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Getup])))
		{
			return E_FAIL;
		}
		// パンチ
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Punchi])))
		{
			return E_FAIL;
		}
		// キック
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Kick])))
		{
			return E_FAIL;
		}
		// 波動
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Hadou])))
		{
			return E_FAIL;
		}
		// 昇竜
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Shoryu])))
		{
			return E_FAIL;
		}
		// SP技
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[SPattack])))
		{
			return E_FAIL;
		}
		// 投げ
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Throw])))
		{
			return E_FAIL;
		}

		// AnimationSetを初期化する
		for (int i = 0; i < enemyWk.Animation->AnimSetNum; i++)
		{
			if (FAILED(enemyWk.Animation->InitAnimation(enemyWk.Animation, CharaStateAnim[i], i)))
			{
				return E_FAIL;
			}
		}
		enemyWk.Animation->CurrentAnimID = Idle;
		
		// アニメーション間の補完を設定
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Idle, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Frontwalk, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Backwalk, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Rightstep, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Leftstep, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Guard, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Damage, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Down, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Downpose, 1.0f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Getup, 1.0f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Punchi, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Kick, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Hadou, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Shoryu, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, SPattack, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Throw, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Win, 0.1f);

		// 当たり判定用ボールを生成
		D3DXMATRIX Mtx = GetBoneMatrix(enemyWk.Animation, "Hips");
		InitBall(0, &enemyWk.HitBall[0], Mtx, BODY_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "Neck");
		InitBall(0, &enemyWk.HitBall[1], Mtx, BODY_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "Head");
		InitBall(0, &enemyWk.HitBall[2], Mtx, BODY_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "LeftShoulder");
		InitBall(0, &enemyWk.HitBall[3], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "RightShoulder");
		InitBall(0, &enemyWk.HitBall[4], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "LeftHand");
		InitBall(0, &enemyWk.HitBall[5], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "RightHand");
		InitBall(0, &enemyWk.HitBall[6], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "LeftFoot");
		InitBall(0, &enemyWk.HitBall[7], Mtx, FOOT_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "RightFoot");
		InitBall(0, &enemyWk.HitBall[8], Mtx, FOOT_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "LeftForeArm");
		InitBall(0, &enemyWk.HitBall[9], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "RightForeArm");
		InitBall(0, &enemyWk.HitBall[10], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "LeftLeg");
		InitBall(0, &enemyWk.HitBall[11], Mtx, FOOT_RADIUS);
		Mtx = GetBoneMatrix(enemyWk.Animation, "RightLeg");
		InitBall(0, &enemyWk.HitBall[12], Mtx, FOOT_RADIUS);

		// 影の生成
		enemyWk.IdxShadow = CreateShadow(enemyWk.pos, SHADOW_SIZE_X, SHADOW_SIZE_Z);
		enemyWk.SizeShadow = 25.0f;
		enemyWk.ColShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	}
	else
	{
		enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle, ANIM_SPD_1);
	}

	// 波動拳用バレットをセット
	InitHadou(type, &enemyWk.HadouBullet);

	// 2P表示のビルボードを作成
	InitPop(type, &enemyWk.Popup, 1);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	// アニメーションをリリース
	enemyWk.Animation->UninitAnimation(enemyWk.Animation);

	// 当たり判定ボールをリリース
	for (int i = 0; i < HIT_CHECK_NUM; i++)
	{
		UninitBall(&enemyWk.HitBall[i]);
	}

	// 波動拳用バレットをリリース
	UninitHadou(&enemyWk.HadouBullet);

	// 2P表示をリリース
	UninitPop(&enemyWk.Popup);

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	int *Phase = GetPhase();
	CHARA *playerWk = GetPlayer();
	bool ko = GetKnockout()->pushok;

#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("エネミー座標 X:%f Y:%f Z:%f\n", enemyWk.pos.x, enemyWk.pos.y, enemyWk.pos.z);
	PrintDebugProc("エネミー角度 X:%f Y:%f Z:%f\n", enemyWk.rot.x, enemyWk.rot.y, enemyWk.rot.z);
	PrintDebugProc("エネミーアニメーション番号 No:%d\n", enemyWk.Animation->CurrentAnimID);
#endif

	if (*Phase == PhaseGame)
	{
		// 簡単入力
		EasyInput(&enemyWk, 1);
	}

	// 本格入力

	// KO表示中は更新しない
	if (*Phase == PhaseFinish && ko == false)
	{

	}
	else
	{
		// アニメーションを更新
		enemyWk.Animation->UpdateAnimation(enemyWk.Animation, TIME_PER_FRAME);
	}

	// 勝利時
	if (*Phase == PhaseFinish && enemyWk.HPzan > playerWk->HPzan && enemyWk.Animation->CurrentAnimID == Idle)
	{
		enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Win, 1.5f);
	}
	// 敗北時HP0になったらダウン
	if (enemyWk.HPzan <= 0)
	{
		enemyWk.HPzan = 0;
		// 強制的にアニメーション変更
		if (enemyWk.Animation->CurrentAnimID != Downpose)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Downpose, 0.5f);
		}
		SetPhase(PhaseFinish);
	}

	// 座標移動
	MoveEnemy();

	// 当たり判定用ボール座標の更新
	D3DXMATRIX Mtx;
	for (int i = 0; i < HIT_CHECK_NUM; i++)
	{
		Mtx = GetBoneMatrix(enemyWk.Animation, CharaHitPos[i]);
		UpdateBall(&enemyWk.HitBall[i], Mtx);
	}

	// 当たり判定
	if (enemyWk.HitFrag == false)
	{
		// キャラクター同士の当たり判定
		if (HitCheckCToC(&enemyWk, playerWk) == true)
		{
			// 当たった後の動き
			HitAction(&enemyWk, playerWk);
		}
	}

	if (enemyWk.HadouBullet.use == true)
	{
		// 波動拳用バレットの更新
		UpdateHadou(&enemyWk.HadouBullet);
		
		// 波動拳の当たり判定
		HitHadou(&enemyWk, playerWk);
	}

	// 2P表示の位置更新
	UpdatePop(&enemyWk.Popup, enemyWk.HitBall[Hips].pos);

	// 影の位置設定
	SetPositionShadow(enemyWk.IdxShadow, D3DXVECTOR3(enemyWk.pos.x, 0.1f, enemyWk.pos.z));
	SetVertexShadow(enemyWk.IdxShadow, enemyWk.SizeShadow, enemyWk.SizeShadow);
	SetColorShadow(enemyWk.IdxShadow, enemyWk.ColShadow);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DMATERIAL9 matDef;
	MATRIX matrix = *GetMatrix();
	D3DXMATRIX CapsuleMatrix;
	bool RenderState = GetRenderState();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&matrix.world);

	// スケールを反映
	D3DXMatrixScaling(&matrix.scale, enemyWk.scl.x, enemyWk.scl.y, enemyWk.scl.z);
	D3DXMatrixMultiply(&matrix.world, &matrix.world, &matrix.scale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&matrix.scale, enemyWk.rot.y, enemyWk.rot.x, enemyWk.rot.z);
	D3DXMatrixMultiply(&matrix.world, &matrix.world, &matrix.scale);

	// 移動を反映
	D3DXMatrixTranslation(&matrix.translation, enemyWk.pos.x, enemyWk.pos.y, enemyWk.pos.z);
	D3DXMatrixMultiply(&matrix.world, &matrix.world, &matrix.translation);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &matrix.world);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// 描画モードをワイヤーフレームに切り替える
	if (RenderState == true)
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}

	// レンダリング
	enemyWk.Animation->DrawAnimation(enemyWk.Animation, &matrix.world);

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

	if (RenderState == true)
	{
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			// プレイヤーの当たり判定用ボールを描画する
			DrawBall(&enemyWk.HitBall[i]);
		}
	}

	if (enemyWk.HadouBullet.use == true)
	{
		// 波動拳用バレットの描画
		DrawHadou(&enemyWk.HadouBullet);
	}

	// 2P表示用ビルボードを描画
	DrawPop(&enemyWk.Popup);
}

//=============================================================================
//エネミーの情報を取得する
//=============================================================================
CHARA *GetEnemy(void)
{
	return &enemyWk;
}

//=============================================================================
//座標移動
//=============================================================================
void MoveEnemy(void)
{
	CAMERA *camera = GetCamera(0);
	D3DXVECTOR3 centerpos = GetCenterPos();
	CHARA *playerWk = GetPlayer();
	D3DXVECTOR3		oldPos = enemyWk.pos;		// 元の位置
	float PEdistance = GetPEdistance();

	// アクションに合わせた座標移動
	switch (enemyWk.Animation->CurrentAnimID)
	{
		// 前移動中の座標処理
	case Frontwalk:
		enemyWk.move.x -= sinf(enemyWk.rot.y) * VALUE_FRONTWALK;
		enemyWk.move.z -= cosf(enemyWk.rot.y) * VALUE_FRONTWALK;
		// 相手に接触していた場合、相手を押す
		if (PEdistance <= MIN_DISTANCE)
		{
			playerWk->move.x -= sinf(playerWk->rot.y + D3DX_PI) * VALUE_FRONTWALK;
			playerWk->move.z -= cosf(playerWk->rot.y + D3DX_PI) * VALUE_FRONTWALK;
		}
		break;
		// 後移動中の座標処理
	case Backwalk:
		// フィールド外には出られない
		// 相手から一定距離離れたらそれ以上離れられない
		if (PEdistance >= MAX_DISTANCE)
		{

		}
		else
		{
			enemyWk.move.x -= sinf(enemyWk.rot.y + D3DX_PI) * VALUE_BACKWALK;
			enemyWk.move.z -= cosf(enemyWk.rot.y + D3DX_PI) * VALUE_BACKWALK;
		}
		break;
		// 手前移動中の座標処理
	case Rightstep:
		enemyWk.move.x -= sinf(enemyWk.rot.y - D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		enemyWk.move.z -= cosf(enemyWk.rot.y - D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		break;
		// 奥移動中の座標処理
	case Leftstep:
		enemyWk.move.x -= sinf(enemyWk.rot.y + D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		enemyWk.move.z -= cosf(enemyWk.rot.y + D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		break;
	default:
		break;
	}

	// 攻撃時以外常に中心を向く
	if (enemyWk.Animation->CurrentAnimID == Punchi || enemyWk.Animation->CurrentAnimID == Kick ||
		enemyWk.Animation->CurrentAnimID == Hadou || enemyWk.Animation->CurrentAnimID == Shoryu)
	{

	}
	else
	{
		enemyWk.rot.y = atan2f(centerpos.x - enemyWk.pos.x, centerpos.z - enemyWk.pos.z) + D3DX_PI;
	}

	// 位置移動
	enemyWk.pos.x += enemyWk.move.x;
	enemyWk.pos.y += enemyWk.move.y;
	enemyWk.pos.z += enemyWk.move.z;

	// 移動量をリセットする
	enemyWk.move.x = 0;
	enemyWk.move.y = 0;
	enemyWk.move.z = 0;


}
