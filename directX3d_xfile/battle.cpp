//=============================================================================
//
// バトル処理（プレイヤーもエネミーも呼び出す処理） [battle.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "battle.h"
#include "camera.h"
#include "sound.h"
#include "particle.h"
#include "game.h"
#include "action.h"

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
bool HitSphere(D3DXVECTOR3 AttackPos, D3DXVECTOR3 DefendPos, float AttackRange, float DefendRange)
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
			if (HitSphere(AttackChara->HitBall[LeftHand].pos, DefendChara->HitBall[i].pos,
				AttackChara->HitBall[LeftHand].scl.x, DefendChara->HitBall[i].scl.x) == true)
			{
				return true;
			}
		}
		break;
	case Straight:
		// 右手と相手の各部位との判定
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			if (HitSphere(AttackChara->HitBall[RightHand].pos, DefendChara->HitBall[i].pos,
				AttackChara->HitBall[RightHand].scl.x, DefendChara->HitBall[i].scl.x) == true)
			{
				return true;
			}
		}
		break;
	case Upper:
		// 左手と相手の各部位との判定
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			if (HitSphere(AttackChara->HitBall[LeftHand].pos, DefendChara->HitBall[i].pos,
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
			if (HitSphere(AttackChara->HitBall[RightFoot].pos, DefendChara->HitBall[i].pos,
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
			if (HitSphere(AttackChara->HitBall[LeftFoot].pos, DefendChara->HitBall[i].pos,
				AttackChara->HitBall[LeftFoot].scl.x, DefendChara->HitBall[i].scl.x) == true)
			{
				return true;
			}
			// 右足との判定
			else if (HitSphere(AttackChara->HitBall[RightFoot].pos, DefendChara->HitBall[i].pos,
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
	case SPattack:
		// 両足と相手の各部位との判定
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			// 左足との判定
			if (HitSphere(AttackChara->HitBall[LeftFoot].pos, DefendChara->HitBall[i].pos,
				AttackChara->HitBall[LeftFoot].scl.x, DefendChara->HitBall[i].scl.x) == true)
			{
				return true;
			}
			// 右足との判定
			else if (HitSphere(AttackChara->HitBall[RightFoot].pos, DefendChara->HitBall[i].pos,
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
// 波動拳の当たり判定及びダメージなどの処理
// 引数:AttackChara:攻撃側 DefendChara:防御側
//=============================================================================
void HitHadou(CHARA *AttackChara, CHARA *DefendChara)
{
	// 弾同士の当たり判定
	if (DefendChara->HadouBullet.use == true)
	{
		if (HitSphere(AttackChara->HadouBullet.pos, DefendChara->HadouBullet.pos, AttackChara->HadouBullet.scl.x, DefendChara->HadouBullet.scl.x) == true)
		{
			// エフェクト
			SetHitParticle(AttackChara->HadouBullet.pos);
			SetHitParticle(DefendChara->HadouBullet.pos);
			// 使用フラグの変更
			AttackChara->HadouBullet.use = false;
			DefendChara->HadouBullet.use = false;

			// スコア
			AddScore(AttackChara, Data[Hadou].Damage);
			AddScore(DefendChara, Data[Hadou].Damage);

			// 音
			PlaySound(SE_HIT1);
		}
	}
	// 弾と相手キャラとの当たり判定
	for (int i = 0; i < HIT_CHECK_NUM; i++)
	{
		if (HitSphere(AttackChara->HadouBullet.pos, DefendChara->HitBall[i].pos, AttackChara->HadouBullet.scl.x, DefendChara->HitBall[i].scl.x) == true)
		{
			// 敵の状態確認
			if (DefendChara->Animation->CurrentAnimID == Guard || DefendChara->Animation->CurrentAnimID == Down
				|| DefendChara->Animation->CurrentAnimID == Downpose || DefendChara->Animation->CurrentAnimID == Getup)
			{
				// ダメージ
				SubDamage(DefendChara, (int)(Data[Hadou].Damage * 0.1f), AttackChara->CommandInput);

				// エフェクト
				SetGuardParticle(DefendChara->HitBall[i].pos);

				// スコア
				AddScore(AttackChara, Data[Hadou].Damage);
				AddScore(DefendChara, Data[Hadou].Damage);

				// 音
				PlaySound(SE_DEFEND0);
			}
			else
			{
				// 敵のモーション変更
				DefendChara->Animation->ChangeAnimation(DefendChara->Animation, Damage, Data[Damage].Spd);
				// ダメージ
				SubDamage(DefendChara, Data[Hadou].Damage, AttackChara->CommandInput);

				// エフェクト
				SetHitParticle(AttackChara->HadouBullet.pos);

				// スコア
				AddScore(AttackChara, Data[Hadou].Damage);

				// 音
				PlaySound(SE_HIT0);
			}
			// SPゲージ増減
			AddSpGauge(DefendChara, Data[Hadou].Damage);
			AddSpGauge(AttackChara, Data[Hadou].Damage);

			//　使用フラグの変更
			AttackChara->HadouBullet.use = false;

			// ヒットストップ処理を入れる
			SetHitStop();
			break;
		}
	}
}

//=============================================================================
// 攻撃モーションに合わせた当たり判定発生フレームの更新
//=============================================================================
void UpdateAttackCollision(CHARA *Chara)
{
	if (Chara->Animation->CurrentAnimID == Punchi || Chara->Animation->CurrentAnimID == Straight
		|| Chara->Animation->CurrentAnimID == Upper || Chara->Animation->CurrentAnimID == Kick
		|| Chara->Animation->CurrentAnimID == Hadou || Chara->Animation->CurrentAnimID == Shoryu
		|| Chara->Animation->CurrentAnimID == SPattack)
	{
		Chara->framecount++;
		if (Chara->Animation->MotionEnd == true)
		{
			Chara->framecount = 0;
		}
	}
	else
	{
		Chara->framecount = 0;
	}
}

//=============================================================================
// ダメージを与える（コマンド入力の場合ダメージを1.1倍にする）
// 引数:CHARA:与える対象のキャラクタ sub :減少させる点数。マイナスも可能、初期化などに。 AtkCharaInput:攻撃側の操作モード
//=============================================================================
void SubDamage(CHARA *Chara, int sub, bool AtkCharaInput)
{
	int *Phase = GetPhase();

	if (*Phase != PhaseReplay)
	{
		if (AtkCharaInput == true)
		{
			Chara->HPzan -= (int)(sub * 1.1f);
		}
		else
		{
			Chara->HPzan -= sub;
		}

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

	// 攻撃時の当たり判定発生処理
	if (AIChara->Animation->CurrentAnimID == Punchi || AIChara->Animation->CurrentAnimID == Straight
		|| AIChara->Animation->CurrentAnimID == Upper || AIChara->Animation->CurrentAnimID == Kick
		|| AIChara->Animation->CurrentAnimID == Hadou || AIChara->Animation->CurrentAnimID == Shoryu
		|| AIChara->Animation->CurrentAnimID == SPattack)
	{
		AIChara->framecount++;
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->framecount = 0;
		}
	}
	else
	{
		AIChara->framecount = 0;
	}

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

	int ActRand = rand() % 100;

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
						// SPゲージ消費
						AddSpGauge(AIChara, -FULL_SPGAUGE);
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
		RightstepAction(AIChara);
		break;
	case Leftstep:
		LeftstepAction(AIChara);
		break;
	case Guard:
		if (AttackFlag == false)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
		}
		break;
	case Damage:
		DamageAction(AIChara);
		break;
	case Down:
		DownAction(AIChara);
		break;
	case Downpose:
		DownposeAction(AIChara);
		break;
	case Getup:
		GetupAction(AIChara);
		break;
	case Punchi:
		SetAttackParticle(AIChara->HitBall[LeftHand].pos);
		// アニメーション終了で待機に戻る
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
			AIChara->HitFrag = false;
		}
		break;
	case Straight:
		SetAttackParticle(AIChara->HitBall[RightHand].pos);
		// アニメーション終了で待機に戻る
		if (AIChara->Animation->MotionEnd == true)
		{
			AIChara->Animation->ChangeAnimation(AIChara->Animation, Idle, Data[Idle].Spd);
			AIChara->HitFrag = false;
		}
		break;
	case Upper:
		UpperAction(AIChara);
		break;
	case Kick:
		KickAction(AIChara);
		break;
	case Hadou:
		HadouAction(AIChara);
		break;
	case Shoryu:
		ShoryuAction(AIChara);
		break;
	case SPattack:
		SPattackAction(AIChara);
		break;
	case Throw:
		ThrowAction(AIChara);
		break;
	case Win:
		WinAction(AIChara);
		break;
	case Miss:
		MissAction(AIChara);
		break;
	case ThrowedPose:
		ThrowedposeAction(AIChara);
		break;
	default:
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
				SubDamage(Chara, -HPHEAL, Chara->CommandInput);
				Gauge->value += HPHEAL;
				if (Gauge->value >= FULL_HP)
				{
					Gauge->value = FULL_HP;
				}
			}
		}
	}
	else
	{
		Chara->damagecount = 0;
	}

	Chara->SP = FULL_SPGAUGE;

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
			StopSound(BGM_TUTORIAL);
			PlaySound(BGM_TITLE);
			ReInit();
		}
	}
	else if (*phase == PhaseGame)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(ControllerNum, BUTTON_M))
		{
			SetPhase(PhasePause);
			StopSound(BGM_BATTLE);
		}
	}
	else if (*phase == PhaseTraining)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(ControllerNum, BUTTON_M))
		{
			SetPhase(PhaseTrainingPause);
			StopSound(BGM_TRAINING);
		}
	}

	// 攻撃時の当たり判定発生処理
	UpdateAttackCollision(Chara);

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
						// SPゲージ消費
						AddSpGauge(Chara, -FULL_SPGAUGE);
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
						// SPゲージ消費
						AddSpGauge(Chara, -FULL_SPGAUGE);
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
		else if (Chara->graceframe >= GRACE_VALUE)
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
		IdleAction(Chara, ControllerNum);
		break;
	case Frontwalk:
		FrontwalkAction(Chara, ControllerNum);
		break;
	case Backwalk:
		BackwalkAction(Chara, ControllerNum);
		break;
	case Rightstep:
		RightstepAction(Chara);
		break;
	case Leftstep:
		LeftstepAction(Chara);
		break;
	case Guard:
		GuardAction(Chara, ControllerNum);
		break;
	case Damage:
		DamageAction(Chara);
		break;
	case Down:
		DownAction(Chara);
		break;
	case Downpose:
		DownposeAction(Chara);
		break;
	case Getup:
		GetupAction(Chara);
		break;
	case Punchi:
		PunchiAction(Chara, ControllerNum);
		break;
	case Straight:
		StraightAction(Chara, ControllerNum);
		break;
	case Upper:
		UpperAction(Chara);
		break;
	case Kick:
		KickAction(Chara);
		break;
	case Hadou:
		HadouAction(Chara);
		break;
	case Shoryu:
		ShoryuAction(Chara);
		break;
	case SPattack:
		SPattackAction(Chara);
		break;
	case Throw:
		ThrowAction(Chara);
		break;
	case Win:
		WinAction(Chara);
		break;
	case Miss:
		MissAction(Chara);
		break;
	case ThrowedPose:
		ThrowedposeAction(Chara);
		break;
	default:
		break;
	}
}

