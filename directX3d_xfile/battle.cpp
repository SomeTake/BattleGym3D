//=============================================================================
//
// バトル処理（プレイヤーもエネミーも呼び出す処理） [battle.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "main.h"
#include "battle.h"
#include "input.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=====================================================================================================
// 当たり判定
// AttackPos = 攻撃側中心位置, DefendPos = 防御側中心位置, AttackRange = 攻撃側半径, DefendPos = 防御側半径
//=====================================================================================================
bool HitBC(D3DXVECTOR3 AttackPos, D3DXVECTOR3 DefendPos, float AttackRange, float DefendRange)
{
	// 当たり判定の中心の距離を測る
	D3DXVECTOR3 unit = AttackPos - DefendPos;
	float dist = D3DXVec3Length(&unit);

	// 当たり判定の範囲を足した距離を出す（球なのでXYZどれとっても同じ）
	float hitrange = AttackRange + DefendRange;

	// 当たり判定の中心の距離よりも範囲を足した距離の方が長ければ当たる
	if (dist <= hitrange)
	{
		return true;
	}
	// 外れ
	else
	{

	}
	return false;
}


//=============================================================================
// キャラクター同士の当たり判定
//=============================================================================
bool HitCheckCToC(CHARA *AttackChara, CHARA *DefendChara)
{
	switch (AttackChara->Animation->CurrentAnimID)
	{
	case Punchi:
		// 左手と相手の各部位との判定
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			if (HitBC(AttackChara->HitBall[LeftHand].pos, DefendChara->HitBall[i].pos,
				AttackChara->HitBall[LeftHand].scl.x, DefendChara->HitBall[i].scl.x) == true)
			{
				return true;
			}
		}
		break;
	case Kick:
		// 右足と相手の各部位との判定
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			if (HitBC(AttackChara->HitBall[RightFoot].pos, DefendChara->HitBall[i].pos,
				AttackChara->HitBall[RightFoot].scl.x, DefendChara->HitBall[i].scl.x) == true)
			{
				return true;
			}
		}
		break;
	case Shoryu:
		// 両足と相手の各部位との判定
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			// 左足との判定
			if (HitBC(AttackChara->HitBall[LeftFoot].pos, DefendChara->HitBall[i].pos,
				AttackChara->HitBall[LeftFoot].scl.x, DefendChara->HitBall[i].scl.x) == true)
			{
				return true;
			}
			// 右足との判定
			else if (HitBC(AttackChara->HitBall[RightFoot].pos, DefendChara->HitBall[i].pos,
				AttackChara->HitBall[RightFoot].scl.x, DefendChara->HitBall[i].scl.x) == true)
			{
				return true;
			}
		}
		break;

	default:
		break;
	}

	return false;
}


