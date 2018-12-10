//=============================================================================
//
// アニメーション処理 [AnimationModel.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
// 
//=============================================================================
#ifndef _ANIMATIONMODEL_H_
#define _ANIMATIONMODEL_H_

#include "D3DXAnimation.h"

typedef struct 
{
	UINT uiAnimID;                 // アニメーションID
	LPCSTR SetName;
	ID3DXAnimationSet *pAnimSet;   // アニメーションセット
	FLOAT fLoopTime;               // 1ループの時間
	float PlaySpeed;
	FLOAT fTrackSpeed;             // トラックスピード調節値
	FLOAT fShiftTime;              // シフトするのにかかる時間
	FLOAT fCurWeightTime;          // 現在のウェイト時間
}HLANIMATION_DESC;

class AnimationModel
{
private:
	D3DXAnimation*				m_pAnimMesh;
	//LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	HLANIMATION_DESC			*AnimationSet;
	D3DXMATRIX					m_Matrix;
	UINT						PreAnimID;
	float						m_fSpeed;
	bool						m_bIsLoop;
public:
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	UINT						CurAnimID;
public:
	AnimationModel(void);
	~AnimationModel(void);

	void InitController(D3DXAnimation* mesh);

	void Render();

	bool PlayAnimation(LPCTSTR name, bool isLoop = true);

	void Update(void);

	void SetSpeed(float speed);

	float GetSpeed(void);

	// ループ時間を設定
	bool SetLoopTime(UINT animID, FLOAT time);

	// 動作開始にかかる時間を設定
	bool SetShiftTime(UINT animID, FLOAT interval);

	// アニメーションを切り替え
	bool ChangeAnimation(UINT animID);

	// アニメーションを更新
	bool AdvanceTime(FLOAT time);

	void SetMatrix(const LPD3DXMATRIX matrix);

	D3DMATRIX GetMatrix() { return m_Matrix; }

	int GetAnimationNum() const;

	LPD3DXANIMATIONSET GetAnimationSet(int index) const;

};

#endif
