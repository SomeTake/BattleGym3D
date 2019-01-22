//=============================================================================
//
// バトル処理（プレイヤーもエネミーも呼び出す処理） [battle.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _BATTLE_H_
#define _BATTLE_H_

#include "D3DXAnimation.h"
#include "ball.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	CHARA_XFILE		"data/MODEL/Boy.x"				// 読み込むモデル名(ファイルパス名)

#define	VALUE_MOVE		(1.00f)											// 移動量
#define VALUE_FRONTWALK	(1.50f)											// 前歩き移動量
#define VALUE_BACKWALK	(1.00f)											// 後ろ歩き移動量
#define	VALUE_ROTATE	(D3DX_PI * 0.80f)								// 回転量
#define VALUE_ENLARGEMENT	(0.1f)										// 拡大・縮小量
#define	VALUE_ROTATE_MODEL	(D3DX_PI * 0.05f)							// 回転速度
#define VALUE_HALF		(0.5f)											// 何らかの値を半分にするために掛ける

#define	RATE_MOVE_MODEL		(0.20f)										// 移動慣性係数
#define	RATE_ROTATE_MODEL	(0.20f)										// 回転慣性係数

// バトル関係
#define MAX_DISTANCE						(250.0f)					// プレイヤー間の最大距離（これ以上は遠ざかれない）
#define MIN_DISTANCE						(30.0f)						// プレイヤー間の最小距離（これ以上は近づけない）
#define FULL_HP								(1000)						// ゲージの最大値
#define FULL_SPGUAGE						(1000)						// ゲージの最大値
#define DAMAGE_PUNCHI						(40)						// ダメージ量
#define DAMAGE_KICK							(50)						// ダメージ量
#define DAMAGE_HADOU						(100)						// ダメージ量
#define DAMAGE_SHORYU						(120)						// ダメージ量
#define DAMAGE_SPATTACK						(400)						// ダメージ量
#define DAMAGE_THROW						(150)						// ダメージ量

// 当たり判定
#define BODY_RADIUS							(10.0f)						// 体の当たり判定の半径
#define ARM_RADIUS							(5.0f)						// 手の当たり判定の半径
#define FOOT_RADIUS							(7.0f)						// 足の当たり判定の半径

// アニメーションスピード
#define ANIM_SPD_05							(0.5f)
#define ANIM_SPD_1							(1.0f)
#define ANIM_SPD_15							(1.5f)
#define ANIM_SPD_2							(2.0f)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// キャラクターのデータを管理する構造体
typedef struct {
	D3DXVECTOR3			pos;				// モデルの位置
	D3DXVECTOR3			move;				// モデルの移動量
	D3DXVECTOR3			rot;				// 現在の向き
	D3DXVECTOR3			scl;				// モデルの大きさ(スケール)
	int					IdxShadow;			// 影ID
	float				SizeShadow;			// 影のサイズ
	D3DXCOLOR			ColShadow;			// 影の色
	int					HP;					// 体力
	int					HPzan;				// 残り体力
	D3DXANIMATION		*Animation;			// アニメーション
	BALL				HitBall[HIT_CHECK_NUM];	// 当たり判定用ボール
	bool				HitFrag;			// 攻撃が当たったかどうか
}CHARA;

// キャラクターのアニメーション番号
static const char* CharaStateAnim[] =
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

// キャラクターのアニメーション番号と連動（CharaStateAnim）
enum CharaStateNum
{
	Idle,
	Frontwalk,
	Backwalk,
	Rightstep,
	Leftstep,
	Guard,
	Damage,
	Down,
	Downpose,
	Getup,
	Punchi,
	Kick,
	Hadou,
	Shoryu,
	SPattack,
	Throw,
	Win
};

// 当たり判定を発生させる場所
static const char* CharaHitPos[] =
{
	"Hips",				// 尻
	"Neck",				// 首
	"Head",				// 頭
	"LeftShoulder",		// 左肩
	"RightShoulder",	// 右肩
	"LeftHand",			// 左手
	"RightHand",		// 右手
	"LeftFoot",			// 左足
	"RightFoot",		// 右足
	"LeftForeArm",		// 左肘
	"RightForeArm",		// 右肘
	"LeftLeg",			// 左膝
	"RightLeg"			// 右膝
};

// 当たり判定発生場所と連動（CharaHitPos）
enum CharaHitNum
{
	Hips,
	Neck,
	Head,
	LeftShoulder,
	RightShoulder,
	LeftHand,
	RightHand,
	LeftFoot,
	RightFoot,
	LeftForeArm,
	RightForeArm,
	LeftLeg,
	RightLeg
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void HitAction(CHARA *AttackChara, CHARA *DefendChara);			// 攻撃が当たったときの動き

#endif
