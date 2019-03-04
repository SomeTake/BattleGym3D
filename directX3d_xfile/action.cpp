//=============================================================================
//
// アクション処理 [action.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "main.h"
#include "action.h"
#include "battle.h"
#include "particle.h"
#include "game.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//=====================================================================================================
// アイドル時のアクション
//=====================================================================================================
void IdleAction(CHARA *Chara, int ControllerNum)
{
	// コマンド入力
	if (Chara->CommandInput == true)
	{
		// 入力されたコマンドに応じてアニメーションを変更させる
		// 移動
		if (CheckInput(Chara->Input, CMD_Frontwalk) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Frontwalk, Data[Frontwalk].Spd);
		}
		else if (CheckInput(Chara->Input, CMD_Backwalk) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Backwalk, Data[Backwalk].Spd);
		}
		else if (CheckInput(Chara->Input, CMD_Rightstep) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Rightstep, Data[Rightstep].Spd);
		}
		else if (CheckInput(Chara->Input, CMD_Leftstep) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Leftstep, Data[Leftstep].Spd);
		}

		// 攻撃
		// SP技
		else if (CheckInput(Chara->Input, CMD_SPattack) == true && Chara->SP == FULL_SPGAUGE)
		{
			// SPゲージ消費
			PlaySound(SE_EFFECT0);
			AddSpGauge(Chara, -FULL_SPGAUGE);
			Chara->Animation->ChangeAnimation(Chara->Animation, SPattack, Data[SPattack].Spd);
		}
		// 投げ
		else if (CheckInput(Chara->Input, CMD_Throw) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Throw, Data[Throw].Spd);
		}
		// 波動拳
		else if ((CheckInput(Chara->Input, CMD_Hadou) == true || CheckInput(Chara->Input, CMD_Hadou1) == true)
			&& Chara->HadouBullet.use == false)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Hadou, Data[Hadou].Spd);
		}
		// 昇竜拳
		else if (CheckInput(Chara->Input, CMD_Shoryu) == true || CheckInput(Chara->Input, CMD_Shoryu1) == true
			|| CheckInput(Chara->Input, CMD_Shoryu2) == true || CheckInput(Chara->Input, CMD_Shoryu3) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Shoryu, Data[Shoryu].Spd);
		}
		// パンチ
		else if (CheckInput(Chara->Input, CMD_Punchi) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Punchi, Data[Punchi].Spd);
		}
		// キック
		else if (CheckInput(Chara->Input, CMD_Kick) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Kick, Data[Kick].Spd);
		}
		// ガード
		else if (CheckInput(Chara->Input, CMD_Guard) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Guard, Data[Guard].Spd);
		}

	}

	// イージー入力
	else
	{
		// 移動
		// 前
		if (GetKeyboardPress(ControllerNum == 0 ? DIK_RIGHT : DIK_A)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? BUTTON_RIGHT : BUTTON_LEFT)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? STICK_RIGHT : STICK_LEFT))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Frontwalk, Data[Frontwalk].Spd);
		}
		// 後ろ
		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_LEFT : DIK_D)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? BUTTON_LEFT : BUTTON_RIGHT)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? STICK_LEFT : STICK_RIGHT))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Backwalk, Data[Backwalk].Spd);
		}
		// 手前
		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_DOWN : DIK_S)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? BUTTON_DOWN : BUTTON_UP)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? STICK_DOWN : STICK_UP))
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Rightstep, Data[Rightstep].Spd);
		}
		// 奥
		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_UP : DIK_W)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? BUTTON_UP : BUTTON_DOWN)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? STICK_UP : STICK_DOWN))
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
			// SPゲージ消費
			PlaySound(SE_EFFECT0);
			AddSpGauge(Chara, -FULL_SPGAUGE);
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
	}
}

