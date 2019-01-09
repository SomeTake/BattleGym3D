//=============================================================================
//
// アニメーション処理 [AnimationModel.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================

#include "AnimationModel.h"

#if 0

AnimationModel::AnimationModel(void)
{
}


AnimationModel::~AnimationModel(void)
{
}

void AnimationModel::InitController(D3DXAnimation* mesh)
{
	m_pAnimMesh = mesh;
	m_pAnimController = mesh->CloneAnimCtrl();

	// メモリの動的確保
	AnimationSet = (HLANIMATION_DESC*)malloc(sizeof(HLANIMATION_DESC) * GetAnimationNum());
	for (int i = 0; i < GetAnimationNum(); i++)
	{
		AnimationSet[i].pAnimSet = GetAnimationSet(i);
		AnimationSet[i].SetName = AnimationSet[i].pAnimSet->GetName();
		AnimationSet[i].PlaySpeed = (float)AnimationSet[i].pAnimSet->GetPeriod() / 4800;
	}
	return;
}

void AnimationModel::Render()
{
	m_pAnimMesh->Render(&m_Matrix);
}

bool AnimationModel::PlayAnimation(LPCTSTR name, bool isLoop)
{
	LPD3DXANIMATIONSET pAnimationSet = NULL;
	m_pAnimController->GetAnimationSetByName(name, &pAnimationSet);
	m_pAnimController->SetTrackAnimationSet(0, pAnimationSet);
	return true;
}

void AnimationModel::Update(void)
{
	m_pAnimController->AdvanceTime(m_fSpeed, NULL);
}

void AnimationModel::SetSpeed(float speed)
{
	m_fSpeed = speed;
}

float AnimationModel::GetSpeed(void)
{
	return AnimationSet[CurAnimID].PlaySpeed;
}

// ループ時間を設定
bool AnimationModel::SetLoopTime(UINT animID, FLOAT time)
{
	// 指定のアニメーションIDの存在をチェック
	if (animID > (UINT)GetAnimationNum())
	{
		return false;
	}

	// トラックスピード調節値を算出
	float DefTime = (float)AnimationSet[animID].pAnimSet->GetPeriod();
	AnimationSet[animID].fLoopTime = time;
	AnimationSet[animID].fTrackSpeed = DefTime / time;

	return true;
}

// 動作開始にかかる時間を設定
bool AnimationModel::SetShiftTime(UINT animID, FLOAT interval)
{
	// 指定のアニメーションIDの存在をチェック
	if (animID > (UINT)GetAnimationNum())
	{
		return false;
	}

	// シフト時間を登録
	AnimationSet[animID].fShiftTime = interval;

	return true;
}

// アニメーションを切り替え
bool AnimationModel::ChangeAnimation(UINT animID)
{
	// 指定のアニメーションIDの存在をチェック
	if (animID > (UINT)GetAnimationNum())
	{
		return false;
	}

	// 異なるアニメーションであるかをチェック
	if (CurAnimID == animID)
	{
		return false;
	}


	// 現在のアニメーションセットの設定値を取得
	D3DXTRACK_DESC TD;   // トラックの能力
	m_pAnimController->GetTrackDesc(0, &TD);

	// 今のアニメーションをトラック1に移行し
	// トラックの設定値も移行
	m_pAnimController->SetTrackAnimationSet(1, AnimationSet[CurAnimID].pAnimSet);
	m_pAnimController->SetTrackDesc(1, &TD);

	// 新しいアニメーションセットをトラック0に設定
	m_pAnimController->SetTrackAnimationSet(0, AnimationSet[animID].pAnimSet);

	// トラック0のスピードの設定
	m_pAnimController->SetTrackSpeed(0, 1.0f);

	// トラック0の位置は最初からに設定
	m_pAnimController->SetTrackPosition(0, 0.0f);

	// トラックの合成を許可
	m_pAnimController->SetTrackEnable(0, true);
	m_pAnimController->SetTrackEnable(1, true);

	// ウェイト時間を初期化
	AnimationSet[animID].fCurWeightTime = 0.0f;

	m_pAnimController->ResetTime();

	// 現在のアニメーション番号を切り替え
	PreAnimID = CurAnimID;
	CurAnimID = animID;

	return true;
}

// アニメーションを更新
bool AnimationModel::AdvanceTime(FLOAT time)
{
	// 合成中か否かを判定
	AnimationSet[CurAnimID].fCurWeightTime += time;
	if (AnimationSet[CurAnimID].fCurWeightTime <= AnimationSet[CurAnimID].fShiftTime)
	{
		// 合成中。ウェイトを算出
		FLOAT Weight = AnimationSet[CurAnimID].fCurWeightTime / AnimationSet[CurAnimID].fShiftTime;
		// ウェイトを登録
		m_pAnimController->SetTrackWeight(0, Weight);       // 現在のアニメーション
		m_pAnimController->SetTrackWeight(1, 1 - Weight);   // 前のアニメーション
	}
	else
	{
		// 合成終了中。通常アニメーションをするTrack0のウェイトを最大値に
		m_pAnimController->SetTrackWeight(0, 1.0f);       // 現在のアニメーション
		m_pAnimController->SetTrackEnable(1, false);      // 前のアニメーションを無効にする
	}

	// 時間を更新
	m_pAnimController->AdvanceTime(time, NULL);

	return true;
}

void AnimationModel::SetMatrix(const LPD3DXMATRIX matrix)
{
	m_Matrix = *matrix;
}

int AnimationModel::GetAnimationNum() const
{
	return m_pAnimController->GetMaxNumAnimationSets();
}

LPD3DXANIMATIONSET AnimationModel::GetAnimationSet(int index) const
{
	if (index >= 0 && index < GetAnimationNum())
	{
		LPD3DXANIMATIONSET pAnimSet = NULL;
		m_pAnimController->GetAnimationSet(index, &pAnimSet);
		return pAnimSet;
	}

	return NULL;
}

#endif
