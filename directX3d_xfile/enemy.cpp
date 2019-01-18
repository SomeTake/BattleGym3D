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
#include "HitCheck.h"
#include "meshwall.h"
#include "particle.h"
#include "CapsuleMesh.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
ENEMY enemyWk;

// キャラクターのアニメーション番号
const char* EnemyAnimNum[] =
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

enum EnemyStateNum
{
	Idle_E,
	Frontwalk_E,
	Backwalk_E,
	Rightstep_E,
	Leftstep_E,
	Guard_E,
	Damage_E,
	Down_E,
	Downpose_E,
	Getup_E,
	Punchi_E,
	Kick_E,
	Hadou_E,
	Shoryu_E,
	SPattack_E,
	Throw_E,
	Win_E
};


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

		// 横移動
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[Rightstep_E])))
		{
			return E_FAIL;
		}
		// 横移動
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[Leftstep_E])))
		{
			return E_FAIL;
		}
		// ダメージ
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[Damage_E])))
		{
			return E_FAIL;
		}
		// ダウン
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[Down_E])))
		{
			return E_FAIL;
		}
		// ダウン状態
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[Downpose_E])))
		{
			return E_FAIL;
		}
		// 起き上がり
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[Getup_E])))
		{
			return E_FAIL;
		}
		// パンチ
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[Punchi_E])))
		{
			return E_FAIL;
		}
		// キック
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[Kick_E])))
		{
			return E_FAIL;
		}
		// 波動
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[Hadou_E])))
		{
			return E_FAIL;
		}
		// 昇竜
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[Shoryu_E])))
		{
			return E_FAIL;
		}
		// SP技
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[SPattack_E])))
		{
			return E_FAIL;
		}
		// 投げ
		if (FAILED(SetupCallbackKeyframes(enemyWk.Animation, EnemyAnimNum[Throw_E])))
		{
			return E_FAIL;
		}

		// AnimationSetを初期化する
		for (int i = 0; i < enemyWk.Animation->AnimSetNum; i++)
		{
			if (FAILED(enemyWk.Animation->InitAnimation(enemyWk.Animation, EnemyAnimNum[i], i)))
			{
				return E_FAIL;
			}
		}
		enemyWk.Animation->CurrentAnimID = Idle_E;
		
		// アニメーション間の補完を設定
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Idle_E, 0.5f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Frontwalk_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Backwalk_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Rightstep_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Leftstep_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Guard_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Damage_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Down_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Downpose_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Getup_E, 1.0f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Punchi_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Kick_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Hadou_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Shoryu_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, SPattack_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Throw_E, 0.1f);
		enemyWk.Animation->SetShiftTime(enemyWk.Animation, Win_E, 0.1f);

		// 当たり判定カプセルを生成
		if (FAILED(
			CreateCapsule(&enemyWk.HitCapsule, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), 60.0f, 15.0f, true)))
		{
			return E_FAIL;
		}

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

	// カプセルをリリース
	UninitCapsule(&enemyWk.HitCapsule);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	int *Phase = GetPhase();
	PLAYER *playerWk = GetPlayer();

#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("エネミー座標 X:%f Y:%f Z:%f\n", enemyWk.pos.x, enemyWk.pos.y, enemyWk.pos.z);
	PrintDebugProc("エネミー角度 X:%f Y:%f Z:%f\n", enemyWk.rot.x, enemyWk.rot.y, enemyWk.rot.z);
#endif

	// 簡単入力
	EasyInputEnemy();

	// 本格入力

	// アニメーションを更新
	enemyWk.Animation->UpdateAnimation(enemyWk.Animation, TIME_PER_FRAME);

	// 勝利時
	if (*Phase == PhaseFinish && enemyWk.HPzan > playerWk->HPzan && enemyWk.Animation->CurrentAnimID == Idle_E)
	{
		enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Win_E, 1.5f);
	}
	// 敗北時HP0になったらダウン
	if (enemyWk.HPzan <= 0)
	{
		enemyWk.HPzan = 0;
		enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Down_E, 0.5f);
		SetPhase(PhaseFinish);
	}

	// 座標移動
	MoveEnemy();

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
	CAMERA *cameraWk = GetCamera(0);

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

	// レンダリング
	enemyWk.Animation->DrawAnimation(enemyWk.Animation, &matrix.world);

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

	// エネミーのカプセルを描画する
	CapsuleMatrix = GetBoneMatrix(enemyWk.Animation, "Hips");
	DrawCapsule(&enemyWk.HitCapsule, &CapsuleMatrix);
}