//=====================================================================================================
// 前歩き時のアクション
//=====================================================================================================
void FrontwalkAction(CHARA *Chara, int ControllerNum)
{
	// コマンド入力
	if (Chara->CommandInput == true)
	{
		// 入力されたコマンドに応じてアニメーションを変更させる
		// 攻撃
		// SP技
		if (CheckInput(Chara->Input, CMD_SPattack) == true && Chara->SP == FULL_SPGAUGE)
		{
			// SPゲージ消費
			PlaySound(SE_EFFECT0);
			AddSpGauge(Chara, -FULL_SPGAUGE);
			Chara->Animation->ChangeAnimation(Chara->Animation, SPattack, Data[SPattack].Spd);
		}
		// 投げ
		else if (CheckInput(Chara->Input, CMD_Throw) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Throw, Data[Throw].Spd);
		}
		// 波動拳
		else if ((CheckInput(Chara->Input, CMD_Hadou) == true || CheckInput(Chara->Input, CMD_Hadou1) == true)
			&& Chara->HadouBullet.use == false)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Hadou, Data[Hadou].Spd);
		}
		// 昇竜拳
		else if (CheckInput(Chara->Input, CMD_Shoryu) == true || CheckInput(Chara->Input, CMD_Shoryu1) == true
			|| CheckInput(Chara->Input, CMD_Shoryu2) == true || CheckInput(Chara->Input, CMD_Shoryu3) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Shoryu, Data[Shoryu].Spd);
		}
		// パンチ
		else if (CheckInput(Chara->Input, CMD_Punchi) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Punchi, Data[Punchi].Spd);
		}
		// キック
		else if (CheckInput(Chara->Input, CMD_Kick) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Kick, Data[Kick].Spd);
		}
		// ガード
		else if (CheckInput(Chara->Input, CMD_Guard) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Guard, Data[Guard].Spd);
		}

		// 移動
		else if (CheckInput(Chara->Input, CMD_Frontwalk) == true)
		{
		}
		else if (CheckInput(Chara->Input, CMD_Backwalk) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Backwalk, Data[Backwalk].Spd);
		}
		else if (CheckInput(Chara->Input, CMD_Rightstep) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Rightstep, Data[Rightstep].Spd);
		}
		else if (CheckInput(Chara->Input, CMD_Leftstep) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Leftstep, Data[Leftstep].Spd);
		}

		// キーリリースで待機に戻る
		else
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		}
	}

	// イージー入力
	else
	{
		// 前
		if (GetKeyboardPress(ControllerNum == 0 ? DIK_RIGHT : DIK_A)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? BUTTON_RIGHT : BUTTON_LEFT)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? STICK_RIGHT : STICK_LEFT))
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
			// SPゲージ消費
			PlaySound(SE_EFFECT0);
			AddSpGauge(Chara, -FULL_SPGAUGE);
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
	}
}

//=====================================================================================================
// 後ろ歩き時のアクション
//=====================================================================================================
void BackwalkAction(CHARA *Chara, int ControllerNum)
{
	// コマンド入力
	if (Chara->CommandInput == true)
	{
		// 入力されたコマンドに応じてアニメーションを変更させる
		// 攻撃
		// SP技
		if (CheckInput(Chara->Input, CMD_SPattack) == true && Chara->SP == FULL_SPGAUGE)
		{
			// SPゲージ消費
			PlaySound(SE_EFFECT0);
			AddSpGauge(Chara, -FULL_SPGAUGE);
			Chara->Animation->ChangeAnimation(Chara->Animation, SPattack, Data[SPattack].Spd);
		}
		// 投げ
		else if (CheckInput(Chara->Input, CMD_Throw) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Throw, Data[Throw].Spd);
		}
		// 波動拳
		else if ((CheckInput(Chara->Input, CMD_Hadou) == true || CheckInput(Chara->Input, CMD_Hadou1) == true)
			&& Chara->HadouBullet.use == false)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Hadou, Data[Hadou].Spd);
		}
		// 昇竜拳
		else if (CheckInput(Chara->Input, CMD_Shoryu) == true || CheckInput(Chara->Input, CMD_Shoryu1) == true
			|| CheckInput(Chara->Input, CMD_Shoryu2) == true || CheckInput(Chara->Input, CMD_Shoryu3) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Shoryu, Data[Shoryu].Spd);
		}
		// パンチ
		else if (CheckInput(Chara->Input, CMD_Punchi) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Punchi, Data[Punchi].Spd);
		}
		// キック
		else if (CheckInput(Chara->Input, CMD_Kick) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Kick, Data[Kick].Spd);
		}
		// ガード
		else if (CheckInput(Chara->Input, CMD_Guard) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Guard, Data[Guard].Spd);
		}

		// 移動
		else if (CheckInput(Chara->Input, CMD_Frontwalk) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Frontwalk, Data[Frontwalk].Spd);
		}
		else if (CheckInput(Chara->Input, CMD_Backwalk) == true)
		{
		}
		else if (CheckInput(Chara->Input, CMD_Rightstep) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Rightstep, Data[Rightstep].Spd);
		}
		else if (CheckInput(Chara->Input, CMD_Leftstep) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Leftstep, Data[Leftstep].Spd);
		}

		// キーリリースで待機に戻る
		else
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		}
	}

	// イージー入力
	else
	{
		// 後ろ
		if (GetKeyboardPress(ControllerNum == 0 ? DIK_LEFT : DIK_D)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? BUTTON_LEFT : BUTTON_RIGHT)
			|| IsButtonPressed(ControllerNum, ControllerNum == 0 ? STICK_LEFT : STICK_RIGHT))
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
			// SPゲージ消費
			PlaySound(SE_EFFECT0);
			AddSpGauge(Chara, -FULL_SPGAUGE);
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
	}
}

