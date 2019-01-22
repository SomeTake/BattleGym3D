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
#include "enemy.h"
#include "debugproc.h"
#include "HitCheck.h"
#include "meshwall.h"
#include "particle.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
CHARA playerWk;

D3DXMATRIX WorldMtxPlayer;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 位置・回転・スケールの初期設定
	playerWk.pos = FIRST_PLAYER_POS;
	playerWk.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	playerWk.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk.HP = FULL_HP;
	playerWk.HPzan = playerWk.HP;
	playerWk.HitFrag = false;

	if (type == 0)
	{
		//オブジェクトの初期化
		playerWk.Animation = CreateAnimationObject();

		// xFileの読み込み
		if (FAILED(Load_xFile(playerWk.Animation, CHARA_XFILE, "Player")))
		{
			return E_FAIL;
		}

		// AnimationCallbackをセットする
		// 前歩き
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Frontwalk])))
		{
			return E_FAIL;
		}
		// 後ろ歩き
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Backwalk])))
		{
			return E_FAIL;
		}
		// 横移動
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Rightstep])))
		{
			return E_FAIL;
		}
		// 横移動
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Leftstep])))
		{
			return E_FAIL;
		}
		// ダメージ
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Damage])))
		{
			return E_FAIL;
		}
		// ダウン
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Down])))
		{
			return E_FAIL;
		}
		// ダウンポーズ
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Downpose])))
		{
			return E_FAIL;
		}
		// 起き上がり
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Getup])))
		{
			return E_FAIL;
		}
		// パンチ
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Punchi])))
		{
			return E_FAIL;
		}
		// キック
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Kick])))
		{
			return E_FAIL;
		}
		// 波動
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Hadou])))
		{
			return E_FAIL;
		}
		// 昇竜
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Shoryu])))
		{
			return E_FAIL;
		}
		// SP技
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[SPattack])))
		{
			return E_FAIL;
		}
		// 投げ
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Throw])))
		{
			return E_FAIL;
		}

		// AnimationSetを初期化する
		for (int i = 0; i < playerWk.Animation->AnimSetNum; i++)
		{
			if (FAILED(playerWk.Animation->InitAnimation(playerWk.Animation, CharaStateAnim[i], i)))
			{
				return E_FAIL;
			}
		}
		playerWk.Animation->CurrentAnimID = Idle;

		// アニメーション感の補完を設定
		playerWk.Animation->SetShiftTime(playerWk.Animation, Idle, 0.5f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Frontwalk, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Backwalk, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Rightstep, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Leftstep, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Guard, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Damage, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Down, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Downpose, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Getup, 1.0f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Punchi, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Kick, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Hadou, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Shoryu, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, SPattack, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Throw, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Win, 0.1f);

		// 当たり判定用ボールを生成
		D3DXMATRIX Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[Hips]);
		InitBall(0, &playerWk.HitBall[0], Mtx, BODY_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[Neck]);
		InitBall(0, &playerWk.HitBall[1], Mtx, BODY_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[Head]);
		InitBall(0, &playerWk.HitBall[2], Mtx, BODY_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[LeftShoulder]);
		InitBall(0, &playerWk.HitBall[3], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[RightShoulder]);
		InitBall(0, &playerWk.HitBall[4], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[LeftHand]);
		InitBall(0, &playerWk.HitBall[5], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[RightHand]);
		InitBall(0, &playerWk.HitBall[6], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[LeftFoot]);
		InitBall(0, &playerWk.HitBall[7], Mtx, FOOT_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[RightFoot]);
		InitBall(0, &playerWk.HitBall[8], Mtx, FOOT_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[LeftForeArm]);
		InitBall(0, &playerWk.HitBall[9], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[RightForeArm]);
		InitBall(0, &playerWk.HitBall[10], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[LeftLeg]);
		InitBall(0, &playerWk.HitBall[11], Mtx, FOOT_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[RightLeg]);
		InitBall(0, &playerWk.HitBall[12], Mtx, FOOT_RADIUS);

		// 影の生成
		playerWk.IdxShadow = CreateShadow(playerWk.pos, SHADOW_SIZE_X, SHADOW_SIZE_Z);
		playerWk.SizeShadow = 25.0f;
		playerWk.ColShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);	
	}

	// 初期アニメーションを待機にする
	playerWk.Animation->CurrentAnimID = Idle;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	// アニメーションをリリース
	playerWk.Animation->UninitAnimation(playerWk.Animation);
	
	// 当たり判定ボールをリリース
	for (int i = 0; i < HIT_CHECK_NUM; i++)
	{
		UninitBall(&playerWk.HitBall[i]);
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	int *Phase = GetPhase();
	CHARA *enemyWk = GetEnemy();

#ifdef _DEBUG
	// デバッグ用入力
	if (GetKeyboardTrigger(DIK_1))
	{
		playerWk.Animation->ChangeAnimation(playerWk.Animation, Down, 1.5f);
	}
	// プレイヤーHP0
	else if (GetKeyboardTrigger(DIK_2))
	{
		playerWk.HPzan = 0;
	}
	// エネミーHP0
	else if (GetKeyboardTrigger(DIK_3))
	{
		enemyWk->HPzan = 0;
	}
	else if (GetKeyboardTrigger(DIK_4))
	{
		playerWk.Animation->ChangeAnimation(playerWk.Animation, Damage, 1.5f);
	}

	// デバッグ表示
	PrintDebugProc("プレイヤー座標 X:%f Y:%f Z:%f\n", playerWk.pos.x, playerWk.pos.y, playerWk.pos.z);
	PrintDebugProc("プレイヤー角度 X:%f Y:%f Z:%f\n", playerWk.rot.x, playerWk.rot.y, playerWk.rot.z);
	PrintDebugProc("プレイヤーアニメーション番号 X:%d\n", playerWk.Animation->CurrentAnimID);
	PrintDebugProc("ワイヤーフレーム表示 キーボード:9 = OFF キーボード:0 = ON\n");
#endif

	// HP0&ダウンモーションが終了した場合、それ以上アニメーションを更新しない、操作できない
	if (playerWk.Animation->CurrentAnimID == Downpose && playerWk.HPzan == 0)
	{

	}
	else
	{
		// 簡単入力
		EasyInputPlayer();

		// 本格入力

		// アニメーションを更新
		playerWk.Animation->UpdateAnimation(playerWk.Animation, TIME_PER_FRAME);

		// 勝利時
		if (*Phase == PhaseFinish && playerWk.HPzan > enemyWk->HPzan && playerWk.Animation->CurrentAnimID == Idle)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Win, 1.5f);
		}
		// 敗北時HP0になったらダウン
		if (playerWk.HPzan <= 0)
		{
			playerWk.HPzan = 0;
			// 強制的にアニメーション変更
			if (playerWk.Animation->CurrentAnimID != Downpose && playerWk.Animation->CurrentAnimID != Down)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Down, 0.5f);
			}
			else if (playerWk.Animation->CurrentAnimID == Down && playerWk.Animation->MotionEnd == true)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Down, ANIM_SPD_1);
			}
			SetPhase(PhaseFinish);
		}
	}

	// 座標移動
	MovePlayer();
	
	// 当たり判定用ボール座標の更新
	D3DXMATRIX Mtx;
	for (int i = 0; i < HIT_CHECK_NUM; i++)
	{
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[i]);
		UpdateBall(&playerWk.HitBall[i], Mtx);
	}

	// 当たり判定
	if (playerWk.HitFrag == false)
	{
		HitCheckPlayer();
	}

	// 影の位置設定
	SetPositionShadow(playerWk.IdxShadow, D3DXVECTOR3(playerWk.pos.x, 0.1f, playerWk.pos.z));
	SetVertexShadow(playerWk.IdxShadow, playerWk.SizeShadow, playerWk.SizeShadow);
	SetColorShadow(playerWk.IdxShadow, playerWk.ColShadow);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DMATERIAL9 matDef;
	D3DXMATRIX ScaleMatrix, RotMatrix, TransMatrix, CapsuleMatrix, BallMatrix;
	bool RenderState = GetRenderState();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&WorldMtxPlayer);

	// スケールを反映
	D3DXMatrixScaling(&ScaleMatrix, playerWk.scl.x, playerWk.scl.y, playerWk.scl.z);
	D3DXMatrixMultiply(&WorldMtxPlayer, &WorldMtxPlayer, &ScaleMatrix);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&ScaleMatrix, playerWk.rot.y, playerWk.rot.x, playerWk.rot.z);
	D3DXMatrixMultiply(&WorldMtxPlayer, &WorldMtxPlayer, &ScaleMatrix);

	// 移動を反映
	D3DXMatrixTranslation(&TransMatrix, playerWk.pos.x, playerWk.pos.y, playerWk.pos.z);
	D3DXMatrixMultiply(&WorldMtxPlayer, &WorldMtxPlayer, &TransMatrix);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &WorldMtxPlayer);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// 描画モードをワイヤーフレームに切り替える
	if (RenderState == true)
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}

	// レンダリング
	playerWk.Animation->DrawAnimation(playerWk.Animation, &WorldMtxPlayer);

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

	if (RenderState == true)
	{
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			// プレイヤーの当たり判定用ボールを描画する
			DrawBall(&playerWk.HitBall[i]);
		}
	}
}

