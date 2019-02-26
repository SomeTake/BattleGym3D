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
#include "D3DXAnimation.h"
#include "enemy.h"
#include "debugproc.h"
#include "meshwall.h"
#include "particle.h"
#include "knockout.h"
#include "redgauge.h"
#include "sound.h"
#include "shadow.h"
#include "game.h"

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
	
	// ステータス等の初期設定
	playerWk.HP = FULL_HP;
	playerWk.HPzan = playerWk.HP;
	playerWk.SP = 0;
	playerWk.HitFrag = false;
	playerWk.score = 0;
	playerWk.graceflag = false;
	playerWk.graceframe = 0;
	playerWk.gracetype = Idle;
	playerWk.damagecount = 0;

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
		// 投げスカり
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, CharaStateAnim[Miss])))
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

		// アニメーション間の補完を設定
		for (int i = 0; i < AnimMax; i++)
		{
			playerWk.Animation->SetShiftTime(playerWk.Animation, i, Data[i].ShiftTime);
		}

		// 当たり判定用ボールを生成
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			D3DXMATRIX Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[i]);
			InitBall(0, &playerWk.HitBall[i], Mtx, HitRadius[i]);
		}

	}
	else
	{
		playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle, Data[Idle].Spd);

		// 前の影を削除
		DeleteShadow(playerWk.ShadowIdx);
	}

	// 波動拳用バレットをセット
	InitHadou(type, &playerWk.HadouBullet);

	// 1P表示のビルボードを作成
	InitPop(type, &playerWk.Popup, 0);

	// 影のセット
	playerWk.ShadowIdx = SetShadow(playerWk.pos, SHADOW_SIZE_X, SHADOW_SIZE_Z);

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

	// 波動拳用バレットをリリース
	UninitHadou(&playerWk.HadouBullet);

	// 1P表示をリリース
	UninitPop(&playerWk.Popup);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	int *Phase = GetPhase();
	CHARA *enemyWk = GetEnemy();
	bool ko = GetKnockout()->pushok;

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
	// 両方SPゲージMAX
	else if (GetKeyboardTrigger(DIK_4))
	{
		AddSpGauge(&playerWk, FULL_SPGAUGE);
		AddSpGauge(enemyWk, FULL_SPGAUGE);
	}
	// 両方体力MAX
	else if (GetKeyboardTrigger(DIK_5))
	{
		SubDamage(&playerWk, -FULL_HP, false);
		SubDamage(enemyWk, -FULL_HP, false);
	}

	// 入力モードの切替
	if (GetKeyboardTrigger(DIK_9))
	{
		playerWk.CommandInput = playerWk.CommandInput ? false : true;
	}

	int Num = NumParticle();

	// デバッグ表示
	PrintDebugProc("プレイヤー座標 X:%f Y:%f Z:%f\n", playerWk.pos.x, playerWk.pos.y, playerWk.pos.z);
	PrintDebugProc("プレイヤー角度 X:%f Y:%f Z:%f\n", playerWk.rot.x, playerWk.rot.y, playerWk.rot.z);
	PrintDebugProc("プレイヤーアニメーション番号 X:%d\n", playerWk.Animation->CurrentAnimID);
	PrintDebugProc("ワイヤーフレーム表示 キーボード:0 = ON or OFF\n");
	PrintDebugProc("プレイヤー入力猶予フラグ %s\n", playerWk.graceflag ? "ON" : "OFF");
	PrintDebugProc("使用しているパーティクルの数 %d\n", Num);
	PrintDebugProc("現在の入力モード Player:%s 切り替え9キー\n", playerWk.CommandInput ? "CommandInput" : "EasyInput");
	PrintDebugProc("モーション時間 %d\n", playerWk.framecount);