//=====================================================================================================
// 右ステップ時のアクション
//=====================================================================================================
void RightstepAction(CHARA *Chara)
{
	// アニメーション終了で待機に戻る
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HitFrag = false;
	}
}

//=====================================================================================================
// 左ステオップ時のアクション
//=====================================================================================================
void LeftstepAction(CHARA *Chara)
{
	// アニメーション終了で待機に戻る
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HitFrag = false;
	}
}

//=====================================================================================================
// ガード時のアクション
//=====================================================================================================
void GuardAction(CHARA *Chara, int ControllerNum)
{
	// コマンド入力
	if (Chara->CommandInput == true)
	{
		// 攻撃
		// SP技
		if (CheckInput(Chara->Input, CMD_SPattack) == true && Chara->SP == FULL_SPGAUGE)
		{
			// SPゲージ消費
			PlaySound(SE_EFFECT0);
			AddSpGauge(Chara, -FULL_SPGAUGE);
			Chara->Animation->ChangeAnimation(Chara->Animation, SPattack, Data[SPattack].Spd);
		}
		// 投げ
		else if (CheckInput(Chara->Input, CMD_Throw) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Throw, Data[Throw].Spd);
		}
		// 波動拳
		else if ((CheckInput(Chara->Input, CMD_Hadou) == true || CheckInput(Chara->Input, CMD_Hadou1) == true)
			&& Chara->HadouBullet.use == false)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Hadou, Data[Hadou].Spd);
		}
		// 昇竜拳
		else if (CheckInput(Chara->Input, CMD_Shoryu) == true || CheckInput(Chara->Input, CMD_Shoryu1) == true
			|| CheckInput(Chara->Input, CMD_Shoryu2) == true || CheckInput(Chara->Input, CMD_Shoryu3) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Shoryu, Data[Shoryu].Spd);
		}
		// パンチ
		else if (CheckInput(Chara->Input, CMD_Punchi) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Punchi, Data[Punchi].Spd);
		}
		// キック
		else if (CheckInput(Chara->Input, CMD_Kick) == true)
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Kick, Data[Kick].Spd);
		}
		else if (CheckInput(Chara->Input, CMD_Guard) == true)
		{
		}
		// キーリリースで待機に戻る
		else
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		}
	}

	// イージー入力
	else
	{
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
			// SPゲージ消費
			PlaySound(SE_EFFECT0);
			AddSpGauge(Chara, -FULL_SPGAUGE);
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

		else if (GetKeyboardPress(ControllerNum == 0 ? DIK_G : DIK_P)
			|| IsButtonPressed(ControllerNum, BUTTON_Y))
		{

		}
		// ボタンリリースで待機に戻る
		else
		{
			Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		}
	}

}

//=====================================================================================================
// 被ダメージ時のアクション
//=====================================================================================================
void DamageAction(CHARA *Chara)
{
	// アニメーション終了で待機に戻る
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HadouBullet.frame = 0;
		Chara->graceframe = 0;
	}
}

//=====================================================================================================
// ダウン時のアクション
//=====================================================================================================
void DownAction(CHARA *Chara)
{
	// アニメーション終了で起き上がりに移行
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Downpose, Data[Downpose].Spd);
		// モーション座標にキャラクター座標を合わせる
		Chara->pos = D3DXVECTOR3(Chara->HitBall[Hips].pos.x, 0.0f, Chara->HitBall[Hips].pos.z);
	}
}