//=============================================================================
//プレイヤーの情報を取得する
//=============================================================================
CHARA *GetPlayer(void)
{
	return &playerWk;
}

//=============================================================================
//簡単入力（アニメーションの管理も）
//=============================================================================
void EasyInputPlayer(void)
{
	switch (playerWk.Animation->CurrentAnimID)
	{
	case Idle:
		// 移動
		// 前
		if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_RIGHT) || IsButtonTriggered(0, STICK_RIGHT))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Frontwalk, ANIM_SPD_2);
		}
		// 後ろ
		else if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_LEFT) || IsButtonTriggered(0, STICK_LEFT))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Backwalk, ANIM_SPD_2);
		}
		// 手前
		else if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN) || IsButtonTriggered(0, STICK_DOWN))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Rightstep, ANIM_SPD_2);
		}
		// 奥
		else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, STICK_UP))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Leftstep, ANIM_SPD_2);
		}

		// 攻撃
		// パンチ
		else if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi, ANIM_SPD_15);
		}
		// キック
		else if (GetKeyboardTrigger(DIK_M) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick, ANIM_SPD_15);
		}
		// 波動拳
		else if (GetKeyboardTrigger(DIK_N) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Hadou, ANIM_SPD_2);
		}
		// 昇竜拳
		else if (GetKeyboardTrigger(DIK_H) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Shoryu, ANIM_SPD_15);
		}
		// ガード
		else if (GetKeyboardTrigger(DIK_G) || IsButtonTriggered(0, BUTTON_Y))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Guard, ANIM_SPD_1);
		}
		else
		{
		}
		break;
	case Frontwalk:
		// 前
		if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, STICK_RIGHT))
		{

		}
		// ボタンリリースで待機に戻る
		else
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
		}

		// 攻撃
		// パンチ
		if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi, ANIM_SPD_15);
		}
		// キック
		else if (GetKeyboardTrigger(DIK_M) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick, ANIM_SPD_15);
		}
		// 波動拳
		else if (GetKeyboardTrigger(DIK_N) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Hadou, ANIM_SPD_2);
		}
		// 昇竜拳
		else if (GetKeyboardTrigger(DIK_H) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Shoryu, ANIM_SPD_15);
		}
		// ガード
		else if (GetKeyboardTrigger(DIK_G) || IsButtonTriggered(0, BUTTON_Y))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Guard, ANIM_SPD_1);
		}
		break;
	case Backwalk:
		// 後ろ
		if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, STICK_LEFT))
		{

		}
		// ボタンリリースで待機に戻る
		else
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
		}

		// 攻撃
		// パンチ
		if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi, ANIM_SPD_15);
		}
		// キック
		else if (GetKeyboardTrigger(DIK_M) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick, ANIM_SPD_15);
		}
		// 波動拳
		else if (GetKeyboardTrigger(DIK_N) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Hadou, ANIM_SPD_2);
		}
		// 昇竜拳
		else if (GetKeyboardTrigger(DIK_H) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Shoryu, ANIM_SPD_15);
		}
		// ガード
		else if (GetKeyboardTrigger(DIK_G) || IsButtonTriggered(0, BUTTON_Y))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Guard, ANIM_SPD_1);
		}
		break;
	case Rightstep:
		// アニメーション終了で待機に戻る
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
		}
		break;
	case Leftstep:
		// アニメーション終了で待機に戻る
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
		}
		break;
	case Guard:
		if (GetKeyboardPress(DIK_G) || IsButtonPressed(0, BUTTON_Y))
		{

		}
			// ボタンリリースで待機に戻る
		else
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
		}

		// 攻撃
		// パンチ
		if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi, ANIM_SPD_15);
		}
		// キック
		else if (GetKeyboardTrigger(DIK_M) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick, ANIM_SPD_15);
		}
		// 波動拳
		else if (GetKeyboardTrigger(DIK_N) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Hadou, ANIM_SPD_2);
		}
		// 昇竜拳
		else if (GetKeyboardTrigger(DIK_H) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Shoryu, ANIM_SPD_15);
		}
	case Damage:
		// アニメーション終了で待機に戻る
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
		}
		break;
	case Down:
		// アニメーション終了で起き上がりに移行
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Downpose, ANIM_SPD_1);
			// プレイヤー座標をモーション座標に合わせる
			D3DXMATRIXA16 newmatrix;	// モーション座標を取得するための行列
			newmatrix = GetBoneMatrix(playerWk.Animation, "Hips");
			playerWk.pos = D3DXVECTOR3(newmatrix._41, 0.0f, newmatrix._43);
		}
		break;
	case Downpose:
		// アニメーション終了で起き上がりに移行
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Getup, ANIM_SPD_15);
		}
	case Getup:
		// アニメーション終了で待機に戻る
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
		}
		break;
	case Punchi:
		// アニメーション終了で待機に戻る
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
			playerWk.HitFrag = false;
		}
		break;
	case Kick:
		// アニメーション終了で待機に戻る
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
			playerWk.HitFrag = false;
		}
		break;
	case Hadou:
		// アニメーション終了で待機に戻る
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
			playerWk.HitFrag = false;
		}
		break;
	case Shoryu:
		// アニメーション終了で待機に戻る
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, ANIM_SPD_1);
			playerWk.HitFrag = false;
		}
		break;
	default:
		break;
	}

}

