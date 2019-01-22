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
#include "CapsuleMesh.h"
#include "ball.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
PLAYER playerWk;

// キャラクターのアニメーション番号
const char* PlayerAnimNum[] =
{
	"idle",				// 待機
	"frontwalk",		// 前歩き
	"backwalk",			// 後ろ歩き
	"rightstep",		// 横移動
	"leftstep",			// 横移動
	"guard",			// ガード めっちゃ胸反る
	"damage",			// ダメージ受けた
	"down",				// ダウン
	"downpose",			// ダウン状態
	"getup",			// 起き上がり
	"punchi",			// パンチ
	"kick",				// キック
	"hadou",			// 波動拳。やたら発生が遅い
	"shoryu",			// 昇竜拳。バックフリップ
	"SPattack",			// SP技。めっちゃ回転する
	"throw",			// 投げ。掴んで膝入れてアッパー
	"win"				// ガッツポーズ（勝利時）
};

enum PlayerStateNum
{
	Idle_P,
	Frontwalk_P,
	Backwalk_P,
	Rightstep_P,
	Leftstep_P,
	Guard_P,
	Damage_P,
	Down_P,
	Downpose_P,
	Getup_P,
	Punchi_P,
	Kick_P,
	Hadou_P,
	Shoryu_P,
	SPattack_P,
	Throw_P,
	Win_P
};

bool RenderWireframe = false;
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

	if (type == 0)
	{
		//オブジェクトの初期化
		playerWk.Animation = CreateAnimationObject();

		// xFileの読み込み
		if (FAILED(Load_xFile(playerWk.Animation, PLAYER_XFILE, "Player")))
		{
			return E_FAIL;
		}

		// AnimationCallbackをセットする
		// 前歩き
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Frontwalk_P])))
		{
			return E_FAIL;
		}
		// 後ろ歩き
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Backwalk_P])))
		{
			return E_FAIL;
		}
		// 横移動
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Rightstep_P])))
		{
			return E_FAIL;
		}
		// 横移動
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Leftstep_P])))
		{
			return E_FAIL;
		}
		// ダメージ
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Damage_P])))
		{
			return E_FAIL;
		}
		// ダウン
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Down_P])))
		{
			return E_FAIL;
		}
		// ダウンポーズ
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Downpose_P])))
		{
			return E_FAIL;
		}
		// 起き上がり
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Getup_P])))
		{
			return E_FAIL;
		}
		// パンチ
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Punchi_P])))
		{
			return E_FAIL;
		}
		// キック
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Kick_P])))
		{
			return E_FAIL;
		}
		// 波動
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Hadou_P])))
		{
			return E_FAIL;
		}
		// 昇竜
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Shoryu_P])))
		{
			return E_FAIL;
		}
		// SP技
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[SPattack_P])))
		{
			return E_FAIL;
		}
		// 投げ
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Throw_P])))
		{
			return E_FAIL;
		}

		// AnimationSetを初期化する
		for (int i = 0; i < playerWk.Animation->AnimSetNum; i++)
		{
			if (FAILED(playerWk.Animation->InitAnimation(playerWk.Animation, PlayerAnimNum[i], i)))
			{
				return E_FAIL;
			}
		}
		playerWk.Animation->CurrentAnimID = Idle_P;
		playerWk.Animation->SetShiftTime(playerWk.Animation, Idle_P, 0.5f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Frontwalk_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Backwalk_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Rightstep_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Leftstep_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Guard_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Damage_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Down_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Downpose_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Getup_P, 1.0f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Punchi_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Kick_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Hadou_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Shoryu_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, SPattack_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Throw_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Win_P, 0.1f);

		// 当たり判定用ボールを生成
		D3DXMATRIX Mtx = GetBoneMatrix(playerWk.Animation, "Hips");
		InitBall(0, &playerWk.HitBall[0], Mtx, BODY_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, "Neck");
		InitBall(0, &playerWk.HitBall[1], Mtx, BODY_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, "Head");
		InitBall(0, &playerWk.HitBall[2], Mtx, BODY_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, "LeftShoulder");
		InitBall(0, &playerWk.HitBall[3], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, "RightShoulder");
		InitBall(0, &playerWk.HitBall[4], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, "LeftHand");
		InitBall(0, &playerWk.HitBall[5], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, "RightHand");
		InitBall(0, &playerWk.HitBall[6], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, "LeftFoot");
		InitBall(0, &playerWk.HitBall[7], Mtx, FOOT_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, "RightFoot");
		InitBall(0, &playerWk.HitBall[8], Mtx, FOOT_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, "LeftForeArm");
		InitBall(0, &playerWk.HitBall[9], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, "RightForeArm");
		InitBall(0, &playerWk.HitBall[10], Mtx, ARM_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, "LeftLeg");
		InitBall(0, &playerWk.HitBall[11], Mtx, FOOT_RADIUS);
		Mtx = GetBoneMatrix(playerWk.Animation, "RightLeg");
		InitBall(0, &playerWk.HitBall[12], Mtx, FOOT_RADIUS);

		// 影の生成
		playerWk.IdxShadow = CreateShadow(playerWk.pos, SHADOW_SIZE_X, SHADOW_SIZE_Z);
		playerWk.SizeShadow = 25.0f;
		playerWk.ColShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);	
	}

	// 初期アニメーションを待機にする
	playerWk.Animation->CurrentAnimID = Idle_P;

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
	ENEMY *enemyWk = GetEnemy();

