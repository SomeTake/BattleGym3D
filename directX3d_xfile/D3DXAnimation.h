//=============================================================================
//
// モデルのアニメーション処理 [D3DXAnimation.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================

#ifndef _D3DXANIMATION_H_
#define _D3DXANIMATION_H_

#include "CAllocateHierarchy.h"

typedef struct
{
	LPCSTR				SetName;				// アニメーションセットの名前
	ID3DXAnimationSet	*AnimSet;				// アニメーションセット
	float				ShiftTime;              // シフトするのにかかる時間
	float				CurWeightTime;          // 現在のウェイト時間
}ANIMATIONSTATUS;

typedef struct D3DXANIMATION D3DXANIMATION;
struct D3DXANIMATION
{
	// メンバー変数
	LPD3DXANIMATIONCONTROLLER	AnimController;	// アニメーションコントローラー
	UINT						CurrentAnimID;	// 現在再生しているアニメーションの番号
	UINT						PreventAnimID;	// 前再生していたアニメーションの番号
	ANIMATIONSTATUS				*Status;		// アニメーションセットのデータ
	AllocateHierarchy			*AllocateHier;	// x fileの各情報を保存する
	LPD3DXFRAME					FrameRoot;		// ルートフレーム	
	int							AnimSetNum;		// アニメーションセットの数
	int							KeyframeCount;  // Callback Keyframesを処理した数
	bool						MotionEnd;		// 今再生しているアニメーションは最後かどうか
	bool						StopMove;		// キャラが移動を停止しているかどうか

	// メンバ関数
	HRESULT(*InitAnimation)(D3DXANIMATION* Animation, LPCSTR SetName, int Set_No);
	void(*UninitAnimation)(D3DXANIMATION* Animation);
	void(*UpdateAnimation)(D3DXANIMATION* Animation, float Time);
	void(*DrawAnimation)(D3DXANIMATION* Animation, LPD3DXMATRIX WorldMatrix);
	void(*ChangeAnimation)(D3DXANIMATION* Animation, UINT animID, float PlaySpeed);
	// モーションを切り替える時、かかる時間
	void(*SetShiftTime)(D3DXANIMATION* Animation, UINT AnimID, float Interval);
};

struct AnimCallBackHandler : public ID3DXAnimationCallbackHandler
{
	D3DXANIMATION *AnimPointer;
	LPCSTR SetName;
	int AnimStatus;
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// アニメーションオブジェクトを作成する
D3DXANIMATION* CreateAnimationObject(void);
// モデルのx Fileを読み込む
HRESULT Load_xFile(D3DXANIMATION* D3DXAnimation, LPCTSTR filename, const char* ErrorSrc);
// アニメーション中断イベントのKeyframesを設置する
HRESULT SetupCallbackKeyframes(D3DXANIMATION* Animation, LPCSTR SetName);
// 特定のボーンマトリクスを取得
D3DXMATRIX GetBoneMatrix(D3DXANIMATION* Animation, const char* BoneName);


#endif
