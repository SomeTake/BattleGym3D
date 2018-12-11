//=============================================================================
// サウンド処理 [sound.cpp]
//
// Author : GP11B341 24　中込和輝
// 作成日 : 2018/8/1
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"
#include "sound.h"

// おまじない
#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )
#pragma comment ( lib, "winmm.lib" )


//*****************************************************************************
// サウンドファイルのパス
//*****************************************************************************
// サウンドファイルのパス（hの通しナンバーと合わせること）
const TCHAR* c_soundFilename[] = {
	// SE
	_T("data/SE/Defend0.wav"),
	_T("data/SE/Hit0.wav"),
	_T("data/SE/Hit1.wav"),
	_T("data/SE/Swing0.wav"),
	_T("data/SE/Select0.wav"),
	_T("data/SE/Select1.wav"),
	_T("data/SE/yattaze0.wav"),
	_T("data/SE/KO.wav"),
	_T("data/SE/effect0.wav"),
	_T("data/SE/cutin0.wav"),
	_T("data/SE/countdown0.wav"),
	// BGM
	_T("data/BGM/title.wav"),
	_T("data/BGM/tutorial.wav"),
	_T("data/BGM/training.wav"),
	_T("data/BGM/battle.wav"),
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
IDirectSound8 *g_pDirectSound = NULL;			// サウンドインターフェース
LPDIRECTSOUNDBUFFER8 sound[SOUND_MAX];			// サウンド用バッファ


//=============================================================================
// 初期化処理
//=============================================================================
// hWnd:ウィンドウハンドル
HRESULT Init_Sound( HWND hWnd )
{
	// DirectSoundオブジェクトの作成
	if (FAILED(DirectSoundCreate8(NULL, &g_pDirectSound, NULL)))
		return E_FAIL;
	
	// 協調レベル設定 
	if (FAILED(g_pDirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
		return E_FAIL;

	//ロード
	int i;
	for (i = 0; i < SOUND_MAX; i++)
	{
		sound[i] = Load_Sound(i);
	}


	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void Uninit_Sound()
{
	// サウンド用バッファの解放
	int i;
	for (i = 0; i < SOUND_MAX; i++)
	{
		SAFE_RELEASE(sound[i])
	}

	// ※サウンド用バッファ解放後に行う
	if( g_pDirectSound != NULL )
	{
		g_pDirectSound->Release();
		g_pDirectSound = NULL;
	}
}



//=============================================================================
// サウンドのロード
//=============================================================================
// no:サウンドナンバー（ヘッダに定義された列挙型定数）
LPDIRECTSOUNDBUFFER8 Load_Sound( int no )
{
	// MMIO = マルチメディア入出力、の略。
	LPDIRECTSOUNDBUFFER  pBaseBuffer = NULL;	// 曲データの総合バッファ
	LPDIRECTSOUNDBUFFER8 pBuffer = NULL;		// 曲データのバッファ
	DSBUFFERDESC buff;							// バッファ設定構造体

	HMMIO hMmio = NULL;							// MMIOハンドル
	MMIOINFO mmioInfo;							// マルチメディアデータ構造体

	MMRESULT mmRes;								// MM処理結果格納変数
	MMCKINFO riffChunk, formatChunk, dataChunk;	// チャンクデータ構造体
	DWORD size;									// データサイズ格納用
	WAVEFORMATEX pcm;							// 曲データフォーマット構造体

	LPVOID pBlock;								// セカンダリバッファのデータ書き込み先アドレス
	DWORD  dwSize;								// セカンダリバッファのサイズ

	// 1.ハンドルをもとにファイルを開く
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen((LPSTR)c_soundFilename[no], &mmioInfo, MMIO_READ);
	if(!hMmio)
		return NULL;

	// 2.ファイル解析① RIFFチャンク検索
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');						// 検索キーワード
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);			// キーワードをもとに検索
	
	if(mmRes != MMSYSERR_NOERROR)		// 見つからなかったら異常終了
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 3.ファイル解析② フォーマットチャンク検索
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');						// 検索キーワード
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);	// キーワードをもとに検索
	
	if(mmRes != MMSYSERR_NOERROR)		// 見つからなかったら異常終了
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	size = mmioRead(hMmio, (HPSTR)&pcm, formatChunk.cksize);				// 検索情報をもとに読み込み
	
	if(size != formatChunk.cksize) 		// 正しく読み込めなかったら異常終了
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	mmioAscend(hMmio, &formatChunk, 0); 

	// 4.ファイル解析③ データチャンク検索
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');						// 検索キーワード
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);		// キーワードをもとに検索
	
	if(mmRes != MMSYSERR_NOERROR) 		// 見つからなかったら異常終了
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 5.データ読み込み		
	char *pData = new char[dataChunk.cksize];								// 必要な大きさの領域を確保して
	size = mmioRead(hMmio, (HPSTR)pData, dataChunk.cksize);					// データを読み込む
	
	if(size != dataChunk.cksize)  		// 正しく読み込めなかったら異常終了
	{
		delete[] pData;
		return NULL;
	}

	// 6.曲を読み込む「セカンダリバッファ」を用意
	ZeroMemory(&buff, sizeof(DSBUFFERDESC));								// まず初期化
	buff.dwSize = sizeof(DSBUFFERDESC);										// そこから各種設定
	buff.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER;
	buff.dwBufferBytes = size;
	buff.lpwfxFormat = &pcm;

	// 総合バッファを作る
	if (FAILED(g_pDirectSound->CreateSoundBuffer(&buff, &pBaseBuffer, NULL)))
		return NULL;

	// サウンドバッファを取り出す
	if (FAILED(pBaseBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&pBuffer)))
		return NULL;

	// 取り出したら総合バッファを捨てる
	pBaseBuffer->Release();

	// 7.作られたセカンダリバッファにデータを転送
	// まずは書き込みできるようバッファをロック
	if (FAILED(pBuffer->Lock(0, 0, &pBlock, &dwSize, NULL, NULL, DSBLOCK_ENTIREBUFFER)))
		return NULL;

	memcpy(pBlock, pData, dwSize);	// データ転送
	delete[] pData;					// 元の読み込み領域を消す

	// バッファロック解除
	pBuffer->Unlock(pBlock, dwSize, 0, 0);

	// セカンダリバッファを返してようやく完了...
	return pBuffer;
}