//=============================================================================
//攻撃が当たったときに発生する動き
//=============================================================================
void HitAction(CHARA *AttackChara, CHARA *DefendChara)
{
	switch (AttackChara->Animation->CurrentAnimID)
	{
	case Punchi:
		// 敵の状態確認
		if (DefendChara->Animation->CurrentAnimID == Guard || DefendChara->Animation->CurrentAnimID == Down
			|| DefendChara->Animation->CurrentAnimID == Downpose || DefendChara->Animation->CurrentAnimID == Getup)
		{
			// エフェクト

			// スコア
			AddScore(AttackChara, DAMAGE_PUNCHI);
			AddScore(DefendChara, DAMAGE_PUNCHI);
		}
		else
		{
			// 敵のモーション変更
			DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Damage, ANIM_SPD_2);
			// ダメージ
			SubDamage(DefendChara, DAMAGE_PUNCHI);
			// SPゲージ増減
			AddSpGauge(AttackChara, DAMAGE_PUNCHI);
			AddSpGauge(DefendChara, DAMAGE_PUNCHI);
			// エフェクト

			// スコア
			AddScore(AttackChara, DAMAGE_PUNCHI);
		}
		//　ヒットフラグの変更
		AttackChara->HitFrag = true;
		break;
	case Kick:
		// 敵の状態確認
		if (DefendChara->Animation->CurrentAnimID == Guard || DefendChara->Animation->CurrentAnimID == Down
			|| DefendChara->Animation->CurrentAnimID == Downpose || DefendChara->Animation->CurrentAnimID == Getup)
		{
			// エフェクト

			// スコア
			AddScore(AttackChara, DAMAGE_KICK);
			AddScore(DefendChara, DAMAGE_KICK);
		}
		else
		{
			// 敵のモーション変更
			DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Damage, ANIM_SPD_1);
			// ダメージ
			SubDamage(DefendChara, DAMAGE_KICK);
			// SPゲージ増減
			AddSpGauge(AttackChara, DAMAGE_KICK);
			AddSpGauge(DefendChara, DAMAGE_KICK);
			// エフェクト

			// スコア
			AddScore(AttackChara, DAMAGE_KICK);
		}
		//　ヒットフラグの変更
		AttackChara->HitFrag = true;
		break;
	case Shoryu:
		// 敵の状態確認
		if (DefendChara->Animation->CurrentAnimID == Guard || DefendChara->Animation->CurrentAnimID == Down
			|| DefendChara->Animation->CurrentAnimID == Downpose || DefendChara->Animation->CurrentAnimID == Getup)
		{
			// ダメージ
			SubDamage(DefendChara, (int)(DAMAGE_SHORYU * 0.1f));
			// エフェクト

			// スコア
			AddScore(AttackChara, DAMAGE_SHORYU);
			AddScore(DefendChara, DAMAGE_SHORYU);
		}
		else
		{
			// 敵のモーション変更
			DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Down, ANIM_SPD_1);
			// ダメージ
			SubDamage(DefendChara, DAMAGE_SHORYU);
			// SPゲージ増減
			AddSpGauge(AttackChara, DAMAGE_SHORYU);
			AddSpGauge(DefendChara, DAMAGE_SHORYU);
			// エフェクト

			// スコア
			AddScore(AttackChara, DAMAGE_SHORYU);
		}
		//　ヒットフラグの変更
		AttackChara->HitFrag = true;
		break;
	default:
		break;
	}
}
//=============================================================================
// ダメージを与える
// 引数:CHARA:与える対象のキャラクタ sub :減少させる点数。マイナスも可能、初期化などに。
//=============================================================================
void SubDamage(CHARA *Chara, int sub)
{
	Chara->HPzan -= sub;

	//カンスト処理
	if (Chara->HPzan < 0)
	{
		Chara->HPzan = 0;
	}

}


//=============================================================================
// SPゲージの増加
// 引数:CHARA:増やす対象のキャラクタ add :追加する点数。マイナスも可能、初期化などに。
//=============================================================================
void AddSpGauge(CHARA *Chara, int add)
{
	Chara->SP += add;

	//カンスト処理
	if (Chara->SP >= FULL_SPGUAGE)
	{
		Chara->SP = FULL_SPGUAGE;
	}

}

//=============================================================================
// 波動拳の当たり判定及びダメージなどの処理
// 引数:AttackChara:攻撃側 DefendChara:防御側
//=============================================================================
void HitHadou(CHARA *AttackChara, CHARA *DefendChara)
{
	// 弾同士の当たり判定
	if (DefendChara->HadouBullet.use == true)
	{
		if (HitBC(AttackChara->HadouBullet.pos, DefendChara->HadouBullet.pos, AttackChara->HadouBullet.scl.x, DefendChara->HadouBullet.scl.x) == true)
		{
			// エフェクト
			// 使用フラグの変更
			AttackChara->HadouBullet.use = false;
			DefendChara->HadouBullet.use = false;

			// スコア
			AddScore(AttackChara, DAMAGE_HADOU);
			AddScore(DefendChara, DAMAGE_HADOU);
		}
	}
	// 弾と相手キャラとの当たり判定
	for (int i = 0; i < HIT_CHECK_NUM; i++)
	{
		if (HitBC(AttackChara->HadouBullet.pos, DefendChara->HitBall[i].pos, AttackChara->HadouBullet.scl.x, DefendChara->HitBall[i].scl.x) == true)
		{
			// 敵の状態確認
			if (DefendChara->Animation->CurrentAnimID == Guard || DefendChara->Animation->CurrentAnimID == Down
				|| DefendChara->Animation->CurrentAnimID == Downpose || DefendChara->Animation->CurrentAnimID == Getup)
			{
				// ダメージ
				SubDamage(DefendChara, (int)(DAMAGE_HADOU * 0.1f));

				// エフェクト

				// スコア
				AddScore(AttackChara, DAMAGE_HADOU);
				AddScore(DefendChara, DAMAGE_HADOU);
			}
			else
			{
				// 敵のモーション変更
				DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Damage, ANIM_SPD_1);
				// ダメージ
				SubDamage(DefendChara, DAMAGE_HADOU);

				// エフェクト

				// スコア
				AddScore(AttackChara, DAMAGE_HADOU);
			}
			// SPゲージ増減
			AddSpGauge(DefendChara, DAMAGE_HADOU);
			AddSpGauge(AttackChara, DAMAGE_HADOU);

			//　使用フラグの変更
			AttackChara->HadouBullet.use = false;
			break;
		}
	}
}