#ifdef _DEBUG
	// デバッグ用入力
	if (GetKeyboardTrigger(DIK_1))
	{
		playerWk.Animation->ChangeAnimation(playerWk.Animation, Down_P, 1.5f);
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
		playerWk.Animation->ChangeAnimation(playerWk.Animation, Damage_P, 1.5f);
	}
	else if (GetKeyboardTrigger(DIK_9))
	{
		RenderWireframe = true;
	}
	else if (GetKeyboardTrigger(DIK_0))
	{
		RenderWireframe = false;
	}

	// デバッグ表示
	PrintDebugProc("プレイヤー座標 X:%f Y:%f Z:%f\n", playerWk.pos.x, playerWk.pos.y, playerWk.pos.z);
	PrintDebugProc("プレイヤー角度 X:%f Y:%f Z:%f\n", playerWk.rot.x, playerWk.rot.y, playerWk.rot.z);
	PrintDebugProc("プレイヤーアニメーション番号 X:%d\n", playerWk.Animation->CurrentAnimID);
	PrintDebugProc("ワイヤーフレーム表示 キーボード:9 = OFF キーボード:0 = ON\n");
#endif

	// HP0&ダウンモーションが終了した場合、それ以上アニメーションを更新しない、操作できない
	if (playerWk.Animation->CurrentAnimID == Downpose_P && playerWk.HPzan == 0)
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
		if (*Phase == PhaseFinish && playerWk.HPzan > enemyWk->HPzan && playerWk.Animation->CurrentAnimID == Idle_P)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Win_P, 1.5f);
		}
		// 敗北時HP0になったらダウン
		if (playerWk.HPzan <= 0)
		{
			playerWk.HPzan = 0;
			// 強制的にアニメーション変更
			if (playerWk.Animation->CurrentAnimID != Downpose_P && playerWk.Animation->CurrentAnimID != Down_P)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Down_P, 0.5f);
			}
			else if (playerWk.Animation->CurrentAnimID == Down_P && playerWk.Animation->MotionEnd == true)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Down_P, ANIM_SPD_1);
			}
			SetPhase(PhaseFinish);
		}
	}

	// 座標移動
	MovePlayer();

	// 攻撃判定
	HitCheckPlayer();

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

	if (RenderWireframe == true)
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}

	// レンダリング
	playerWk.Animation->DrawAnimation(playerWk.Animation, &WorldMtxPlayer);

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

	if (RenderWireframe == true)
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
PLAYER *GetPlayer(void)
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
	case Idle_P:
		// 移動
		// 前
		if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_RIGHT) || IsButtonTriggered(0, STICK_RIGHT))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Frontwalk_P, ANIM_SPD_2);
		}
		// 後ろ
		else if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_LEFT) || IsButtonTriggered(0, STICK_LEFT))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Backwalk_P, ANIM_SPD_2);
		}
		// 手前
		else if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN) || IsButtonTriggered(0, STICK_DOWN))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Rightstep_P, ANIM_SPD_2);
		}
		// 奥
		else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, STICK_UP))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Leftstep_P, ANIM_SPD_2);
		}

		// 攻撃
		// パンチ
		else if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi_P, ANIM_SPD_15);
		}
		// キック
		else if (GetKeyboardTrigger(DIK_M) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick_P, ANIM_SPD_15);
		}
		// 波動拳
		else if (GetKeyboardTrigger(DIK_N) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Hadou_P, ANIM_SPD_2);
		}
		// 昇竜拳
		else if (GetKeyboardTrigger(DIK_H) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Shoryu_P, ANIM_SPD_15);
		}
		// ガード
		else if (GetKeyboardTrigger(DIK_G) || IsButtonTriggered(0, BUTTON_Y))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Guard_P, ANIM_SPD_1);
		}
		else
		{
		}
		break;
	case Frontwalk_P:
		// 前
		if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, STICK_RIGHT))
		{

		}
		// ボタンリリースで待機に戻る
		else
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, ANIM_SPD_1);
		}

		// 攻撃
		// パンチ
		if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi_P, ANIM_SPD_15);
		}
		// キック
		else if (GetKeyboardTrigger(DIK_M) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick_P, ANIM_SPD_15);
		}
		// 波動拳
		else if (GetKeyboardTrigger(DIK_N) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Hadou_P, ANIM_SPD_2);
		}
		// 昇竜拳
		else if (GetKeyboardTrigger(DIK_H) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Shoryu_P, ANIM_SPD_15);
		}
		// ガード
		else if (GetKeyboardTrigger(DIK_G) || IsButtonTriggered(0, BUTTON_Y))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Guard_P, ANIM_SPD_1);
		}
		break;
	case Backwalk_P:
		// 後ろ
		if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, STICK_LEFT))
		{

		}
		// ボタンリリースで待機に戻る
		else
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, ANIM_SPD_1);
		}

		// 攻撃
		// パンチ
		if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi_P, ANIM_SPD_15);
		}
		// キック
		else if (GetKeyboardTrigger(DIK_M) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick_P, ANIM_SPD_15);
		}
		// 波動拳
		else if (GetKeyboardTrigger(DIK_N) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Hadou_P, ANIM_SPD_2);
		}
		// 昇竜拳
		else if (GetKeyboardTrigger(DIK_H) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Shoryu_P, ANIM_SPD_15);
		}
		// ガード
		else if (GetKeyboardTrigger(DIK_G) || IsButtonTriggered(0, BUTTON_Y))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Guard_P, ANIM_SPD_1);
		}
		break;
	case Rightstep_P:
		// アニメーション終了で待機に戻る
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, ANIM_SPD_1);
		}
		break;
	case Leftstep_P:
		// アニメーション終了で待機に戻る
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, ANIM_SPD_1);
		}
		break;
	case Guard_P:
		if (GetKeyboardPress(DIK_G) || IsButtonPressed(0, BUTTON_Y))
		{

		}
			// ボタンリリースで待機に戻る
		else
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, ANIM_SPD_1);
		}

		// 攻撃
		// パンチ
		if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi_P, ANIM_SPD_15);
		}
		// キック
		else if (GetKeyboardTrigger(DIK_M) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick_P, ANIM_SPD_15);
		}
		// 波動拳
		else if (GetKeyboardTrigger(DIK_N) || IsButtonTriggered(0, BUTTON_X))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Hadou_P, ANIM_SPD_2);
		}
		// 昇竜拳
		else if (GetKeyboardTrigger(DIK_H) || IsButtonTriggered(0, BUTTON_C))
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Shoryu_P, ANIM_SPD_15);
		}
	case Damage_P:
		// アニメーション終了で待機に戻る
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, ANIM_SPD_1);
		}
		break;
	case Down_P:
		// アニメーション終了で起き上がりに移行
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Downpose_P, ANIM_SPD_1);
			D3DXMATRIXA16 newmatrix;	// モーション座標を取得するための行列
			newmatrix = GetBoneMatrix(playerWk.Animation, "Hips");
			playerWk.pos = D3DXVECTOR3(newmatrix._41, 0.0f, newmatrix._43);
		}
		break;
	case Downpose_P:
		// アニメーション終了で起き上がりに移行
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Getup_P, ANIM_SPD_15);
		}
	case Getup_P:
		// アニメーション終了で待機に戻る
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, ANIM_SPD_1);
		}
		break;
	case Punchi_P:
		// アニメーション終了で待機に戻る
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, ANIM_SPD_1);
		}
		break;
	case Kick_P:
		// アニメーション終了で待機に戻る
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, ANIM_SPD_1);
		}
		break;
	case Hadou_P:
		// アニメーション終了で待機に戻る
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, ANIM_SPD_1);
		}
		break;
	case Shoryu_P:
		// アニメーション終了で待機に戻る
		if (playerWk.Animation->MotionEnd == true)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, ANIM_SPD_1);
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
	ENEMY *enemyWk = GetEnemy();
	float PEdistance = GetPEdistance();

	// アクションに合わせた座標移動
	switch (playerWk.Animation->CurrentAnimID)
	{
		// 前移動中の座標処理
	case Frontwalk_P:
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
	case Backwalk_P:
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
	case Rightstep_P:
		playerWk.move.x -= sinf(playerWk.rot.y + D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		playerWk.move.z -= cosf(playerWk.rot.y + D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		break;
		// 奥移動中の座標処理
	case Leftstep_P:
		playerWk.move.x -= sinf(playerWk.rot.y - D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		playerWk.move.z -= cosf(playerWk.rot.y - D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		break;
	default:
		break;
	}

	// 攻撃モーション時以外に中心を向く
	if (playerWk.Animation->CurrentAnimID == Punchi_P || playerWk.Animation->CurrentAnimID == Kick_P 
		|| playerWk.Animation->CurrentAnimID == Hadou_P || playerWk.Animation->CurrentAnimID == Shoryu_P)
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
	if (playerWk.Animation->CurrentAnimID == Frontwalk_P || playerWk.Animation->CurrentAnimID == Backwalk_P ||
		playerWk.Animation->CurrentAnimID == Rightstep_P || playerWk.Animation->CurrentAnimID == Leftstep_P)
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
	// 当たり判定の更新
	D3DXMATRIX Mtx = GetBoneMatrix(playerWk.Animation, "Hips");
	UpdateBall(&playerWk.HitBall[0], Mtx);
	Mtx = GetBoneMatrix(playerWk.Animation, "Neck");
	UpdateBall(&playerWk.HitBall[1], Mtx);
	Mtx = GetBoneMatrix(playerWk.Animation, "Head");
	UpdateBall(&playerWk.HitBall[2], Mtx);
	Mtx = GetBoneMatrix(playerWk.Animation, "LeftShoulder");
	UpdateBall(&playerWk.HitBall[3], Mtx);
	Mtx = GetBoneMatrix(playerWk.Animation, "RightShoulder");
	UpdateBall(&playerWk.HitBall[4], Mtx);
	Mtx = GetBoneMatrix(playerWk.Animation, "LeftHand");
	UpdateBall(&playerWk.HitBall[5], Mtx);
	Mtx = GetBoneMatrix(playerWk.Animation, "RightHand");
	UpdateBall(&playerWk.HitBall[6], Mtx);
	Mtx = GetBoneMatrix(playerWk.Animation, "LeftFoot");
	UpdateBall(&playerWk.HitBall[7], Mtx);
	Mtx = GetBoneMatrix(playerWk.Animation, "RightFoot");
	UpdateBall(&playerWk.HitBall[8], Mtx);
	Mtx = GetBoneMatrix(playerWk.Animation, "LeftForeArm");
	UpdateBall(&playerWk.HitBall[9], Mtx);
	Mtx = GetBoneMatrix(playerWk.Animation, "RightForeArm");
	UpdateBall(&playerWk.HitBall[10], Mtx);
	Mtx = GetBoneMatrix(playerWk.Animation, "LeftLeg");
	UpdateBall(&playerWk.HitBall[11], Mtx);
	Mtx = GetBoneMatrix(playerWk.Animation, "RightLeg");
	UpdateBall(&playerWk.HitBall[12], Mtx);

	// 引数にAtkPosと半径と、エネミー側の当たり判定の座標と半径を入れられる当たり判定の関数を作る
}