//=============================================================================
//座標移動
//=============================================================================
void MovePlayer(void)
{
	D3DXVECTOR3 centerpos = GetCenterPos();
	CHARA *enemyWk = GetEnemy();
	float PEdistance = GetPEdistance();

	// アクションに合わせた座標移動
	switch (playerWk.Animation->CurrentAnimID)
	{
		// 前移動中の座標処理
	case Frontwalk:
		playerWk.move.x -= sinf(playerWk.rot.y) * VALUE_FRONTWALK;
		playerWk.move.z -= cosf(playerWk.rot.y) * VALUE_FRONTWALK;
		// 相手に接触していた場合、相手を押す
		if (PEdistance <= MIN_DISTANCE)
		{
			enemyWk->move.x -= sinf(enemyWk->rot.y + D3DX_PI) * VALUE_FRONTWALK;
			enemyWk->move.z -= cosf(enemyWk->rot.y + D3DX_PI) * VALUE_FRONTWALK;
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
			playerWk.move.x -= sinf(playerWk.rot.y + D3DX_PI) * VALUE_BACKWALK;
			playerWk.move.z -= cosf(playerWk.rot.y + D3DX_PI) * VALUE_BACKWALK;
		}
		break;
		// 手前移動中の座標処理
	case Rightstep:
		playerWk.move.x -= sinf(playerWk.rot.y + D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		playerWk.move.z -= cosf(playerWk.rot.y + D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		break;
		// 奥移動中の座標処理
	case Leftstep:
		playerWk.move.x -= sinf(playerWk.rot.y - D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		playerWk.move.z -= cosf(playerWk.rot.y - D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		break;
	default:
		break;
	}

	// 攻撃モーション時以外に中心を向く
	if (playerWk.Animation->CurrentAnimID == Punchi || playerWk.Animation->CurrentAnimID == Kick 
		|| playerWk.Animation->CurrentAnimID == Hadou || playerWk.Animation->CurrentAnimID == Shoryu)
	{
	}
	else
	{
		playerWk.rot.y = atan2f(centerpos.x - playerWk.pos.x, centerpos.z - playerWk.pos.z) + D3DX_PI;
	}

	// 位置移動
	playerWk.pos.x += playerWk.move.x;
	playerWk.pos.y += playerWk.move.y;
	playerWk.pos.z += playerWk.move.z;

	// (半径*角度)＋基準座標でプレイヤーの座標を計算する

	// 移動量をリセットする
	playerWk.move.x = 0.0f;
	playerWk.move.y = 0.0f;
	playerWk.move.z = 0.0f;	

	// 移動中のエフェクトの発生
	if (playerWk.Animation->CurrentAnimID == Frontwalk || playerWk.Animation->CurrentAnimID == Backwalk ||
		playerWk.Animation->CurrentAnimID == Rightstep || playerWk.Animation->CurrentAnimID == Leftstep)
	{
		D3DXVECTOR3 pos;

		pos.x = playerWk.pos.x + sinf(playerWk.rot.y) * 10.0f;
		pos.y = playerWk.pos.y + 2.0f;
		pos.z = playerWk.pos.z + cosf(playerWk.rot.y) * 10.0f;

		SetParticle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.85f, 0.05f, 0.65f, 0.50f), 14.0f, 14.0f, 360);
		SetParticle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.65f, 0.85f, 0.05f, 0.30f), 10.0f, 10.0f, 360);
		SetParticle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.45f, 0.45f, 0.05f, 0.15f), 5.0f, 5.0f, 360);
	}

}