//=============================================================================
// 簡単操作	引数 Chara:キャラクタ構造体 ControllerNum:操作するコントローラ番号
//=============================================================================
void EasyInput(CHARA *Chara, int ControllerNum)
{
	switch (Chara->Animation->CurrentAnimID)
	{
	case Idle:
		// 移動
		// 前
		if (GetKeyboardPress(ControllerNum == 0 ? DIK_RIGHT : DIK_A) 
			|| IsButtonPressed(ControllerNum, BUTTON_RIGHT) || IsButtonPressed(ControllerNum, STICK_RIGHT))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Frontwalk, ANIM_SPD_2);
		}
		// 後ろ
		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_LEFT : DIK_D)
			|| IsButtonPressed(ControllerNum, BUTTON_LEFT) || IsButtonPressed(ControllerNum, STICK_LEFT))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Backwalk, ANIM_SPD_2);
		}
		// 手前
		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_DOWN : DIK_S)
			|| IsButtonPressed(ControllerNum, BUTTON_DOWN) || IsButtonPressed(ControllerNum, STICK_DOWN))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Rightstep, ANIM_SPD_2);
		}
		// 奥
		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_UP : DIK_W)
			|| IsButtonPressed(ControllerNum, BUTTON_UP) || IsButtonPressed(ControllerNum, STICK_UP))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Leftstep, ANIM_SPD_2);
		}

		// 攻撃
		// パンチ
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I)
			|| IsButtonTriggered(ControllerNum, BUTTON_A))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Punchi, ANIM_SPD_15);
		}
		// キック
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K)
			|| IsButtonTriggered(ControllerNum, BUTTON_B))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Kick, ANIM_SPD_15);
		}
		// 波動拳
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L) 
			|| IsButtonTriggered(ControllerNum, BUTTON_X))
		{
			// 発射されていない場合
			if (Chara->HadouBullet.use == false)
			{
				Chara->Animation->ChangeAnimation(Chara->Animation, Hadou, ANIM_SPD_2);
			}
		}
		// 昇竜拳
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O)
			|| IsButtonTriggered(ControllerNum, BUTTON_C))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Shoryu, ANIM_SPD_15);
		}
		// ガード
		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_G : DIK_P) 
			|| IsButtonPressed(ControllerNum, BUTTON_Y))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Guard, ANIM_SPD_1);
		}
		else
		{
		}
		break;
	case Frontwalk:
		// 前
		if (GetKeyboardPress(ControllerNum == 0 ? DIK_RIGHT : DIK_A)
			|| IsButtonPressed(ControllerNum, BUTTON_RIGHT) || IsButtonPressed(ControllerNum, STICK_RIGHT))
		{

		}
		// ボタンリリースで待機に戻る
		else
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, ANIM_SPD_1);
		}

		// 攻撃
		// パンチ
		if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I)
			|| IsButtonTriggered(ControllerNum, BUTTON_A))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Punchi, ANIM_SPD_15);
		}
		// キック
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K)
			|| IsButtonTriggered(ControllerNum, BUTTON_B))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Kick, ANIM_SPD_15);
		}
		// 波動拳
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L) 
			|| IsButtonTriggered(ControllerNum, BUTTON_X))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Hadou, ANIM_SPD_2);
		}
		// 昇竜拳
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O)
			|| IsButtonTriggered(ControllerNum, BUTTON_C))
		{
			// 発射されていない場合のみ
			if (Chara->HadouBullet.use == false)
			{
				Chara->Animation->ChangeAnimation(Chara->Animation, Shoryu, ANIM_SPD_15);
			}
		}
		// ガード
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_G : DIK_P) 
			|| IsButtonTriggered(ControllerNum, BUTTON_Y))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Guard, ANIM_SPD_1);
		}
		break;
	case Backwalk:
		// 後ろ
		if (GetKeyboardPress(ControllerNum == 0 ? DIK_LEFT : DIK_D) 
			|| IsButtonPressed(ControllerNum, BUTTON_LEFT) || IsButtonPressed(ControllerNum, STICK_LEFT))
		{

		}
		// ボタンリリースで待機に戻る
		else
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, ANIM_SPD_1);
		}

		// 攻撃
		// パンチ
		if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I)
			|| IsButtonTriggered(ControllerNum, BUTTON_A))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Punchi, ANIM_SPD_15);
		}
		// キック
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K) 
			|| IsButtonTriggered(ControllerNum, BUTTON_B))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Kick, ANIM_SPD_15);
		}
		// 波動拳
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L) 
			|| IsButtonTriggered(ControllerNum, BUTTON_X))
		{
			// 発射されていない場合のみ
			if (Chara->HadouBullet.use == false)
			{
				Chara->Animation->ChangeAnimation(Chara->Animation, Hadou, ANIM_SPD_2);
			}
		}
		// 昇竜拳
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O) 
			|| IsButtonTriggered(ControllerNum, BUTTON_C))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Shoryu, ANIM_SPD_15);
		}
		// ガード
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_G : DIK_P)
			|| IsButtonTriggered(ControllerNum, BUTTON_Y))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Guard, ANIM_SPD_1);
		}
		break;
	case Rightstep:
		// アニメーション終了で待機に戻る
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, ANIM_SPD_1);
		}
		break;
	case Leftstep:
		// アニメーション終了で待機に戻る
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, ANIM_SPD_1);
		}
		break;
	case Guard:
		if (GetKeyboardPress(ControllerNum == 0 ? DIK_G : DIK_P)
			|| IsButtonPressed(ControllerNum, BUTTON_Y))
		{

		}
		// ボタンリリースで待機に戻る
		else
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, ANIM_SPD_1);
		}

		// 攻撃
		// パンチ
		if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I)
			|| IsButtonTriggered(ControllerNum, BUTTON_A))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Punchi, ANIM_SPD_15);
		}
		// キック
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K) 
			|| IsButtonTriggered(ControllerNum, BUTTON_B))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Kick, ANIM_SPD_15);
		}
		// 波動拳
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L)
			|| IsButtonTriggered(ControllerNum, BUTTON_X))
		{
			if (Chara->HadouBullet.use == false)
			{
				Chara->Animation->ChangeAnimation(Chara->Animation, Hadou, ANIM_SPD_2);
			}
		}
		// 昇竜拳
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O)
			|| IsButtonTriggered(ControllerNum, BUTTON_C))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Shoryu, ANIM_SPD_15);
		}
	case Damage:
		// アニメーション終了で待機に戻る
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, ANIM_SPD_1);
			Chara->HadouBullet.frame = 0;
		}
		break;
	case Down:
		// アニメーション終了で起き上がりに移行
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Downpose, ANIM_SPD_1);
			// プレイヤー座標をモーション座標に合わせる
			D3DXMATRIXA16 newmatrix;	// モーション座標を取得するための行列
			newmatrix = GetBoneMatrix(Chara->Animation, "Hips");
			Chara->pos = D3DXVECTOR3(newmatrix._41, 0.0f, newmatrix._43);
		}
		break;
	case Downpose:
		// アニメーション終了で起き上がりに移行
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Getup, ANIM_SPD_15);
		}
	case Getup:
		// アニメーション終了で待機に戻る
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, ANIM_SPD_1);
			Chara->HadouBullet.frame = 0;
		}
		break;
	case Punchi:
		// アニメーション終了で待機に戻る
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, ANIM_SPD_1);
			Chara->HitFrag = false;
		}
		break;
	case Kick:
		// アニメーション終了で待機に戻る
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, ANIM_SPD_1);
			Chara->HitFrag = false;
		}
		break;
	case Hadou:
		// 一定フレーム経過で弾が出る
		Chara->HadouBullet.frame++;
		if (Chara->HadouBullet.frame == FIRE_FRAME)
		{
			SetHadou(&Chara->HadouBullet, Chara->HitBall, Chara->rot);
		}
		// アニメーション終了で待機に戻る
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, ANIM_SPD_1);
			Chara->HadouBullet.frame = 0;
			Chara->HitFrag = false;
		}
		break;
	case Shoryu:
		// アニメーション終了で待機に戻る
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, ANIM_SPD_1);
			Chara->HitFrag = false;
		}
		break;
	default:
		break;
	}
}

//=============================================================================
// スコアの追加
// 引数:add :追加する点数。マイナスも可能、初期化などに。
//=============================================================================
void AddScore(CHARA *Chara, int add)
{
	Chara->score += add;

	//カンスト処理
	if (Chara->score > FULL_SCORE)
	{
		Chara->score = FULL_SCORE;
	}
	else if (Chara->score < 0)
	{
		Chara->score = 0;
	}
}
