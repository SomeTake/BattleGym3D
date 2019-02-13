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
#include "D3DXAnimation.h"
#include "debugproc.h"
#include "player.h"
#include "meshwall.h"
#include "particle.h"
#include "knockout.h"
#include "eredgauge.h"
#include "sound.h"

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
	enemyWk.damagecount = 0;

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
		// 投げスカり
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, CharaStateAnim[Miss])))
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
		for (int i = 0; i < AnimMax; i++)
		{
			enemyWk.Animation->SetShiftTime(enemyWk.Animation, i, Data[i].ShiftTime);
		}

		// 当たり判定用ボールを生成
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			D3DXMATRIX Mtx = GetBoneMatrix(enemyWk.Animation, CharaHitPos[i]);
			InitBall(0, &enemyWk.HitBall[i], Mtx, HitRadius[i]);
		}

	}
	else
	{
		enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle, Data[Idle].Spd);
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
	// 入力モードの切替
	if (GetKeyboardTrigger(DIK_8))
	{
		enemyWk.CommandInput = enemyWk.CommandInput ? false : true;
	}
	// デバッグ表示
	PrintDebugProc("エネミー座標 X:%f Y:%f Z:%f\n", enemyWk.pos.x, enemyWk.pos.y, enemyWk.pos.z);
	PrintDebugProc("エネミー角度 X:%f Y:%f Z:%f\n", enemyWk.rot.x, enemyWk.rot.y, enemyWk.rot.z);
	PrintDebugProc("エネミーアニメーション番号 No:%d\n", enemyWk.Animation->CurrentAnimID);
	PrintDebugProc("現在の入力モード Enemy:%s 切り替え8キー\n", enemyWk.CommandInput ? "CommandInput" : "EasyInput");

#endif
	// チュートリアル用
	if (*Phase == PhaseTutorial)
	{
		REDGAUGE *RedWk = GetERedGauge();
		SetupTutorial(&enemyWk, RedWk);
	}

	if (*Phase != PhaseCountdown && *Phase != PhaseTraining && enemyWk.HPzan > 0)
	{
		// 入力切替
		if (enemyWk.CommandInput == false)
		{
			EasyInput(&enemyWk, 1);
		}
		else if (enemyWk.CommandInput == true)
		{
			CommandInput(&enemyWk, 1);
		}
	}

	if (*Phase == PhaseTraining)
	{
		// AIが操作
		BattleAI(&enemyWk, playerWk);
	}

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
		enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Win, Data[Win].Spd);
	}
	// 敗北時HP0になったらダウン
	if (enemyWk.HPzan <= 0 && *Phase != PhaseTutorial)
	{
		enemyWk.HPzan = 0;
		// 強制的にアニメーション変更
		if (enemyWk.Animation->CurrentAnimID != Downpose)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Downpose, Data[Downpose].Spd);
		}
		PlaySound(SE_KO, 0, 0);
		StopSound(BGM_BATTLE, 0);
		StopSound(BGM_TRAINING, 0);
		SetPhase(PhaseFinish);
	}

	// KO表示中は更新しない
	if (*Phase == PhaseFinish && ko == false)
	{

	}
	else
	{
		// 座標移動
		MoveEnemy();
	}

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
		//DrawHadou(&enemyWk.HadouBullet);
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
	case Shoryu:
		enemyWk.move.x -= sinf(enemyWk.rot.y) * VALUE_FRONTWALK;
		enemyWk.move.z -= cosf(enemyWk.rot.y) * VALUE_FRONTWALK;
		// 相手に接触していた場合、相手を押す
		if (PEdistance <= MIN_DISTANCE)
		{
			playerWk->move.x -= sinf(playerWk->rot.y + D3DX_PI) * VALUE_FRONTWALK;
			playerWk->move.z -= cosf(playerWk->rot.y + D3DX_PI) * VALUE_FRONTWALK;
		}
	default:
		break;
	}

	// 攻撃時&ダウン時以外常に中心を向く
	if (enemyWk.Animation->CurrentAnimID == Punchi || enemyWk.Animation->CurrentAnimID == Kick 
		|| enemyWk.Animation->CurrentAnimID == Hadou || enemyWk.Animation->CurrentAnimID == Shoryu
		|| enemyWk.Animation->CurrentAnimID == Down || enemyWk.Animation->CurrentAnimID == Downpose)
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

	// 画面外判定
	if (enemyWk.pos.x > MOVABLE_AREA)
	{
		enemyWk.pos.x = MOVABLE_AREA;
	}
	else if (enemyWk.pos.x < -MOVABLE_AREA)
	{
		enemyWk.pos.x = -MOVABLE_AREA;
	}
	if (enemyWk.pos.z > MOVABLE_AREA)
	{
		enemyWk.pos.z = MOVABLE_AREA;
	}
	else if (enemyWk.pos.z < -MOVABLE_AREA)
	{
		enemyWk.pos.z = -MOVABLE_AREA;
	}

	// 移動中のエフェクトの発生
	if (enemyWk.Animation->CurrentAnimID == Frontwalk || enemyWk.Animation->CurrentAnimID == Backwalk ||
		enemyWk.Animation->CurrentAnimID == Rightstep || enemyWk.Animation->CurrentAnimID == Leftstep)
	{
		// 右足
		SetWalkParticle(D3DXVECTOR3(enemyWk.HitBall[RightFoot].pos.x, -5.0f, enemyWk.HitBall[RightFoot].pos.z));

		// 左足
		SetWalkParticle(D3DXVECTOR3(enemyWk.HitBall[LeftFoot].pos.x, -5.0f, enemyWk.HitBall[LeftFoot].pos.z));

	}

}