//=============================================================================
// コマンド操作モード
//=============================================================================
void CommandInput(CHARA *Chara, int ControllerNum)
{
	int *phase = GetPhase();

	// ポーズ画面へ
	if (*phase == PhaseTutorial)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(ControllerNum, BUTTON_M))
		{
			SetPhase(PhaseTitle);
			StopSound(BGM_TUTORIAL);
			PlaySound(BGM_TITLE);
			ReInit();
		}
	}
	else if (*phase == PhaseGame)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(ControllerNum, BUTTON_M))
		{
			SetPhase(PhasePause);
			StopSound(BGM_BATTLE);
		}
	}
	else if (*phase == PhaseTraining)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(ControllerNum, BUTTON_M))
		{
			SetPhase(PhaseTrainingPause);
			StopSound(BGM_TRAINING);
		}
	}


	int input = 0;

	// 押したボタンに応じて、ビットをオンにする
	if (IsButtonTriggered(ControllerNum, BUTTON_UP))
	{
		input |= BUTTON_UP;
	}
	else if (IsButtonTriggered(ControllerNum, BUTTON_DOWN))
	{
		input |= BUTTON_DOWN;
	}
	// 2P側は左右を反転する
	else if (IsButtonPressed(ControllerNum, BUTTON_RIGHT))
	{
		input |= (ControllerNum == 0 ? BUTTON_RIGHT : BUTTON_LEFT);
	}
	else if (IsButtonPressed(ControllerNum, BUTTON_LEFT))
	{
		input |= (ControllerNum == 0 ? BUTTON_LEFT : BUTTON_RIGHT);
	}
	else if (IsButtonPressed(ControllerNum, BUTTON_LEFTUP))
	{
		input |= (ControllerNum == 0 ? BUTTON_LEFTUP : BUTTON_RIGHTUP);
	}
	else if (IsButtonPressed(ControllerNum, BUTTON_RIGHTUP))
	{
		input |= (ControllerNum == 0 ? BUTTON_RIGHTUP : BUTTON_LEFTUP);
	}
	else if (IsButtonPressed(ControllerNum, BUTTON_LEFTDOWN))
	{
		input |= (ControllerNum == 0 ? BUTTON_LEFTDOWN : BUTTON_RIGHTDOWN);
	}
	else if (IsButtonPressed(ControllerNum, BUTTON_RIGHTDOWN))
	{
		input |= (ControllerNum == 0 ? BUTTON_RIGHTDOWN : BUTTON_LEFTDOWN);
	}

	// 何も十字キーを入力していない場合
	if (!IsButtonTriggered(ControllerNum, BUTTON_UP) && !IsButtonTriggered(ControllerNum, BUTTON_DOWN)
		&& !IsButtonTriggered(ControllerNum, BUTTON_RIGHT) && !IsButtonTriggered(ControllerNum, BUTTON_LEFT)
		&& !IsButtonTriggered(ControllerNum, BUTTON_RIGHTUP) && !IsButtonTriggered(ControllerNum, BUTTON_LEFTUP)
		&& !IsButtonTriggered(ControllerNum, BUTTON_RIGHTDOWN) && !IsButtonTriggered(ControllerNum, BUTTON_LEFTDOWN))
	{
		input |= BUTTON_DEFAULT;
	}

	if (IsButtonTriggered(ControllerNum, BUTTON_A))
	{
		input |= BUTTON_A;
	}
	if (IsButtonTriggered(ControllerNum, BUTTON_B))
	{
		input |= BUTTON_B;
	}
	if (IsButtonTriggered(ControllerNum, BUTTON_C))
	{
		input |= BUTTON_C;
	}
	if (IsButtonTriggered(ControllerNum, BUTTON_X))
	{
		input |= BUTTON_X;
	}
	if (IsButtonTriggered(ControllerNum, BUTTON_START))
	{
		input |= BUTTON_START;
	}
	if (IsButtonTriggered(ControllerNum, BUTTON_M))
	{
		input |= BUTTON_M;
	}
	if (IsButtonPressed(ControllerNum, BUTTON_Y))
	{
		input |= BUTTON_Y;
	}

	// 配列を一つずつ右に移動させる
	for (int i = INPUT_MAX - 1; i > 0; i--)
	{
		Chara->Input[i] = Chara->Input[i - 1];
	}

	// 配列0番に今入力したものを入れる
	Chara->Input[0] = input;


	// 攻撃時の当たり判定発生処理
	UpdateAttackCollision(Chara);

	switch (Chara->Animation->CurrentAnimID)
	{
	case Idle:
		IdleAction(Chara, ControllerNum);
		break;
	case Frontwalk:
		FrontwalkAction(Chara, ControllerNum);
		break;
	case Backwalk:
		BackwalkAction(Chara, ControllerNum);
		break;
	case Rightstep:
		RightstepAction(Chara);
		break;
	case Leftstep:
		LeftstepAction(Chara);
		break;
	case Guard:
		GuardAction(Chara, ControllerNum);
		break;
	case Damage:
		DamageAction(Chara);
		break;
	case Down:
		DownAction(Chara);
		break;
	case Downpose:
		DownposeAction(Chara);
		break;
	case Getup:
		GetupAction(Chara);
		break;
	case Punchi:
		PunchiAction(Chara, ControllerNum);
		break;
	case Straight:
		StraightAction(Chara, ControllerNum);
		break;
	case Upper:
		UpperAction(Chara);
		break;
	case Kick:
		KickAction(Chara);
		break;
	case Hadou:
		HadouAction(Chara);
		break;
	case Shoryu:
		ShoryuAction(Chara);
		break;
	case SPattack:
		SPattackAction(Chara);
		break;
	case Throw:
		ThrowAction(Chara);
		break;
	case Win:
		WinAction(Chara);
		break;
	case Miss:
		MissAction(Chara);
		break;
	case ThrowedPose:
		ThrowedposeAction(Chara);
		break;
	}

}

//=============================================================================
// 入力の判定
//=============================================================================
bool CheckInput(int Input[], const int *command)
{
	int i = 0;
	int j = 0;


	// コマンドの最後を調べる
	while (!(command[i] & INPUT_END))
	{
		i++;
	}

	// 入力猶予時間を設定する
	int time = command[i] & ~INPUT_END;

	// 入力履歴を調べて、コマンドが正しく入力されていればtrueされていなかったらfalse
	for (i--; i >= 0; i--)
	{
		int input = command[i];

		// 履歴からコマンドの要素を探す
		while (j < INPUT_MAX && (Input[j] & input) != input)
		{
			j++;
		}

		// 入力時間を超えたり、履歴の末尾に達したらfalse
		if (j >= time || j == INPUT_MAX)
		{
			return false;
		}
	}

	// コマンドの全ての要素を見つけることができたらtrue
	return true;
}
