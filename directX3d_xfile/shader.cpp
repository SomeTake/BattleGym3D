//====================================================================================================================================================================================
//
// シェーダ処理 [shader.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//====================================================================================================================================================================================

//====================================================================================================================================================================================
// インクルード
//====================================================================================================================================================================================
#include "main.h"	//	メインヘッダー
#include "camera.h"	//	カメラヘッダー
#include "light.h"	//	ライトヘッダー
#include "player.h"	//	プレイヤーヘッダー
#include "shader.h"	//	シェーダヘッダー
//====================================================================================================================================================================================
// グローバル変数
//====================================================================================================================================================================================
SHADER ShaderState;	//	シェーダ
//====================================================================================================================================================================================
// シェーダの初期化処理
// 関数名	：HRESULT Initialization_Shader
// 戻り値	：HRESULT
//====================================================================================================================================================================================
HRESULT Initialization_Shader(void)
{
	// 変数宣言
	LPD3DXBUFFER Error;	//	エラー

	// ポインタ変数宣言 / 初期化
	LPDIRECT3DDEVICE9 Device = GetDevice();	//	デバイスポインタ

	// 各初期化
	ShaderState.Effect = NULL;
	ShaderState.ToonTexture = NULL;
	ShaderState.WhiteTexture = NULL;

	// エフェクトの作成
	if (FAILED(D3DXCreateEffectFromFile			//	D3DXCreateEffectFromFile関数：エフェクトのASCII記述、またはバイナリ記述からエフェクトを作成する
	(
		Device,					//	エフェクトを作成するデバイスへのポインタ
		SHADER_FILE,			//	ファイル名
		NULL,					//	プリプロセッサマクロ定義の"NULL"で終わるオプションの配列
		NULL,					//	オプションのインターフェイスポインタ"ID3DXInclude"：#include、擬似命令の処理に使う
		EFFECT_COMPILE_OPTION,	//	コンパイルオプション
		NULL,					//	共有引数で使う"ID3DXEffectPool"オブジェクトへのポインタ：この値に"NULL"を指定すると、引数は共有されない
		&ShaderState.Effect,	//	コンパイルされたエフェクトが格納されたバッファへのポインタ
		&Error					//	コンパイルエラーの一覧が格納されたバッファへのポインタ
	)))
	{
		MessageBox								//	MessageBox関数：メッセージボックスの作成、表示、操作を行う
		(
			NULL,								//	作成したいメッセージボックスのオーナーウィンドウのハンドルを指定する
			(LPCSTR)Error->GetBufferPointer(),	//	表示したいメッセージを保持する、"NULL"で終わる文字列へのポインタを指定する
			"Error",							//	ダイアログボックスのタイトルを保持する、"NULL"で終わる文字列へのポインタを指定する
			MB_OK								//	メッセージボックスの内容と動作を指定する
		);
		return E_FAIL;							//	処理失敗
	}

	// リソースの再取得
	ShaderState.Effect->OnResetDevice();		//	OnResetDevice関数：リソースを再取得して、初期状態を保存する

	// シェーダテクスチャの読み込み
#if 1
	// テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		Device,						//	デバイスへのポインタ
		SHADER_TEXTURE_TOON,		//	テクスチャファイル
		&ShaderState.ToonTexture	//	テクスチャメモリ
	);
#endif

	// 正常終了
	return S_OK;
}

//====================================================================================================================================================================================
// シェーダの解放処理
// 関数名	：void Release_Shader
// 戻り値	：void
//====================================================================================================================================================================================
void Release_Shader(void)
{
	SAFE_RELEASE(ShaderState.Effect);
	SAFE_RELEASE(ShaderState.ToonTexture);
	SAFE_RELEASE(ShaderState.WhiteTexture);
}

