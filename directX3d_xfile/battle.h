//=============================================================================
//
// バトル処理（プレイヤーもエネミーも呼び出す処理） [battle.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _BATTLE_H_
#define _BATTLE_H_

// 構造体やプロトタイプ宣言に必要なヘッダー
#include "D3DXAnimation.h"
#include "ball.h"
#include "hadou.h"
#include "pop.h"
#include "redgauge.h"
#include "input.h"

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

#define MOVABLE_AREA	(385.0f)										// 動き回れるフィールドの広さ

// バトル関係
// その他モーションごとの詳細データは下のData構造体配列で管理
#define MAX_DISTANCE						(250.0f)					// プレイヤー間の最大距離（これ以上は遠ざかれない）
#define MIN_DISTANCE						(30.0f)						// プレイヤー間の最小距離（これ以上は近づけない）
#define FULL_HP								(1000)						// ゲージの最大値
#define FULL_SPGAUGE						(1000)						// ゲージの最大値
#define FULL_SCORE							(99999)						// スコアの最大数
#define FIRE_FRAME							(25)						// 波動拳の発射タイミング
#define THROW_FRAME							(180)						// 投げアニメーションの途中でダメージを与える＆相手のアニメーションを変更させるタイミング
#define GRACE_VALUE							(3)							// 入力猶予の範囲フレーム数
#define HPHEAL								(10)						// チュートリアルモードでのHPの回復量
#define THROW_VALUE							(40.0f)						// 投げの当たる範囲

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// キャラクターのデータを管理する構造体
typedef struct {
	D3DXVECTOR3			pos;				// モデルの位置
	D3DXVECTOR3			move;				// モデルの移動量
	D3DXVECTOR3			rot;				// 現在の向き
	D3DXVECTOR3			scl;				// モデルの大きさ(スケール)
	int					HP;					// 体力
	int					SP;					// SPゲージ
	int					HPzan;				// 残り体力
	D3DXANIMATION		*Animation;			// アニメーション
	BALL				HitBall[HIT_CHECK_NUM];	// 当たり判定用ボール
	bool				HitFrag;			// 攻撃が当たったかどうか
	HADOU				HadouBullet;		// 波動拳構造体
	int					score;				// スコア
	POP					Popup;				// 1Por2P表示のビルボード
	int					framecount;			// アニメーションの途中で処理をするためタイミングを測るカウンタ
	int					gracetype;			// 入力猶予の種類
	int					graceframe;			// 入力猶予の時間
	bool				graceflag;			// 入力猶予の有効フラグ
	int					damagecount;		// ダメージを回復するカウント
	int					Input[INPUT_MAX];	// 入力処理用配列
	bool				CommandInput;		// コマンド入力をONにするフラグ（true:CommandInput、false:EasyInput）
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
	"win",				// ガッツポーズ（勝利時）
	"miss",				// 投げスカり
	"throwedpose"		// 投げられている最中のポーズ
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
	Win,
	Miss,
	ThrowedPose,
	AnimMax,			// アニメーションの最大数
};

// バトル用データ構造体
typedef struct
{
	int Damage;			// そのモーションによって与えるダメージ量（SPゲージ、スコアなども）
	float Spd;			// アニメーションを再生するスピード
	float ShiftTime;	// アニメーションの切り替え時間
}BATTLEDATA;

// バトル用データ構造体配列
static BATTLEDATA Data[AnimMax] = {
	{0, 1.5f, 0.1f },	// Idle
{ 0, 2.0f, 0.1f },		// Frontwalk
{ 0, 2.0f, 0.1f },		// Backwalk
{ 0, 2.0f, 0.1f },		// Rightstep
{ 0, 2.0f, 0.1f },		// Leftstep
{ 0, 1.0f, 0.1f },		// Guard
{ 0, 1.5f, 0.1f },		// Damage
{ 0, 1.5f, 0.1f },		// Down
{ 0, 1.0f, 0.1f },		// Downpose
{ 0, 1.5f, 0.1f },		// Getup
{ 40, 2.5f, 0.1f },		// Punchi
{ 50, 2.5f, 0.1f },		// Kick
{ 100, 3.0f, 0.1f },	// Hadou
{ 120, 2.0f, 0.1f },	// Shoryu
{ 400, 1.5f, 0.1f },	// SPattack
{ 150, 1.0f, 0.1f },	// Throw
{ 0, 2.0f, 0.1f },		// Win
{ 0, 1.5f, 0.1f },		// Miss
{ 0, 1.0f, 0.1f },		// Throwpose
};

// コマンド表（優先度高い順）
static const int
CMD_SPattack[] = { BUTTON_RIGHT, BUTTON_RIGHTDOWN, BUTTON_DOWN, BUTTON_LEFTDOWN, BUTTON_LEFT, BUTTON_A | BUTTON_B, INPUT_END | 20 },
CMD_Hadou[] = { BUTTON_DOWN, BUTTON_RIGHTDOWN, BUTTON_RIGHT, BUTTON_A, INPUT_END | 15 },
CMD_Shoryu[] = { BUTTON_RIGHT, BUTTON_DOWN, BUTTON_RIGHTDOWN, BUTTON_B, INPUT_END | 15 },
CMD_Throw[] = { BUTTON_A | BUTTON_B, INPUT_END | 3 },
CMD_Punchi[] = { BUTTON_A, INPUT_END | 1 },
CMD_Kick[] = { BUTTON_B, INPUT_END | 1 },
CMD_Guard[] = { BUTTON_Y, INPUT_END | 1 },
CMD_Frontwalk[] = { BUTTON_RIGHT, INPUT_END | 1 },
CMD_Backwalk[] = { BUTTON_LEFT, INPUT_END | 1 },
CMD_Rightstep[] = { BUTTON_DOWN, BUTTON_DEFAULT, BUTTON_DOWN, INPUT_END | 10 },
CMD_Leftstep[] = { BUTTON_UP, BUTTON_DEFAULT, BUTTON_UP, INPUT_END | 10 };

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
	RightLeg,
};

// 当たり判定の半径（上の発生場所と連動）
static float HitRadius[] = 
{
	10.0f,
	10.0f,
	10.0f,
	5.0f,
	5.0f,
	5.0f,
	5.0f,
	7.0f,
	7.0f,
	5.0f,
	5.0f,
	7.0f,
	7.0f
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool HitBC(D3DXVECTOR3 AttackPos, D3DXVECTOR3 DefendPos, float AttackRange, float DefendRange);	// バウンディングサークル当たり判定
bool HitCheckCToC(CHARA *AttackChara, CHARA *DefendChara);		// キャラクター同士の当たり判定
void HitAction(CHARA *AttackChara, CHARA *DefendChara);			// 攻撃が当たったときの動き
void SubDamage(CHARA *Chara, int sub);							// ダメージを与える
void AddSpGauge(CHARA *Chara, int add);							// SPゲージの増減
void HitHadou(CHARA *AttackChara, CHARA *DefendChara);			// 波動拳の当たり判定
void AddScore(CHARA *Chara, int add);							// スコアの追加
void BattleAI(CHARA *AIChara, CHARA *AnotherChara);				// バトル用AI
void SetupTutorial(CHARA *Chara, REDGAUGE *Gauge);				// チュートリアルモードでのキャラクターのHPなどの管理

void EasyInput(CHARA *Chara, int ControllerNum);				// 簡単操作
void CommandInput(CHARA *Chara, int ControllerNum);				// コマンド操作
bool CheckInput(int Input[], const int *command);				// 入力判定

#endif