//=====================================================================================================
// ダウンポーズ時のアクション
//=====================================================================================================
void DownposeAction(CHARA *Chara)
{
	// アニメーション終了で起き上がりに移行
	if (Chara->Animation->MotionEnd == true && Chara->HPzan > 0)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Getup, Data[Getup].Spd);
	}
}

//=====================================================================================================
// 起き上がり時のアクション
//=====================================================================================================
void GetupAction(CHARA *Chara)
{
	// アニメーション終了で待機に戻る
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HadouBullet.frame = 0;
		Chara->graceframe = 0;
		Chara->HitFrag = false;
	}
}

//=====================================================================================================
// パンチ時のアクション
//=====================================================================================================
void PunchiAction(CHARA *Chara, int ControllerNum)
{
	SetAttackParticle(Chara->HitBall[LeftHand].pos);

	// ヒット時追加入力でストレートに移行
	if (Chara->HitFrag == true)
	{

		// コマンド入力
		if (Chara->CommandInput == true)
		{
			if (CheckInput(Chara->Input, CMD_Straight))
			{
				Chara->Animation->ChangeAnimation(Chara->Animation, Straight, Data[Straight].Spd);
				D3DXMATRIX Mtx = GetBoneMatrix(Chara->Animation, CharaHitPos[Hips]);
				Chara->pos = D3DXVECTOR3(Mtx._41, 0.0f, Mtx._43);
				Chara->HitFrag = false;
				Chara->framecount = 0;
			}
		}

		// イージー入力
		else
		{
			if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(ControllerNum, BUTTON_A))
			{
				Chara->Animation->ChangeAnimation(Chara->Animation, Straight, Data[Straight].Spd);
				D3DXMATRIX Mtx = GetBoneMatrix(Chara->Animation, CharaHitPos[Hips]);
				Chara->pos = D3DXVECTOR3(Mtx._41, 0.0f, Mtx._43);
				Chara->HitFrag = false;
				Chara->framecount = 0;
			}
		}
	}

	// アニメーション終了で待機に戻る
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HitFrag = false;
	}

}

//=====================================================================================================
// ストレート（追撃パンチ１）時のアクション
//=====================================================================================================
void StraightAction(CHARA *Chara, int ControllerNum)
{
	SetAttackParticle(Chara->HitBall[RightHand].pos);

	// ヒット時追加入力でアッパーに移行
	if (Chara->HitFrag == true)
	{

		// コマンド入力
		if (Chara->CommandInput == true)
		{
			if (CheckInput(Chara->Input, CMD_Upper))
			{
				Chara->Animation->ChangeAnimation(Chara->Animation, Upper, Data[Upper].Spd);
				Chara->HitFrag = false;
				Chara->framecount = 0;
			}
		}

		// イージー入力
		else
		{
			if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(ControllerNum, BUTTON_A))
			{
				Chara->Animation->ChangeAnimation(Chara->Animation, Upper, Data[Upper].Spd);
				Chara->HitFrag = false;
				Chara->framecount = 0;
			}
		}
	}

	// アニメーション終了で待機に戻る
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HitFrag = false;
	}
}

//=====================================================================================================
// アッパー（追撃パンチ２）時のアクション
//=====================================================================================================
void UpperAction(CHARA *Chara)
{
	SetAttackParticle(Chara->HitBall[LeftHand].pos);
	// アニメーション終了で待機に戻る
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HitFrag = false;
	}
}

//=====================================================================================================
// キック時のアクション
//=====================================================================================================
void KickAction(CHARA *Chara)
{
	SetAttackParticle(Chara->HitBall[RightFoot].pos);
	// アニメーション終了で待機に戻る
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HitFrag = false;
	}
}

//=====================================================================================================
// 波動拳時のアクション
//=====================================================================================================
void HadouAction(CHARA *Chara)
{
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
}

//=====================================================================================================
// 昇竜拳時のアクション
//=====================================================================================================
void ShoryuAction(CHARA *Chara)
{
	SetShoryuParticle(Chara->HitBall[RightFoot].pos);
	SetShoryuParticle(Chara->HitBall[LeftFoot].pos);
	// アニメーション終了で待機に戻る
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HitFrag = false;
	}

}