//=============================================================================
//エネミーの情報を取得する
//=============================================================================
ENEMY *GetEnemy(void)
{
	return &enemyWk;
}

//=============================================================================
//簡単入力（アニメーションの管理も）
//=============================================================================
void EasyInputEnemy(void)
{
	CAMERA *camera = GetCamera(0);

	switch (enemyWk.Animation->CurrentAnimID)
	{
	case Idle_E:
		// 移動
		// 前
		if (GetKeyboardTrigger(DIK_A) || IsButtonTriggered(1, BUTTON_RIGHT) || IsButtonTriggered(1, STICK_RIGHT))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Frontwalk_E, ANIM_SPD_2);
		}
		// 後ろ
		else if (GetKeyboardTrigger(DIK_D) || IsButtonTriggered(1, BUTTON_LEFT) || IsButtonTriggered(1, STICK_LEFT))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Backwalk_E, ANIM_SPD_2);
		}
		// 手前
		else if (GetKeyboardTrigger(DIK_S) || IsButtonTriggered(1, BUTTON_DOWN) || IsButtonTriggered(1, STICK_DOWN))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Rightstep_E, ANIM_SPD_2);
		}
		// 奥
		else if (GetKeyboardTrigger(DIK_W) || IsButtonTriggered(1, BUTTON_UP) || IsButtonTriggered(1, STICK_UP))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Leftstep_E, ANIM_SPD_2);
		}

		// 攻撃
		// パンチ
		else if (GetKeyboardTrigger(DIK_O) || IsButtonTriggered(1, BUTTON_A))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Punchi_E, ANIM_SPD_15);
		}
		// キック
		else if (GetKeyboardTrigger(DIK_K) || IsButtonTriggered(1, BUTTON_B))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Kick_E, ANIM_SPD_15);
		}
		// 波動拳
		else if (GetKeyboardTrigger(DIK_L) || IsButtonTriggered(1, BUTTON_X))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Hadou_E, ANIM_SPD_2);
		}
		// 昇竜拳
		else if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(1, BUTTON_C))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Shoryu_E, ANIM_SPD_15);
		}
		// ガード
		else if (GetKeyboardTrigger(DIK_I) || IsButtonTriggered(1, BUTTON_Y))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Guard_E, ANIM_SPD_1);
		}
		else
		{
		}
		break;
	case Frontwalk_E:
		// 前
		if (GetKeyboardPress(DIK_A) || IsButtonPressed(1, BUTTON_RIGHT) || IsButtonPressed(1, STICK_RIGHT))
		{

		}
		// ボタンリリースで待機に戻る
		else
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle_E, ANIM_SPD_1);
		}

		// 攻撃
		// パンチ
		if (GetKeyboardTrigger(DIK_O) || IsButtonTriggered(1, BUTTON_A))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Punchi_E, ANIM_SPD_15);
		}
		// キック
		else if (GetKeyboardTrigger(DIK_K) || IsButtonTriggered(1, BUTTON_B))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Kick_E, ANIM_SPD_15);
		}
		// 波動拳
		else if (GetKeyboardTrigger(DIK_L) || IsButtonTriggered(1, BUTTON_X))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Hadou_E, ANIM_SPD_2);
		}
		// 昇竜拳
		else if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(1, BUTTON_C))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Shoryu_E, ANIM_SPD_15);
		}
		// ガード
		else if (GetKeyboardTrigger(DIK_I) || IsButtonTriggered(1, BUTTON_Y))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Guard_E, ANIM_SPD_1);
		}
		break;
	case Backwalk_E:
		// 後ろ
		if (GetKeyboardPress(DIK_D) || IsButtonPressed(1, BUTTON_LEFT) || IsButtonPressed(1, STICK_LEFT))
		{

		}
		// ボタンリリースで待機に戻る
		else
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle_E, ANIM_SPD_1);
		}

		// 攻撃
		// パンチ
		if (GetKeyboardTrigger(DIK_O) || IsButtonTriggered(1, BUTTON_A))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Punchi_E, ANIM_SPD_15);
		}
		// キック
		else if (GetKeyboardTrigger(DIK_K) || IsButtonTriggered(1, BUTTON_B))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Kick_E, ANIM_SPD_15);
		}
		// 波動拳
		else if (GetKeyboardTrigger(DIK_L) || IsButtonTriggered(1, BUTTON_X))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Hadou_E, ANIM_SPD_2);
		}
		// 昇竜拳
		else if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(1, BUTTON_C))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Shoryu_E, ANIM_SPD_15);
		}
		// ガード
		else if (GetKeyboardTrigger(DIK_I) || IsButtonTriggered(1, BUTTON_Y))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Guard_E, ANIM_SPD_1);
		}
		break;
	case Rightstep_E:
		// アニメーション終了で待機に戻る
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle_E, ANIM_SPD_1);
		}
		break;
	case Leftstep_E:
		// アニメーション終了で待機に戻る
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle_E, ANIM_SPD_1);
		}
		break;
	case Guard_E:
		if (GetKeyboardPress(DIK_I) || IsButtonPressed(1, BUTTON_Y))
		{

		}
		// ボタンリリースで待機に戻る
		else
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle_E, ANIM_SPD_1);
		}

		// 攻撃
		// パンチ
		if (GetKeyboardTrigger(DIK_O) || IsButtonTriggered(1, BUTTON_A))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Punchi_E, ANIM_SPD_15);
		}
		// キック
		else if (GetKeyboardTrigger(DIK_K) || IsButtonTriggered(1, BUTTON_B))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Kick_E, ANIM_SPD_15);
		}
		// 波動拳
		else if (GetKeyboardTrigger(DIK_L) || IsButtonTriggered(1, BUTTON_X))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Hadou_E, ANIM_SPD_2);
		}
		// 昇竜拳
		else if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(1, BUTTON_C))
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Shoryu_E, ANIM_SPD_15);
		}
	case Damage_E:
		// アニメーション終了で待機に戻る
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle_E, ANIM_SPD_1);
		}
		break;
	case Down_E:
		// アニメーション終了で起き上がりに移行
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Downpose_E, ANIM_SPD_1);
			D3DXMATRIXA16 newmatrix;	// モーション座標を取得するための行列
			newmatrix = GetBoneMatrix(enemyWk.Animation, "Hips");
			enemyWk.pos = D3DXVECTOR3(newmatrix._41, 0.0f, newmatrix._43);
		}
		break;
	case Downpose_E:
		// アニメーション終了で起き上がりに移行
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Getup_E, ANIM_SPD_15);
		}
		break;
	case Getup_E:
		// アニメーション終了で待機に戻る
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle_E, ANIM_SPD_1);
		}
		break;
	case Punchi_E:
		// アニメーション終了で待機に戻る
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle_E, ANIM_SPD_1);
		}
		break;
	case Kick_E:
		// アニメーション終了で待機に戻る
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle_E, ANIM_SPD_1);
		}
		break;
	case Hadou_E:
		// アニメーション終了で待機に戻る
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle_E, ANIM_SPD_1);
		}
		break;
	case Shoryu_E:
		// アニメーション終了で待機に戻る
		if (enemyWk.Animation->MotionEnd == true)
		{
			enemyWk.Animation->ChangeAnimation(enemyWk.Animation, Idle_E, ANIM_SPD_1);
		}
		break;
	default:
		break;
	}

}