//====================================================================================================================================================================================
// シェーダの描画処理
// 関数名	：void Draw_Shader
// 戻り値	：void
//====================================================================================================================================================================================
void Draw_Shader(void)
{
	// 変数宣言 / 初期化
	D3DXMATRIX Calc_Matrix;
	D3DCOLORVALUE Light_Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	D3DCOLORVALUE Light_Ambient = { 0.0f, 0.0f, 0.0f, 1.0f };
	UINT Pass_No;		//	パスナンバー
	UINT Pass_Index;	//	パスインデックス

	// ポインタ変数宣言 / 初期化
	LPDIRECT3DDEVICE9 Device = GetDevice();	//	デバイスポインタ
	MATRIX *Matrix = GetMatrix();				//	マトリクスポインタ
	PLAYER *Player = GetPlayer(0);				//	プレイヤーポインタ
	D3DLIGHT9 *Light = GetLight(0);			//	ライトポインタ
	
	// トゥーンシェーダ

	// テクニックの設定
	ShaderState.Effect->SetTechnique(ShaderState.Effect->GetTechniqueByName("Toon_Shader"));	//	SetTechnique関数：アクティブなテクニックを設定する
	// マトリクスの設定
	ShaderState.Effect->SetMatrix("Matrix_World", &Matrix->world);		//	SetMatrix関数：非転置行列を設定する
	// マトリクスの計算
	Calc_Matrix = Matrix->world * Matrix->view * Matrix->projection;
	// マトリクスの設定
	ShaderState.Effect->SetMatrix("Matrix_Crucible", &Calc_Matrix);	//	SetMatrix関数：非転置行列を設定する

	// ステートの設定
	// ピクセルシェーダが有効の( 固定頂点を使用する )場合
#if ENABLE_PIXEL_SHADER == 0
	// テクスチャステージ０の設定
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DSAMP_MIPFILTER);
	Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	// テクスチャステージ１の設定
	Device->SetSamplerState(1, D3DSAMP_MINFILTER, D3DSAMP_MIPFILTER);
	Device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
	Device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
#endif
	Device->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	Device->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);

	// テクニックの適用を開始
	ShaderState.Effect->Begin(&Pass_No, 0);

	// ライトの設定
	ShaderState.Effect->SetValue("Light_Color_Diffuse", &(Light + 0)->Diffuse, sizeof(D3DCOLORVALUE));	//	SetValue関数：任意のパラメータ、またはアノテーションの値を取得する
	ShaderState.Effect->SetValue("Light_Color_Ambient", &Light_Ambient, sizeof(D3DCOLORVALUE));
	ShaderState.Effect->SetValue("Light_Direction", &(Light + 0)->Direction, sizeof(D3DXVECTOR3));

	// 描画
	for (Pass_Index = 0; Pass_Index < Pass_No; Pass_Index++)
	{
		// テクニック内でパスを開始する
		ShaderState.Effect->BeginPass(Pass_Index);	//	BeginPass関数：アクティブなテクニック内で、パスを開始する

		// メッシュはサブセットや、各材料のための１つに分割されているので、それらをループ処理でレンダリングする
		for (DWORD i = 0; i < Player->NumMat; i++)
		{
			// このサブセットのマテリアルと、テクスチャを設定する
			// ピクセルシェーダが有効の( 固定頂点を使用する )場合
#if ENABLE_PIXEL_SHADER == 0
			// 固定頂点を使用する場合
			Device->SetTexture(0, Player->D3DTexture[i]);
			Device->SetTexture(1, ShaderState.ToonTexture);

			if (Player->D3DTexture[i])
			{
				Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			}
			else
			{
				Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			}
			// ピクセルシェーダを使用する場合
#else
			// テクスチャの設定
			Shader_State.Effect->SetTexture("Shade_Texture", Shader_State.Toon_Texture);

			if (Player->Direct3D_Texture[i])
			{
				Shader_State.Effect->SetTexture("Mesh_Texture", Player->Direct3D_Texture[i]);
			}
			else
			{
				Shader_State.Effect->SetTexture("Mesh_Texture", Shader_State.White_Texture);
			}
#endif
			// マテリアルカラーの設定
			ShaderState.Effect->SetValue("Material_Color_Ambient", &Player->MeshMaterial[i].Ambient, sizeof(D3DCOLORVALUE));
			ShaderState.Effect->SetValue("Material_Color_Diffuse", &Player->MeshMaterial[i].Diffuse, sizeof(D3DCOLORVALUE));

			// パス内の処理をデバイスに設定
			ShaderState.Effect->CommitChanges();	//	CommitChanges関数：レンダリングする前に、アクティブなパスの内部で発生する状態の変化を、デバイスに伝播する

			// メッシュのサブセットを描画
			Player->D3DXMesh->DrawSubset(i);
		}
		// アクティブパスの終了
		ShaderState.Effect->EndPass();	//	EndPass関数：アクティブパスを終了する
	}
	// アクティブなテクニックの終了
	ShaderState.Effect->End();	//	End関数：アクティブなテクニックを終了する

	// レンダリングステートパラメータの再設定
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	Device->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
}