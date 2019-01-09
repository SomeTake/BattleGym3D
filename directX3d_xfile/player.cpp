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
	"rolling",			// 横移動　でんぐり返し
	"jump",				// ジャンプ
	"guard",			// ガード めっちゃ胸反る
	"damage",			// ダメージ受けた
	"down",				// ダウン
	"downpose",			// ダウンしたまま（敗北時）
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
	Rolling_P,
	Jump_P,
	Guard_P,
	Damage_P,
	Down_P,
	DownPose_P,
	Getup_P,
	Punchi_P,
	Kick_P,
	Hadou_P,
	Shoryu_P,
	SPattack_P,
	Throw_P,
	Win_P
};

bool upflag = false;
bool downflag = false;
int flagframecount = 0;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 位置・回転・スケールの初期設定
	playerWk.pos = D3DXVECTOR3(50.0f, 0.0f, 0.0f);
	playerWk.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	playerWk.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk.jump = false;
	playerWk.speed = JUMP_SPEED;
	playerWk.HP = FULL_HP;
	playerWk.HPzan = playerWk.HP;
	playerWk.Action = Idle_P;
	playerWk.NextAction = Idle_P;

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
		// 回転
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Rolling_P])))
		{
			return E_FAIL;
		}
		// ジャンプ
		if (FAILED(SetupCallbackKeyframes(playerWk.Animation, PlayerAnimNum[Jump_P])))
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
		playerWk.Animation->SetShiftTime(playerWk.Animation, Frontwalk_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Backwalk_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Rolling_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Jump_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Guard_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Damage_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Down_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, DownPose_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Getup_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Punchi_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Kick_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Hadou_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Shoryu_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, SPattack_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Throw_P, 0.1f);
		playerWk.Animation->SetShiftTime(playerWk.Animation, Win_P, 0.1f);

		// 影の生成
		playerWk.IdxShadow = CreateShadow(playerWk.pos, SHADOW_SIZE_X, SHADOW_SIZE_Z);
		playerWk.SizeShadow = 25.0f;
		playerWk.ColShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	// アニメーションをリリース
	playerWk.Animation->UninitAnimation(playerWk.Animation);

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *camera = GetCamera(0);
	D3DXVECTOR3 centerpos = GetCenterPos();

	// アニメーションを更新
	playerWk.Animation->UpdateAnimation(playerWk.Animation, TIME_PER_FRAME);

	// 現在のアニメーション管理
	switch (playerWk.Action)
	{
	case Idle_P:
		// 入力処理
		// D：右( → )
		if (GetKeyboardTrigger(DIK_D) || IsButtonTriggered(0, BUTTON_RIGHT) || IsButtonTriggered(0, STICK_RIGHT))
		{
			playerWk.NextAction = Frontwalk_P;
		}
		// A：左( ← )
		else if (GetKeyboardTrigger(DIK_A) || IsButtonTriggered(0, BUTTON_LEFT) || IsButtonTriggered(0, STICK_LEFT))
		{
			playerWk.NextAction = Backwalk_P;
		}
		// S：下( ↓ )
		else if (GetKeyboardTrigger(DIK_S) || IsButtonTriggered(0, BUTTON_DOWN) || IsButtonTriggered(0, STICK_DOWN))
		{
			downflag = true;
			playerWk.NextAction = Rolling_P;
		}
		// W：上( ↑ )
		else if (GetKeyboardTrigger(DIK_W) || IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, STICK_UP))
		{
			upflag = true;
			playerWk.NextAction = Rolling_P;
		}
		
		// 攻撃処理
		// パンチ
		if (GetKeyboardTrigger(DIK_U) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.NextAction = Punchi_P;
		}
		// キック
		if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.NextAction = Kick_P;
		}
		break;
	case Frontwalk_P:
		// 攻撃処理
		// パンチ
		if (GetKeyboardTrigger(DIK_U) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.NextAction = Punchi_P;
		}
		// キック
		else if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.NextAction = Kick_P;
		}

		// モデルの移動
		else if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, STICK_RIGHT))
		{
			playerWk.move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * VALUE_FRONTWALK;
			playerWk.move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * VALUE_FRONTWALK;
			playerWk.rotDest.y = camera->rot.y + D3DX_PI * 0.50f;
		}

		// リリースされた場合待機状態に戻す
		else
		{
			playerWk.NextAction = Idle_P;
		}
		break;
	case Backwalk_P:
		// 攻撃処理
		// パンチ
		if (GetKeyboardTrigger(DIK_U) || IsButtonTriggered(0, BUTTON_A))
		{
			playerWk.NextAction = Punchi_P;
		}
		// キック
		else if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_B))
		{
			playerWk.NextAction = Kick_P;
		}

		// モデルの移動
		else if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, STICK_LEFT))
		{
			playerWk.move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * VALUE_BACKWALK;
			playerWk.move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * VALUE_BACKWALK;
			playerWk.rotDest.y = camera->rot.y - D3DX_PI * 0.50f;
		}

		// リリースされた場合待機状態に戻す
		else
		{
			playerWk.NextAction = Idle_P;
		}
		break;
	case Rolling_P:
		// アニメーションが終わったので待機状態に戻す
		if (playerWk.NextAction == Rolling_P)
		{
			playerWk.NextAction = Idle_P;
		}
		break;
	case Punchi_P:
		// アニメーションが終わったので待機状態に戻す
		if (playerWk.NextAction == Punchi_P)
		{
			playerWk.NextAction = Idle_P;
		}
		break;
	case Kick_P:
		// アニメーションが終わったので待機状態に戻す
		if (playerWk.NextAction == Kick_P)
		{
			playerWk.NextAction = Idle_P;
		}
		break;
	default:
		break;
	}

	// 次のアニメーション管理
	switch (playerWk.NextAction)
	{
	case Idle_P:
		// 他アクションからボタンリリースで待機状態に戻る
		if (playerWk.Action != Idle_P)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, 0.1f);
			playerWk.Action = Idle_P;
		}
		else
		{
			if (playerWk.Animation->MotionEnd == true)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Idle_P, 0.1f);
				playerWk.Action = Idle_P;
			}
		}
		break;

	case Frontwalk_P:
		playerWk.Animation->ChangeAnimation(playerWk.Animation, Frontwalk_P, 0.1f);
		playerWk.Action = Frontwalk_P;
		break;
	case Backwalk_P:
		playerWk.Animation->ChangeAnimation(playerWk.Animation, Backwalk_P, 0.1f);
		playerWk.Action = Backwalk_P;
		break;
	case Rolling_P:
		// アニメーションを移行させる
		if (playerWk.Action != Rolling_P)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Rolling_P, 0.1f);
			playerWk.Action = Rolling_P;
		}
		else
		{
			// アニメーションが終了したら待機モーションに戻す
			if (playerWk.Animation->MotionEnd == true)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Rolling_P, 0.1f);
				playerWk.Action = Rolling_P;
			}
		}
		break;
	case Punchi_P:
		// アニメーションを移行させる
		if (playerWk.Action != Punchi_P)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi_P, 0.1f);
			playerWk.Action = Punchi_P;
		}
		else
		{
			// アニメーションが終了したら待機モーションに戻す
			if (playerWk.Animation->MotionEnd == true)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Punchi_P, 0.1f);
				playerWk.Action = Punchi_P;
			}
		}
		break;
	case Kick_P:
		// アニメーションを移行させる
		if (playerWk.Action != Kick_P)
		{
			playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick_P, 0.1f);
			playerWk.Action = Kick_P;
		}
		else
		{
			// アニメーションが終了したら待機モーションに戻す
			if (playerWk.Animation->MotionEnd == true)
			{
				playerWk.Animation->ChangeAnimation(playerWk.Animation, Kick_P, 0.1f);
				playerWk.Action = Kick_P;
			}
		}
		break;
	default:
		break;
	}

	// 上移動中の座標処理
	if (upflag == true)
	{
		playerWk.move.x -= sinf(camera->rot.y) * VALUE_ROTATE;
		playerWk.move.z -= cosf(camera->rot.y) * VALUE_ROTATE;
		playerWk.rotDest.y = camera->rot.y;
		flagframecount++;
		if (flagframecount == 10)
		{
			flagframecount = 0;
			upflag = false;
		}
	}

	// 下移動中の座標処理
	if (downflag == true)
	{
		playerWk.move.x -= sinf(D3DX_PI + camera->rot.y) * VALUE_ROTATE;
		playerWk.move.z -= cosf(D3DX_PI + camera->rot.y) * VALUE_ROTATE;
		playerWk.rotDest.y = D3DX_PI + camera->rot.y;
		flagframecount++;
		if (flagframecount == 10)
		{
			flagframecount = 0;
			downflag = false;
		}
	}

	// 常に中心を向く
	playerWk.rot.y = atan2f(centerpos.x - playerWk.pos.x, centerpos.z - playerWk.pos.z) + D3DX_PI;

	/// 位置移動
	playerWk.pos.x += playerWk.move.x;
	playerWk.pos.y += playerWk.move.y;
	playerWk.pos.z += playerWk.move.z;

	// 移動量に慣性をかける
	playerWk.move.x += (0.0f - playerWk.move.x) * RATE_MOVE_MODEL;
	playerWk.move.y += (0.0f - playerWk.move.y) * RATE_MOVE_MODEL;
	playerWk.move.z += (0.0f - playerWk.move.z) * RATE_MOVE_MODEL;

	//モデルのジャンプ
	if (GetKeyboardTrigger(DIK_SPACE) && playerWk.pos.y == 0.0f)
	{
		playerWk.jump = true;
	}
	if (playerWk.jump == true)
	{
		playerWk.pos.y += playerWk.speed;
		playerWk.speed -= 0.98f;
		if (playerWk.pos.y <= 0.0f)
		{
			playerWk.speed = JUMP_SPEED;
			playerWk.pos.y = 0.0f;
			playerWk.jump = false;
		}
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
	MATRIX matrix = *GetMatrix();
	CAMERA *cameraWk = GetCamera(0);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&matrix.world);

	// スケールを反映
	D3DXMatrixScaling(&matrix.scale, playerWk.scl.x, playerWk.scl.y, playerWk.scl.z);
	D3DXMatrixMultiply(&matrix.world, &matrix.world, &matrix.scale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&matrix.scale, playerWk.rot.y, playerWk.rot.x, playerWk.rot.z);
	D3DXMatrixMultiply(&matrix.world, &matrix.world, &matrix.scale);

	// 移動を反映
	D3DXMatrixTranslation(&matrix.translation, playerWk.pos.x, playerWk.pos.y, playerWk.pos.z);
	D3DXMatrixMultiply(&matrix.world, &matrix.world, &matrix.translation);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &matrix.world);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// レンダリング
	playerWk.Animation->DrawAnimation(playerWk.Animation, &matrix.world);

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
//プレイヤーの情報を取得する
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &playerWk;
}