//=============================================================================
//座標移動
//=============================================================================
void MoveEnemy(void)
{
	CAMERA *camera = GetCamera(0);
	D3DXVECTOR3 centerpos = GetCenterPos();
	PLAYER *playerWk = GetPlayer();
	D3DXVECTOR3		oldPos = enemyWk.pos;		// 元の位置
	float PEdistance = GetPEdistance();

	// アクションに合わせた座標移動
	switch (enemyWk.Animation->CurrentAnimID)
	{
		// 前移動中の座標処理
	case Frontwalk_E:
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
	case Backwalk_E:
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
	case Rightstep_E:
		enemyWk.move.x -= sinf(enemyWk.rot.y - D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		enemyWk.move.z -= cosf(enemyWk.rot.y - D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		break;
		// 奥移動中の座標処理
	case Leftstep_E:
		enemyWk.move.x -= sinf(enemyWk.rot.y + D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		enemyWk.move.z -= cosf(enemyWk.rot.y + D3DX_PI * VALUE_HALF) * VALUE_ROTATE;
		break;
	default:
		break;
	}

	// 攻撃時以外常に中心を向く
	if (enemyWk.Animation->CurrentAnimID == Punchi_E || enemyWk.Animation->CurrentAnimID == Kick_E ||
		enemyWk.Animation->CurrentAnimID == Hadou_E || enemyWk.Animation->CurrentAnimID == Shoryu_E)
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