//=====================================================================================================
// SP攻撃時のアクション
//=====================================================================================================
void SPattackAction(CHARA *Chara)
{
	SetSPattackParticle(Chara->HitBall[RightFoot].pos);
	SetSPattackParticle(Chara->HitBall[LeftFoot].pos);
	// アニメーション終了で待機に戻る
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		// モーション座標にキャラクター座標を合わせる
		Chara->pos = D3DXVECTOR3(Chara->HitBall[Hips].pos.x, 0.0f, Chara->HitBall[Hips].pos.z);
		Chara->HitFrag = false;
	}
}

//=====================================================================================================
// 投げる時のアクション
//=====================================================================================================
void ThrowAction(CHARA *Chara)
{
	// アニメーション終了で待機に戻る
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HitFrag = false;
	}
}

//=====================================================================================================
// 勝利時のアクション
//=====================================================================================================
void WinAction(CHARA *Chara)
{

}

//=====================================================================================================
// 投げ失敗時のアクション
//=====================================================================================================
void MissAction(CHARA *Chara)
{
	// アニメーション終了で待機に戻る
	if (Chara->Animation->MotionEnd == true)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Idle, Data[Idle].Spd);
		Chara->HitFrag = false;
	}
}

//=====================================================================================================
// 投げられている時のアクション
//=====================================================================================================
void ThrowedposeAction(CHARA *Chara)
{
	if (Chara->Animation->PreventAnimID != ThrowedPose)
	{
		Chara->framecount++;
	}
	// 一定時間経過で相手の投げアニメーションに合わせてダウンモーションに移行
	if (Chara->framecount == THROW_FRAME)
	{
		Chara->Animation->ChangeAnimation(Chara->Animation, Down, Data[Down].Spd);
		Chara->framecount = 0;
	}
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
			SetGuardParticle(AttackChara->HitBall[LeftHand].pos);

			// スコア
			AddScore(AttackChara, Data[Punchi].Damage);
			AddScore(DefendChara, Data[Punchi].Damage);
			// 音
			PlaySound(SE_DEFEND0);
		}
		else
		{
			// 敵のモーション変更
			DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Damage, Data[Damage].Spd);
			// ダメージ
			SubDamage(DefendChara, Data[Punchi].Damage, AttackChara->CommandInput);
			// SPゲージ増減
			AddSpGauge(AttackChara, Data[Punchi].Damage);
			AddSpGauge(DefendChara, Data[Punchi].Damage);
			// エフェクト
			SetHitParticle(AttackChara->HitBall[LeftHand].pos);
			// スコア
			AddScore(AttackChara, Data[Punchi].Damage);
			// 音
			PlaySound(SE_HIT0);
		}
		//　ヒットフラグの変更
		AttackChara->HitFrag = true;
		break;
	case Straight:
		// 敵の状態確認
		if (DefendChara->Animation->CurrentAnimID == Guard || DefendChara->Animation->CurrentAnimID == Down
			|| DefendChara->Animation->CurrentAnimID == Downpose || DefendChara->Animation->CurrentAnimID == Getup)
		{
			// エフェクト
			SetGuardParticle(AttackChara->HitBall[RightHand].pos);

			// スコア
			AddScore(AttackChara, Data[Straight].Damage);
			AddScore(DefendChara, Data[Straight].Damage);
			// 音
			PlaySound(SE_DEFEND0);
		}
		else
		{
			// 敵のモーション変更
			DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Damage, Data[Damage].Spd);
			// ダメージ
			SubDamage(DefendChara, Data[Straight].Damage, AttackChara->CommandInput);
			// SPゲージ増減
			AddSpGauge(AttackChara, Data[Straight].Damage);
			AddSpGauge(DefendChara, Data[Straight].Damage);
			// エフェクト
			SetHitParticle(AttackChara->HitBall[RightHand].pos);
			// スコア
			AddScore(AttackChara, Data[Straight].Damage);
			// 音
			PlaySound(SE_HIT0);
		}
		//　ヒットフラグの変更
		AttackChara->HitFrag = true;
		break;
	case Upper:
		// 敵の状態確認
		if (DefendChara->Animation->CurrentAnimID == Guard || DefendChara->Animation->CurrentAnimID == Down
			|| DefendChara->Animation->CurrentAnimID == Downpose || DefendChara->Animation->CurrentAnimID == Getup)
		{
			// エフェクト
			SetGuardParticle(AttackChara->HitBall[LeftHand].pos);

			// スコア
			AddScore(AttackChara, Data[Upper].Damage);
			AddScore(DefendChara, Data[Upper].Damage);
			// 音
			PlaySound(SE_DEFEND0);
		}
		else
		{
			// 敵のモーション変更
			DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Down, Data[Down].Spd);
			// ダメージ
			SubDamage(DefendChara, Data[Upper].Damage, AttackChara->CommandInput);
			// SPゲージ増減
			AddSpGauge(AttackChara, Data[Upper].Damage);
			AddSpGauge(DefendChara, Data[Upper].Damage);
			// エフェクト
			SetHitParticle(AttackChara->HitBall[LeftHand].pos);
			// スコア
			AddScore(AttackChara, Data[Upper].Damage);
			// 音
			PlaySound(SE_HIT1);
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
			SetGuardParticle(AttackChara->HitBall[RightFoot].pos);

			// スコア
			AddScore(AttackChara, Data[Kick].Damage);
			AddScore(DefendChara, Data[Kick].Damage);
			// 音
			PlaySound(SE_DEFEND0);
		}
		else
		{
			// 敵のモーション変更
			DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Damage, Data[Damage].Spd);
			// ダメージ
			SubDamage(DefendChara, Data[Kick].Damage, AttackChara->CommandInput);
			// SPゲージ増減
			AddSpGauge(AttackChara, Data[Kick].Damage);
			AddSpGauge(DefendChara, Data[Kick].Damage);
			// エフェクト
			SetHitParticle(AttackChara->HitBall[RightFoot].pos);
			// スコア
			AddScore(AttackChara, Data[Kick].Damage);
			// 音
			PlaySound(SE_HIT0);
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
			SubDamage(DefendChara, (int)(Data[Shoryu].Damage * 0.1f), AttackChara->CommandInput);
			// エフェクト
			SetGuardParticle(AttackChara->HitBall[RightFoot].pos);
			SetGuardParticle(AttackChara->HitBall[LeftFoot].pos);
			// スコア
			AddScore(AttackChara, Data[Shoryu].Damage);
			AddScore(DefendChara, Data[Shoryu].Damage);
			// 音
			PlaySound(SE_DEFEND0);
		}
		else
		{
			// 敵のモーション変更
			DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Down, Data[Down].Spd);
			// ダメージ
			SubDamage(DefendChara, Data[Shoryu].Damage, AttackChara->CommandInput);
			// SPゲージ増減
			AddSpGauge(AttackChara, Data[Shoryu].Damage);
			AddSpGauge(DefendChara, Data[Shoryu].Damage);
			// エフェクト
			SetHitParticle(AttackChara->HitBall[LeftFoot].pos);
			SetHitParticle(AttackChara->HitBall[RightFoot].pos);
			// スコア
			AddScore(AttackChara, Data[Shoryu].Damage);
			// 音
			PlaySound(SE_HIT1);
		}
		//　ヒットフラグの変更
		AttackChara->HitFrag = true;
		break;
	case SPattack:
		// 敵のモーション変更
		DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Down, Data[Down].Spd);
		// ダメージ
		SubDamage(DefendChara, Data[SPattack].Damage, AttackChara->CommandInput);
		// SPゲージ増減
		AddSpGauge(AttackChara, Data[SPattack].Damage);
		AddSpGauge(DefendChara, Data[SPattack].Damage);
		// エフェクト
		SetHitParticle(AttackChara->HitBall[RightFoot].pos);
		SetHitParticle(AttackChara->HitBall[LeftFoot].pos);
		// スコア
		AddScore(AttackChara, Data[SPattack].Damage);
		// 音
		PlaySound(SE_HIT1);
		//　ヒットフラグの変更
		AttackChara->HitFrag = true;
		break;
	case Throw:
		// ダメージ
		SubDamage(DefendChara, Data[Throw].Damage, AttackChara->CommandInput);
		// SPゲージ増減
		AddSpGauge(AttackChara, Data[Throw].Damage);
		AddSpGauge(DefendChara, Data[Throw].Damage);
		// スコア
		AddScore(AttackChara, Data[Throw].Damage);
		// ヒットフラグの変更
		AttackChara->HitFrag = true;
		break;
	default:
		break;
	}

	// ヒットストップ処理を入れる
	SetHitStop();
}