//=============================================================================
// 音を鳴らす
//=============================================================================
// flag   :1(E_DS8_FLAG_LOOP)ならループ再生
void Play_Sound(  int no, int type, int flag/*=0*/ )
{	// 続きから鳴らすので、最初から鳴らしたい場合はSetCurrentPosition(0)をすること

	if (type == 0)
	{
		// 再生位置を先頭に戻す
		// 同じサウンドを無音時間なく連続再生するため
		sound[no]->SetCurrentPosition(0);
	}
	/*	セッティング0にすると関数を呼ぶごとに再生位置が先頭にもどるので
	BGMや連続で発生する効果音はセッティングを1にする				 */


	sound[no]->Play(0, 0, flag);
}


//=============================================================================
// 音を止める
//=============================================================================
void Stop_Sound( int no, int type)
{
	DWORD status;

	sound[no]->GetStatus(&status);
	if( status & DSBSTATUS_PLAYING )	// 鳴っていたら
	{
		sound[no]->Stop();	// 意味的にはPauseになる。
	}

	// ↑の後にこれを実行しないと再生位置が初期化されない
	if (type == 0)
	{
		// 再生位置を先頭に戻す
		// 同じサウンドを無音時間なく連続再生するため
		sound[no]->SetCurrentPosition(0);
	}
	/*	セッティング0にすると関数を呼ぶごとに再生位置が先頭にもどるので
	BGMや連続で発生する効果音はセッティングを1にする				 */

}


//=============================================================================
// 再生中かどうか調べる
//=============================================================================
// pBuffer:音を鳴らしたいサウンドデータのセカンダリバッファ
bool Is_Playing( LPDIRECTSOUNDBUFFER8 pBuffer)
{
	DWORD status;

	pBuffer->GetStatus(&status);
	if( status & DSBSTATUS_PLAYING )
	{
		return true;
	}
	return false;
}


//=============================================================================
// サウンド取得関数
// [説明]	: サウンドを取得する
// [戻り値]	: LPDIRECTSOUNDBUFFER8型　サウンド用バッファへのポインタ
// [引数]	: SOUND_NAME型 サウンドネームの番号, int型 サウンドのセッティング
//=============================================================================
LPDIRECTSOUNDBUFFER8 Get_Sound(SOUND_NAME sound_name)
{
	return	sound[sound_name];
}


void Reset_Sound(SOUND_NAME sound_name)
{

	LPDIRECTSOUNDBUFFER8 buffer = Get_Sound(sound_name);
	// 再生位置を先頭に戻す
	// 同じサウンドを無音時間なく連続再生するため
	buffer->SetCurrentPosition(0);
}