#endif
	// チュートリアル用
	if (*Phase == PhaseTutorial)
	{
		REDGAUGE *RedWk = GetRedGauge();
		SetupTutorial(&playerWk, RedWk);
	}

	if (*Phase != PhaseCountdown && playerWk.HPzan > 0)
	{
		// 入力切替
		if (playerWk.CommandInput == false)
		{
			EasyInput(&playerWk, 0);
		}
		else if (playerWk.CommandInput == true)
		{
			CommandInput(&playerWk, 0);
		}
	}

	// KO表示中orヒットストップ中は更新しない
	if ((*Phase == PhaseFinish && ko == false) || GetHitStop() == true)
	{

	}
	else
	{
		// アニメーションを更新
		playerWk.Animation->UpdateAnimation(playerWk.Animation, TIME_PER_FRAME);
	}

	// 勝利時
	if (*Phase == PhaseFinish && playerWk.HPzan > enemyWk->HPzan && playerWk.Animation->CurrentAnimID == Idle)
	{
		playerWk.Animation->ChangeAnimation(playerWk.Animation, Win, Data[Win].Spd);
	}

	// KOボイス
	if ((*Phase == PhaseGame || *Phase == PhaseTraining) && playerWk.HPzan == 0)
	{
		PlaySound(SE_KO);
	}

	// 敗北時HP0になったらダウン
	if (playerWk.HPzan <= 0 && *Phase != PhaseTutorial)
	{
		playerWk.HPzan = 0;
		// 強制的にアニメーション変更
		if (playerWk.Animation->CurrentAnimID != Downpose)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Downpose, Data[Downpose].Spd);
		}
		StopSound(BGM_BATTLE);
		StopSound(BGM_TRAINING);
		if (*Phase != PhaseFinish)
		{
			SetPhase(PhaseFinish);
		}
	}

	// KO表示中orヒットストップ中は更新しない
	if ((*Phase == PhaseFinish && ko == false) || GetHitStop() == true)
	{

	}
	else
	{
		// 座標移動
		MovePlayer();
	}

	// 当たり判定用ボール座標の更新
	D3DXMATRIX Mtx;
	for (int i = 0; i < HIT_CHECK_NUM; i++)
	{
		Mtx = GetBoneMatrix(playerWk.Animation, CharaHitPos[i]);
		UpdateBall(&playerWk.HitBall[i], Mtx);
	}

	// 当たり判定
	if (playerWk.framecount >= Data[playerWk.Animation->CurrentAnimID].CollisionStartTime
		&& playerWk.framecount <= Data[playerWk.Animation->CurrentAnimID].CollisionFinishTime)
	{
		if (playerWk.HitFrag == false)
		{
			// キャラクター同士の当たり判定
			if (HitCheckCToC(&playerWk, enemyWk) == true)
			{
				// 当たった後の動き
				HitAction(&playerWk, enemyWk);
			}
		}
	}

	if (playerWk.HadouBullet.use == true)
	{
		// 波動拳用バレットの更新
		UpdateHadou(&playerWk.HadouBullet);

		// 波動拳の当たり判定
		HitHadou(&playerWk, enemyWk);
	}

	// 1P表示の位置更新
	UpdatePop(&playerWk.Popup, playerWk.HitBall[Hips].pos);

	// 影の更新
	SetPositionShadow(playerWk.ShadowIdx, D3DXVECTOR3(playerWk.HitBall[Hips].pos.x, 0.1f, playerWk.HitBall[Hips].pos.z));
	SetVertexShadow(playerWk.ShadowIdx, SHADOW_SIZE_X, SHADOW_SIZE_Z);
	SetColorShadow(playerWk.ShadowIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

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

	if (playerWk.HadouBullet.use == true)
	{
		// 波動拳用バレットの描画
		//DrawHadou(&playerWk.HadouBullet);
	}

	// 1P表示用ビルボードを描画
	DrawPop(&playerWk.Popup);
}

//=============================================================================
//プレイヤーの情報を取得する
//=============================================================================
CHARA *GetPlayer(void)
{
	return &playerWk;
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
	case Shoryu:
		playerWk.move.x -= sinf(playerWk.rot.y) * VALUE_FRONTWALK;
		playerWk.move.z -= cosf(playerWk.rot.y) * VALUE_FRONTWALK;
		// 相手に接触していた場合、相手を押す
		if (PEdistance <= MIN_DISTANCE)
		{
			enemyWk->move.x -= sinf(enemyWk->rot.y + D3DX_PI) * VALUE_FRONTWALK;
			enemyWk->move.z -= cosf(enemyWk->rot.y + D3DX_PI) * VALUE_FRONTWALK;
		}
		break;
	default:
		break;
	}

	// 攻撃時&ダウン時以外に中心を向く
	if (playerWk.Animation->CurrentAnimID == Punchi || playerWk.Animation->CurrentAnimID == Kick 
		|| playerWk.Animation->CurrentAnimID == Hadou || playerWk.Animation->CurrentAnimID == Shoryu
		|| playerWk.Animation->CurrentAnimID == Down || playerWk.Animation->CurrentAnimID == Downpose)
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

	// 画面外判定
	if (playerWk.pos.x > MOVABLE_AREA)
	{
		playerWk.pos.x = MOVABLE_AREA;
	}
	else if (playerWk.pos.x < -MOVABLE_AREA)
	{
		playerWk.pos.x = -MOVABLE_AREA;
	}
	if (playerWk.pos.z > MOVABLE_AREA)
	{
		playerWk.pos.z = MOVABLE_AREA;
	}
	else if (playerWk.pos.z < -MOVABLE_AREA)
	{
		playerWk.pos.z = -MOVABLE_AREA;
	}

	// 移動中のエフェクトの発生
	if (playerWk.Animation->CurrentAnimID == Frontwalk || playerWk.Animation->CurrentAnimID == Backwalk ||
		playerWk.Animation->CurrentAnimID == Rightstep || playerWk.Animation->CurrentAnimID == Leftstep)
	{		
		// 右足
		SetWalkParticle(D3DXVECTOR3(playerWk.HitBall[RightFoot].pos.x, -5.0f, playerWk.HitBall[RightFoot].pos.z));

		// 左足
		SetWalkParticle(D3DXVECTOR3(playerWk.HitBall[LeftFoot].pos.x, -5.0f, playerWk.HitBall[LeftFoot].pos.z));

	}
}