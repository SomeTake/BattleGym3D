//=============================================================================
//
// バトル処理（プレイヤーもエネミーも呼び出す処理） [battle.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "main.h"
#include "battle.h"
#include "input.h"
#include "camera.h"
#include "sound.h"

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
	float PEdistance = GetPEdistance();

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
	case Throw:
		if (PEdistance <= THROW_VALUE)
		{
			if (DefendChara->Animation->PreventAnimID != ThrowedPose)
			{
				DefendChara->Animation->ChangeAnimation(DefendChara->Animation, ThrowedPose, Data[ThrowedPose].Spd);
			}
			return true;
		}
		else
		{
			AttackChara->Animation->ChangeAnimation(AttackChara->Animation, Miss, Data[Miss].Spd);
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
			AddScore(AttackChara, Data[Punchi].Damage);
			AddScore(DefendChara, Data[Punchi].Damage);
			// 音
			PlaySound(SE_DEFEND0, 0, 0);
		}
		else
		{
			// 敵のモーション変更
			DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Damage, Data[Damage].Spd);
			// ダメージ
			SubDamage(DefendChara, Data[Punchi].Damage);
			// SPゲージ増減
			AddSpGauge(AttackChara, Data[Punchi].Damage);
			AddSpGauge(DefendChara, Data[Punchi].Damage);
			// エフェクト
			//SetEffect(&AttackChara->effect, AttackChara->HitBall[RightHand].pos, EFFECT_WIDTH, EFFECT_HEIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), hit00);
			// スコア
			AddScore(AttackChara, Data[Punchi].Damage);
			// 音
			PlaySound(SE_HIT0, 0, 0);
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
			AddScore(AttackChara, Data[Kick].Damage);
			AddScore(DefendChara, Data[Kick].Damage);
			// 音
			PlaySound(SE_DEFEND0, 0, 0);
		}
		else
		{
			// 敵のモーション変更
			DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Damage, Data[Damage].Spd);
			// ダメージ
			SubDamage(DefendChara, Data[Kick].Damage);
			// SPゲージ増減
			AddSpGauge(AttackChara, Data[Kick].Damage);
			AddSpGauge(DefendChara, Data[Kick].Damage);
			// エフェクト

			// スコア
			AddScore(AttackChara, Data[Kick].Damage);
			// 音
			PlaySound(SE_HIT0, 0, 0);
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
			SubDamage(DefendChara, (int)(Data[Shoryu].Damage * 0.1f));
			// エフェクト

			// スコア
			AddScore(AttackChara, Data[Shoryu].Damage);
			AddScore(DefendChara, Data[Shoryu].Damage);
			// 音
			PlaySound(SE_DEFEND0, 0, 0);
		}
		else
		{
			// 敵のモーション変更
			DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Down, Data[Down].Spd);
			// ダメージ
			SubDamage(DefendChara, Data[Shoryu].Damage);
			// SPゲージ増減
			AddSpGauge(AttackChara, Data[Shoryu].Damage);
			AddSpGauge(DefendChara, Data[Shoryu].Damage);
			// エフェクト

			// スコア
			AddScore(AttackChara, Data[Shoryu].Damage);
			// 音
			PlaySound(SE_HIT1, 0, 0);
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
	else if (Chara->HPzan >= FULL_HP)
	{
		Chara->HPzan = FULL_HP;
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
	if (Chara->SP >= FULL_SPGAUGE)
	{
		Chara->SP = FULL_SPGAUGE;
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
			AddScore(AttackChara, Data[Hadou].Damage);
			AddScore(DefendChara, Data[Hadou].Damage);

			// 音
			PlaySound(SE_HIT1, 0, 0);
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
				SubDamage(DefendChara, (int)(Data[Hadou].Damage * 0.1f));

				// エフェクト

				// スコア
				AddScore(AttackChara, Data[Hadou].Damage);
				AddScore(DefendChara, Data[Hadou].Damage);

				// 音
				PlaySound(SE_DEFEND0, 0, 0);
			}
			else
			{
				// 敵のモーション変更
				DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Damage, Data[Damage].Spd);
				// ダメージ
				SubDamage(DefendChara, Data[Hadou].Damage);

				// エフェクト

				// スコア
				AddScore(AttackChara, Data[Hadou].Damage);

				// 音
				PlaySound(SE_HIT0, 0, 0);
			}
			// SPゲージ増減
			AddSpGauge(DefendChara, Data[Hadou].Damage);
			AddSpGauge(AttackChara, Data[Hadou].Damage);

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
	int *phase = GetPhase();

	// ポーズ画面へ
	if (*phase == PhaseTutorial)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(ControllerNum, BUTTON_M))
		{
			SetPhase(PhaseTitle);
			StopSound(BGM_TUTORIAL, 0);
			PlaySound(BGM_TITLE, 1, 1);
			ReInit();
		}
	}
	else if (*phase == PhaseGame)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(ControllerNum, BUTTON_M))
		{
			SetPhase(PhasePause);
			StopSound(BGM_BATTLE, 0);
		}
	}
	else if (*phase == PhaseTraining)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(ControllerNum, BUTTON_M))
		{
			SetPhase(PhaseTrainingPause);
			StopSound(BGM_TRAINING, 0);
		}
	}


	if (Chara->graceflag == true)
	{
		Chara->graceframe++;
		// 入力猶予範囲内に追加入力がなされた場合
		if (Chara->graceframe < GRACE_VALUE)
		{
			switch (Chara->gracetype)
			{
			case Punchi:
				if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K)
					|| IsButtonTriggered(ControllerNum, BUTTON_B))
				{
					Chara->Animation->ChangeAnimation(Chara->Animation, Throw, Data[Throw].Spd);
					Chara->graceflag = false;
					Chara->graceframe = 0;
				}
				break;
			case Kick:
				if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I)
					|| IsButtonTriggered(ControllerNum, BUTTON_A))
				{
					Chara->Animation->ChangeAnimation(Chara->Animation, Throw, Data[Throw].Spd);
					Chara->graceflag = false;
					Chara->graceframe = 0;
				}
				break;
			case Hadou:
				if (Chara->SP == FULL_SPGAUGE)
				{
					if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O)
						|| IsButtonTriggered(ControllerNum, BUTTON_C))
					{
						Chara->Animation->ChangeAnimation(Chara->Animation, SPattack, Data[SPattack].Spd);
						Chara->graceflag = false;
						Chara->graceframe = 0;
					}
				}
				break;
			case Shoryu:
				if (Chara->SP == FULL_SPGAUGE)
				{
					if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L)
						|| IsButtonTriggered(ControllerNum, BUTTON_X))
					{
						Chara->Animation->ChangeAnimation(Chara->Animation, SPattack, Data[SPattack].Spd);
						Chara->graceflag = false;
						Chara->graceframe = 0;
					}
				}
				break;
			default:
				break;
			}
		}
		// 追加入力がされなかった場合
		else if(Chara->graceframe >= GRACE_VALUE)
		{
			switch (Chara->gracetype)
			{
			case Punchi:
				Chara->Animation->ChangeAnimation(Chara->Animation, Punchi, Data[Punchi].Spd);
				Chara->graceflag = false;
				Chara->graceframe = 0;
				break;
			case Kick:
				Chara->Animation->ChangeAnimation(Chara->Animation, Kick, Data[Kick].Spd);
				Chara->graceflag = false;
				Chara->graceframe = 0;
				break;
			case Hadou:
				// 発射されていない場合
				if (Chara->HadouBullet.use == false)
				{
					Chara->Animation->ChangeAnimation(Chara->Animation, Hadou, Data[Hadou].Spd);
					Chara->graceflag = false;
					Chara->graceframe = 0;
				}
				break;
			case Shoryu:
				Chara->Animation->ChangeAnimation(Chara->Animation, Shoryu, Data[Shoryu].Spd);
				Chara->graceflag = false;
				Chara->graceframe = 0;
				break;
			default:
				break;
			}
		}
	}

	switch (Chara->Animation->CurrentAnimID)
	{
	case Idle:
		// 移動
		// 前
		if (GetKeyboardPress(ControllerNum == 0 ? DIK_RIGHT : DIK_A) 
			|| IsButtonPressed(ControllerNum, BUTTON_RIGHT) || IsButtonPressed(ControllerNum, STICK_RIGHT))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Frontwalk, Data[Frontwalk].Spd);
		}
		// 後ろ
		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_LEFT : DIK_D)
			|| IsButtonPressed(ControllerNum, BUTTON_LEFT) || IsButtonPressed(ControllerNum, STICK_LEFT))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Backwalk, Data[Backwalk].Spd);
		}
		// 手前
		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_DOWN : DIK_S)
			|| IsButtonPressed(ControllerNum, BUTTON_DOWN) || IsButtonPressed(ControllerNum, STICK_DOWN))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Rightstep, Data[Rightstep].Spd);
		}
		// 奥
		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_UP : DIK_W)
			|| IsButtonPressed(ControllerNum, BUTTON_UP) || IsButtonPressed(ControllerNum, STICK_UP))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Leftstep, Data[Leftstep].Spd);
		}

		// 攻撃
		// 投げ
		else if ((GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I) && GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K))
			|| (IsButtonTriggered(ControllerNum, BUTTON_A) && IsButtonTriggered(ControllerNum, BUTTON_B)))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Throw, Data[Throw].Spd);
		}
		// SP技
		else if (Chara->SP == FULL_SPGAUGE &&
			((GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O) && GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L))
			|| (IsButtonTriggered(ControllerNum, BUTTON_C) && IsButtonTriggered(ControllerNum, BUTTON_X))))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, SPattack, Data[SPattack].Spd);
		}
		// パンチ
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I)
			|| IsButtonTriggered(ControllerNum, BUTTON_A))
		{
			Chara->graceflag = true;
			Chara->gracetype = Punchi;
		}
		// キック
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K)
			|| IsButtonTriggered(ControllerNum, BUTTON_B))
		{
			Chara->graceflag = true;
			Chara->gracetype = Kick;
		}
		// 波動拳
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L) 
			|| IsButtonTriggered(ControllerNum, BUTTON_X))
		{
			Chara->graceflag = true;
			Chara->gracetype = Hadou;
		}
		// 昇竜拳
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O)
			|| IsButtonTriggered(ControllerNum, BUTTON_C))
		{
			Chara->graceflag = true;
			Chara->gracetype = Shoryu;
		}
		// ガード
		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_G : DIK_P) 
			|| IsButtonPressed(ControllerNum, BUTTON_Y))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Guard, Data[Guard].Spd);
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
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		}

		// 攻撃
		// 投げ
		if ((GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I) && GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K))
			|| (IsButtonTriggered(ControllerNum, BUTTON_A) && IsButtonTriggered(ControllerNum, BUTTON_B)))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Throw, Data[Throw].Spd);
		}
		// SP技
		else if (Chara->SP == FULL_SPGAUGE &&
			((GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O) && GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L))
			|| (IsButtonTriggered(ControllerNum, BUTTON_C) && IsButtonTriggered(ControllerNum, BUTTON_X))))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, SPattack, Data[SPattack].Spd);
		}
		// パンチ
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I)
			|| IsButtonTriggered(ControllerNum, BUTTON_A))
		{
			Chara->graceflag = true;
			Chara->gracetype = Punchi;
		}
		// キック
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K)
			|| IsButtonTriggered(ControllerNum, BUTTON_B))
		{
			Chara->graceflag = true;
			Chara->gracetype = Kick;
		}
		// 波動拳
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L) 
			|| IsButtonTriggered(ControllerNum, BUTTON_X))
		{
			// 発射されていない場合のみ
			if (Chara->HadouBullet.use == false)
			{
				Chara->graceflag = true;
				Chara->gracetype = Hadou;
			}
		}
		// 昇竜拳
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O)
			|| IsButtonTriggered(ControllerNum, BUTTON_C))
		{
			Chara->graceflag = true;
			Chara->gracetype = Shoryu;
		}
		// ガード
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_G : DIK_P) 
			|| IsButtonTriggered(ControllerNum, BUTTON_Y))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Guard, Data[Guard].Spd);
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
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		}

		// 攻撃
		// 投げ
		if ((GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I) && GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K))
			|| (IsButtonTriggered(ControllerNum, BUTTON_A) && IsButtonTriggered(ControllerNum, BUTTON_B)))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Throw, Data[Throw].Spd);
		}
		// SP技
		else if (Chara->SP == FULL_SPGAUGE &&
			((GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O) && GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L))
				|| (IsButtonTriggered(ControllerNum, BUTTON_C) && IsButtonTriggered(ControllerNum, BUTTON_X))))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, SPattack, Data[SPattack].Spd);
		}
		// パンチ
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I)
			|| IsButtonTriggered(ControllerNum, BUTTON_A))
		{
			Chara->graceflag = true;
			Chara->gracetype = Punchi;
		}
		// キック
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K) 
			|| IsButtonTriggered(ControllerNum, BUTTON_B))
		{
			Chara->graceflag = true;
			Chara->gracetype = Kick;
		}
		// 波動拳
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L) 
			|| IsButtonTriggered(ControllerNum, BUTTON_X))
		{
			// 発射されていない場合のみ
			if (Chara->HadouBullet.use == false)
			{
				Chara->graceflag = true;
				Chara->gracetype = Hadou;
			}
		}
		// 昇竜拳
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O) 
			|| IsButtonTriggered(ControllerNum, BUTTON_C))
		{
			Chara->graceflag = true;
			Chara->gracetype = Shoryu;
		}
		// ガード
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_G : DIK_P)
			|| IsButtonTriggered(ControllerNum, BUTTON_Y))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Guard, Data[Guard].Spd);
		}
		break;
	case Rightstep:
		// アニメーション終了で待機に戻る
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		}
		break;
	case Leftstep:
		// アニメーション終了で待機に戻る
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
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
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		}

		// 攻撃
		// 投げ
		if ((GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I) && GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K))
			|| (IsButtonTriggered(ControllerNum, BUTTON_A) && IsButtonTriggered(ControllerNum, BUTTON_B)))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Throw, Data[Throw].Spd);
		}
		// SP技
		else if (Chara->SP == FULL_SPGAUGE &&
			((GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O) && GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L))
				|| (IsButtonTriggered(ControllerNum, BUTTON_C) && IsButtonTriggered(ControllerNum, BUTTON_X))))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, SPattack, Data[SPattack].Spd);
		}
		// パンチ
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_J : DIK_I)
			|| IsButtonTriggered(ControllerNum, BUTTON_A))
		{
			Chara->graceflag = true;
			Chara->gracetype = Punchi;
		}
		// キック
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_M : DIK_K) 
			|| IsButtonTriggered(ControllerNum, BUTTON_B))
		{
			Chara->graceflag = true;
			Chara->gracetype = Kick;
		}
		// 波動拳
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_N : DIK_L)
			|| IsButtonTriggered(ControllerNum, BUTTON_X))
		{
			if (Chara->HadouBullet.use == false)
			{
				Chara->graceflag = true;
				Chara->gracetype = Hadou;
			}
		}
		// 昇竜拳
		else if (GetKeyboardTrigger(ControllerNum == 0 ? DIK_H : DIK_O)
			|| IsButtonTriggered(ControllerNum, BUTTON_C))
		{
			Chara->graceflag = true;
			Chara->gracetype = Shoryu;
		}
	case Damage:
		// アニメーション終了で待機に戻る
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
			Chara->HadouBullet.frame = 0;
		}
		break;
	case Down:
		// アニメーション終了で起き上がりに移行
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Downpose, Data[Downpose].Spd);
			// モーション座標にキャラクター座標を合わせる
			Chara->pos = D3DXVECTOR3(Chara->HitBall[Hips].pos.x, 0.0f, Chara->HitBall[Hips].pos.z);
		}
		break;
	case Downpose:
		// アニメーション終了で起き上がりに移行
		if (Chara->Animation->MotionEnd == true && Chara->HPzan > 0)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Getup, Data[Getup].Spd);
		}
	case Getup:
		// アニメーション終了で待機に戻る
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
			Chara->HadouBullet.frame = 0;
		}
		break;
	case Punchi:
		// アニメーション終了で待機に戻る
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
			Chara->HitFrag = false;
		}
		break;
	case Kick:
		// アニメーション終了で待機に戻る
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
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
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
			Chara->HadouBullet.frame = 0;
			Chara->HitFrag = false;
		}
		break;
	case Shoryu:
		// アニメーション終了で待機に戻る
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
			Chara->HitFrag = false;
		}
		break;
	case SPattack:
		// アニメーション終了で待機に戻る
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
			// SPゲージ消費
			AddSpGauge(Chara, -FULL_SPGAUGE);
			// モーション座標にキャラクター座標を合わせる
			Chara->pos = D3DXVECTOR3(Chara->HitBall[Hips].pos.x, 0.0f, Chara->HitBall[Hips].pos.z);
			Chara->HitFrag = false;
		}
		break;
	case Throw:
		// アニメーション終了で待機に戻る
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
			Chara->HitFrag = false;
		}
		break;
	case Win:
		break;
	case Miss:
		// アニメーション終了で待機に戻る
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
			Chara->HitFrag = false;
		}
		break;
	case ThrowedPose:
		if (Chara->Animation->PreventAnimID != ThrowedPose)
		{
			Chara->framecount++;
		}
		// 一定時間経過で相手の投げアニメーションに合わせてダウンモーションに移行
		if (Chara->framecount == THROW_FRAME)
		{
			SubDamage(Chara, Data[Throw].Damage);
			Chara->Animation->ChangeAnimation(Chara->Animation, Down, Data[Down].Spd);
			Chara->framecount = 0;
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

//=============================================================================
// バトル用AI 条件1:距離 条件2:相手の状態 によって状況を判断する
//=============================================================================
void BattleAI(CHARA *AIChara, CHARA *AnotherChara)
{
	float PEdist = GetPEdistance();
	bool AttackFlag = false;

	// 相手の攻撃判定
	if (AnotherChara->Animation->CurrentAnimID == Punchi
		|| AnotherChara->Animation->CurrentAnimID == Kick
		|| AnotherChara->Animation->CurrentAnimID == Shoryu
		|| AnotherChara->Animation->CurrentAnimID == SPattack
		|| AnotherChara->HadouBullet.use == true)
	{
		AttackFlag = true;
	}
	else
	{
		AttackFlag = false;
	}

	int ActRand = rand() % 300;

	switch (AIChara->Animation->CurrentAnimID)
	{
	case Idle:
		if (AttackFlag == true)
		{
			switch(ActRand)
			{
			case 0:
				AIChara->Animation->ChangeAnimation(AIChara->Animation, Guard, Data[Guard].Spd);
				break;
			case 1:
				AIChara->Animation->ChangeAnimation(AIChara->Animation, Rightstep, Data[Rightstep].Spd);
				break;
			case 2:
				AIChara->Animation->ChangeAnimation(AIChara->Animation, Leftstep, Data[Leftstep].Spd);
				break;
			default:
				break;
			}
		}
		else
		{
			// 近距離
			if (PEdist < MAX_DISTANCE * 0.2f)
			{
				switch (ActRand)
				{
				case 0:
					AIChara->Animation->ChangeAnimation(AIChara->Animation, Kick, Data[Kick].Spd);
					break;
				case 1:
					AIChara->Animation->ChangeAnimation(AIChara->Animation, Punchi, Data[Punchi].Spd);
					break;
				case 2:
					AIChara->Animation->ChangeAnimation(AIChara->Animation, Hadou, Data[Hadou].Spd);
					break;
				case 3:
					AIChara->Animation->ChangeAnimation(AIChara->Animation, Shoryu, Data[Shoryu].Spd);
					break;
				case 4:
					AIChara->Animation->ChangeAnimation(AIChara->Animation, Throw, Data[Throw].Spd);
					break;
				case 5:
					if (AIChara->SP == FULL_SPGAUGE)
					{
						AIChara->Animation->ChangeAnimation(AIChara->Animation, SPattack, Data[SPattack].Spd);
					}
					break;
				default:
					break;
				}
			}
			// 中距離
			else if (PEdist < MAX_DISTANCE * 0.5f || PEdist > MAX_DISTANCE * 0.2f)
			{
				if (ActRand == 0 && AIChara->HadouBullet.use == false)
				{
					AIChara->Animation->ChangeAnimation(AIChara->Animation, Hadou, Data[Hadou].Spd);
				}
				else if(ActRand == 1)
				{
					AIChara->Animation->ChangeAnimation(AIChara->Animation, Frontwalk, Data[Frontwalk].Spd);
				}
			}
			// 遠距離
			else
			{
				AIChara->Animation->ChangeAnimation(AIChara->Animation, Frontwalk, Data[Frontwalk].Spd);
			}
		}
		break;
	case Frontwalk:
		if (PEdist < MAX_DISTANCE * 0.5f || PEdist > MAX_DISTANCE * 0.2f)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
		}
		break;
	case Backwalk:
		if (PEdist > MAX_DISTANCE * 0.5f)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
		}
		break;
	case Rightstep:
		// アニメーション終了で待機に戻る
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
		}
		break;
	case Leftstep:
		// アニメーション終了で待機に戻る
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
		}
		break;
	case Guard:
		if (AttackFlag == false)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
		}
		break;
	case Damage:
		// アニメーション終了で待機に戻る
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
			AIChara->HadouBullet.frame = 0;
		}
		break;
	case Down:
		// アニメーション終了で起き上がりに移行
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Downpose, Data[Downpose].Spd);
			// モーション座標にキャラクター座標を合わせる
			AIChara->pos = D3DXVECTOR3(AIChara->HitBall[Hips].pos.x, 0.0f, AIChara->HitBall[Hips].pos.z);
		}
		break;
	case Downpose:
		// アニメーション終了で起き上がりに移行
		if (AIChara->Animation->MotionEnd == true && AIChara->HPzan > 0)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Getup, Data[Getup].Spd);
		}
	case Getup:
		// アニメーション終了で待機に戻る
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
			AIChara->HadouBullet.frame = 0;
		}
		break;
	case Punchi:
		// アニメーション終了で待機に戻る
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
			AIChara->HitFrag = false;
		}
		break;
	case Kick:
		// アニメーション終了で待機に戻る
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
			AIChara->HitFrag = false;
		}
		break;
	case Hadou:
		// 一定フレーム経過で弾が出る
		AIChara->HadouBullet.frame++;
		if (AIChara->HadouBullet.frame == FIRE_FRAME)
		{
			SetHadou(&AIChara->HadouBullet, AIChara->HitBall, AIChara->rot);
		}
		// アニメーション終了で待機に戻る
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
			AIChara->HadouBullet.frame = 0;
			AIChara->HitFrag = false;
		}
		break;
	case Shoryu:
		// アニメーション終了で待機に戻る
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
			AIChara->HitFrag = false;
		}
		break;
	case SPattack:
		// アニメーション終了で待機に戻る
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
			// SPゲージ消費
			AddSpGauge(AIChara, -FULL_SPGAUGE);
			// モーション座標にキャラクター座標を合わせる
			AIChara->pos = D3DXVECTOR3(AIChara->HitBall[Hips].pos.x, 0.0f, AIChara->HitBall[Hips].pos.z);
			AIChara->HitFrag = false;
		}
		break;
	case Throw:
		// アニメーション終了で待機に戻る
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
			AIChara->HitFrag = false;
		}
		break;
	case Win:
		break;
	case Miss:
		// アニメーション終了で待機に戻る
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
			AIChara->HitFrag = false;
		}
		break;
	case ThrowedPose:
		if (AIChara->Animation->PreventAnimID != ThrowedPose)
		{
			AIChara->framecount++;
		}
		// 一定時間経過で相手の投げアニメーションに合わせてダウンモーションに移行
		if (AIChara->framecount == THROW_FRAME)
		{
			SubDamage(AIChara, Data[Throw].Damage);
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Down, Data[Down].Spd);
			AIChara->framecount = 0;
		}
		break;
	default:
		AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Idle);
		break;
	}

}

//=============================================================================
// チュートリアルモード
//=============================================================================
void SetupTutorial(CHARA *Chara, REDGAUGE *Gauge)
{
	if (Chara->HPzan <= 0)
	{
		Chara->HPzan = 0;
	}

	if (Chara->HPzan == Gauge->value)
	{
		Chara->damagecount++;
		if (Chara->damagecount >= 180)
		{
			if (Chara->HP > Chara->HPzan)
			{
				Chara->HPzan += HPHEAL;
				Gauge->value += HPHEAL;
			}
		}
	}
	else
	{
		Chara->damagecount = 0;
	}

	Chara->SP = FULL_SPGAUGE;

}