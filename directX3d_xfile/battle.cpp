//=============================================================================
//
// バトル処理（プレイヤーもエネミーも呼び出す処理） [battle.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "main.h"
#include "battle.h"

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
			if (HitBC(AttackChara->HitBall[LeftHand].pos, DefendChara->HitBall[i].pos, AttackChara->HitBall[LeftHand].scl.x, DefendChara->HitBall[i].scl.x) == true)
			{
				return true;
			}
		}
		break;
	case Kick:
		// 右足と相手の各部位との判定
		for (int i = 0; i < HIT_CHECK_NUM; i++)
		{
			if (HitBC(AttackChara->HitBall[RightFoot].pos, DefendChara->HitBall[i].pos, AttackChara->HitBall[RightFoot].scl.x, DefendChara->HitBall[i].scl.x) == true)
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
			if (HitBC(AttackChara->HitBall[LeftFoot].pos, DefendChara->HitBall[i].pos, AttackChara->HitBall[LeftFoot].scl.x, DefendChara->HitBall[i].scl.x) == true)
			{
				return true;
			}
			// 右足との判定
			else if (HitBC(AttackChara->HitBall[RightFoot].pos, DefendChara->HitBall[i].pos, AttackChara->HitBall[RightFoot].scl.x, DefendChara->HitBall[i].scl.x) == true)
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
		}
		//　ヒットフラグの変更
		AttackChara->HitFrag = true;
		break;
	case Shoryu:
		// 敵の状態確認
		if (DefendChara->Animation->CurrentAnimID == Guard || DefendChara->Animation->CurrentAnimID == Down
			|| DefendChara->Animation->CurrentAnimID == Downpose || DefendChara->Animation->CurrentAnimID == Getup)
		{
			// エフェクト
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
// 引数:CHARA:与える対象のキャラクタ add :追加する点数。マイナスも可能、初期化などに。
//=============================================================================
void SubDamage(CHARA *Chara, int sub)
{
	Chara->HPzan -= sub;

	//カンスト処理
	if (Chara->HPzan < 0)
	{
		Chara->SP = 0;
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