//=============================================================================
//当たり判定
//=============================================================================
void HitCheckPlayer(void)
{
	CHARA *enemyWk = GetEnemy();
	D3DXVECTOR3 FirePos;		// 波動拳発射位置

	switch (playerWk.Animation->CurrentAnimID)
	{
	case Punchi:
		// 左手と相手の各部位との判定
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			if (hitBC(playerWk.HitBall[LeftHand].pos, enemyWk->HitBall[i].pos, playerWk.HitBall[LeftHand].scl.x, enemyWk->HitBall[i].scl.x) == true)
			{
				HitAction(&playerWk, enemyWk);
				break;
			}
		}
		break;
	case Kick:
		// 右足と相手の各部位との判定
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			if (hitBC(playerWk.HitBall[RightFoot].pos, enemyWk->HitBall[i].pos, playerWk.HitBall[RightFoot].scl.x, enemyWk->HitBall[i].scl.x) == true)
			{
				HitAction(&playerWk, enemyWk);
				break;
			}
		}
		break;
	case Hadou:
		// 弾の発射（右手と左手の間から出す）
		FirePos = (playerWk.HitBall[RightHand].pos + playerWk.HitBall[LeftHand].pos) * 0.5f;
		playerWk.HitFrag = true;
		break;
	case Shoryu:
		// 両足と相手の各部位との判定
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			// 左足との判定
			if (hitBC(playerWk.HitBall[LeftFoot].pos, enemyWk->HitBall[i].pos, playerWk.HitBall[LeftFoot].scl.x, enemyWk->HitBall[i].scl.x) == true)
			{
				HitAction(&playerWk, enemyWk);
				break;
			}
			// 右足との判定
			else if (hitBC(playerWk.HitBall[RightFoot].pos, enemyWk->HitBall[i].pos, playerWk.HitBall[RightFoot].scl.x, enemyWk->HitBall[i].scl.x) == true)
			{
				HitAction(&playerWk, enemyWk);
				break;
			}
		}
		break;

	default:
		break;
	}

}
